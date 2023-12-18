#include "dataBase.h"
#include <time.h>
#include "functions.h"

DataBase::DataBase()
{
	
}

DataBase::~DataBase()
{
	
}

char DataBase::find(std::string& str) 
{
	int indexToFind = -1;
	std::string genreToFind = "\0";
	std::string channelToFind = "\0";
	std::string programToFind = "\0";
	std::string showManToFind = "\0";
	
	std::vector<char*> miniSplited;
	std::vector<char*> splited = split(str, '-');
	for(uint i = 1; i < splited.size(); i++)
	{
		std::string text = splited[i];
		miniSplited = split(text, '=');
		if(miniSplited.size() != 2)
		{
			printf("%s is not correct form\n", splited[i]);
			return -1;
		}
		text = miniSplited[0];
		if(text == "index")
		{
			if(stringToInt(indexToFind, miniSplited[1]) == true)
			{
				return -1;
			}
		}
		else if(text == "genre")
		{
			genreToFind = miniSplited[1];
		}
		else if(text == "channel")
		{
			channelToFind = miniSplited[1];
		}
		else if(text == "program")
		{
			programToFind = miniSplited[1];
		}
		else if(text == "showman")
		{
			showManToFind = miniSplited[1];
		}
	}
	
	uint counter = 0;
	TVshowsProgram* tv;
	TVshowsPrograms.begin();
	for(uint i = 0; i < TVshowsPrograms.size(); i++)
	{
		tv =  TVshowsPrograms.get();
		
		if(
		(indexToFind == tv->index || indexToFind == -1) &&
		(genreToFind == "\0"      || genreToFind == tv->indexOfTVprogram->indexOfGenre->name) 	&&
		(programToFind == "\0"    || programToFind == tv->indexOfTVprogram->name) 				&&
		(channelToFind == "\0"    || channelToFind == tv->indexOfChanel->name)					&&
		(showManToFind == "\0"    || showManToFind == tv->indexTVpresenter->name))
		{
			counter++;
		}
		
		TVshowsPrograms++;
	}
	if(counter == 0)
	{
		printf("nothing was found by your request\n");
		return 0;
	}
	printTittle(stdout);
	TVshowsPrograms.begin();
	for(uint i = 0; i < TVshowsPrograms.size(); i++)
	{
		tv =  TVshowsPrograms.get();
		
		if(
		(indexToFind == tv->index || indexToFind == -1) &&
		(genreToFind == "\0"      || genreToFind == tv->indexOfTVprogram->indexOfGenre->name) 	&&
		(programToFind == "\0"    || programToFind == tv->indexOfTVprogram->name) 				&&
		(channelToFind == "\0"    || channelToFind == tv->indexOfChanel->name)					&&
		(showManToFind == "\0"    || showManToFind == tv->indexTVpresenter->name))
		{
			printElement(stdout, tv);
		}
		
		TVshowsPrograms++;
	}
	return 0;
}

char DataBase::addAnalizer(std::vector<char*> splited)
{
	if(splited.size() < 5)
	{
		printf("too few values\n");
		return -1;
	}
	
	struct tm startTime;
	struct tm endTime;
	
	char* channel = splited[0];
	if(dataHandler(startTime, splited[1]) == false) {printf("incorrect start time\n"); return -1;};
	if(dataHandler(endTime,   splited[2]) == false) {printf("incorrect end time\n"); return -1;};
	char* name = splited[3];
	char* genr = splited[4];
	char* showManName = splited[5];
	char* showManRemplacementName = splited[6];
	const char* description = "-";
	if(splited.size() == 8)
	{
		description = splited[7];
	}
	
	showMan*   sm = nullptr;
	if(showManName != "-")
	{
			   sm = showMans.add({showMans.size(),         showManName});
	}
	showMan*   rp = nullptr;
	if(showManRemplacementName != "-")
	{
			   rp = showMans.add({showMans.size(),         showManRemplacementName});
	}
	Genre*     gr = genres.add({genres.size(),         genr});
	TVprogram* pr = TVprograms.add({TVprograms.size(), name, gr, description});
	Chanel*    ch = chanels.add({chanels.size(),       channel});
	
	TVshowsProgram tv = {TVshowsPrograms.size(), startTime, endTime, sm, rp, pr, ch};
	TVshowsPrograms.add(tv);
	return 0;
}

char DataBase::add(std::string& str) 
{
	std::vector<char*> splited = split2(str, '\"');
	if(splited.size() == 1)
	{
		FILE* f = fopen(splited[0], "r");
		if(f == nullptr)
		{
			printf("file path is incorrect\n");
			return -1;
		}
		
		char res = 0;
		fseek(f, 0, SEEK_END);
		uint lSize = ftell(f);
		rewind(f);
		fseek(f, 0, SEEK_SET);
		
		
		char* buffer = new char[lSize];
		fread(buffer, 1, lSize, f);
		std::string bufferStr = buffer;
		std::vector<char*> full = split(bufferStr, 10);
		
		
		for(uint i = 0; i < full.size(); i++)
		{
			bufferStr = full[i];
			std::vector<char*> spliter = split2(bufferStr, '\"');
			res = addAnalizer(spliter);
			if(res != 0)
			{
				printf("error\n");
				break;
			}
		}
		
		fclose(f);
		calculateSpacesArray();
		return 0;
	}
	calculateSpacesArray();
	return addAnalizer(splited);
}

void DataBase::remove(uint index)
{
	uint indexInList = 0;
	
	TVshowsProgram* tv;
	TVshowsPrograms.begin();
	for(uint i = 0; i < TVshowsPrograms.size(); i++)
	{
		tv =  TVshowsPrograms.get();
		if(index == tv->index)
		{
			break;
		}
		indexInList++;
		TVshowsPrograms++;
	}
	TVshowsPrograms.pop(indexInList);
	
	TVshowsPrograms.begin();
	for(uint i = 0; i < TVshowsPrograms.size(); i++)
	{
		tv =  TVshowsPrograms.get();
		if(tv->index > index)
		{
			tv->index--;
		}
		TVshowsPrograms++;
	}
}

void DataBase::sort(std::string command)
{
	if(command == "channel")
	{
		TVshowsPrograms.sort([](TVshowsProgram &x, TVshowsProgram &y) { return x.indexOfChanel->name < y.indexOfChanel->name;});
	}
	else if(command == "genre")
	{
		TVshowsPrograms.sort([](TVshowsProgram &x, TVshowsProgram &y) { return x.indexOfTVprogram->indexOfGenre->name < y.indexOfTVprogram->indexOfGenre->name;});
	}
	else if(command == "index")
	{
		TVshowsPrograms.sort([](TVshowsProgram &x, TVshowsProgram &y) { return x.index < y.index;});
	}
	else if(command == "showman")
	{
		TVshowsPrograms.sort([](TVshowsProgram &x, TVshowsProgram &y) { return x.indexTVpresenter->name < y.indexTVpresenter->name;});
	}
	else
	{
		printf("sort parametr unknow\n");
	}
}

void DataBase::printTittle(FILE* f)
{
	for(uint i = 0; i < 10; i++)
	{
		fprintf(f, "%s", tableNames[i]);
		for(uint j = 0; j < spacesArray[i] - namesArray[i] + distBetween; j++)
		{
			fprintf(f, " ");
		}
	}
	fprintf(f, "\n");
}

void DataBase::printElement(FILE* f, TVshowsProgram* tv)
{
	int temp;
		
	temp = numberToStringSize(tv->index);
	fprintf(f, "%d", tv->index);
	for(uint i = 0; i < spacesArray[0] - temp + distBetween; i++) { fprintf(f, " "); }
	
	fprintf(f, "%s", tv->indexOfChanel->name.c_str());
	for(uint i = 0; i < spacesArray[1] - tv->indexOfChanel->name.size() + distBetween; i++) { fprintf(f, " "); }
	
	temp = numberToStringSize(tv->indexOfChanel->index);
	fprintf(f, "%d", tv->indexOfChanel->index);
	for(uint i = 0; i < spacesArray[2] - temp + distBetween; i++) { fprintf(f, " "); }
	
	fprintf(f, "%s",  tv->indexOfTVprogram->name.c_str());
	for(uint i = 0; i < spacesArray[3] - tv->indexOfTVprogram->name.size() + distBetween; i++) { fprintf(f, " "); }
	
	fprintf(f, "%s", tv->indexOfTVprogram->indexOfGenre->name.c_str());
	for(uint i = 0; i < spacesArray[4] - tv->indexOfTVprogram->indexOfGenre->name.size() + distBetween; i++) { fprintf(f, " "); }
	
	if(tv->indexTVpresenter == nullptr) { fprintf(f, "-"); for(uint i = 0; i < spacesArray[5] - 1 + distBetween; i++) { fprintf(f, " "); }}
	else{ fprintf(f, "%s", tv->indexTVpresenter->name.c_str()); for(uint i = 0; i < spacesArray[5] - tv->indexTVpresenter->name.size() + distBetween; i++) { fprintf(f, " "); } }
	
	if(tv->indexTTVpresenterReplacement == nullptr) { fprintf(f, "-"); for(uint i = 0; i < spacesArray[6] - 1 + distBetween; i++) { fprintf(f, " "); }}
	else{ fprintf(f, "%s", tv->indexTTVpresenterReplacement->name.c_str()); for(uint i = 0; i < spacesArray[6] - tv->indexTTVpresenterReplacement->name.size() + distBetween; i++) { fprintf(f, " "); } }
	
	printDate(f, &tv->startTime);
	for(uint i = 0; i < distBetween; i++) { fprintf(f, " "); }
	printDate(f, &tv->endTime);
	for(uint i = 0; i < distBetween; i++) { fprintf(f, " "); }
	fprintf(f, "%s\n", tv->indexOfTVprogram->description.c_str());
}

void DataBase::calculateSpacesArray()
{
	TVshowsProgram* tv;
	int temp;
	TVshowsPrograms.begin();
	for(uint i = 0; i < TVshowsPrograms.size(); i++)
	{
		tv =  TVshowsPrograms.get();
		temp = numberToStringSize(tv->index);
		if(spacesArray[0] < temp) 												{ spacesArray[0] = temp;}
		if(spacesArray[1] < tv->indexOfChanel->name.size()) 					{ spacesArray[1] = tv->indexOfChanel->name.size();}
		temp = numberToStringSize(tv->indexOfChanel->index);
		if(spacesArray[2] < temp) 												{ spacesArray[2] = temp;}
		if(spacesArray[3] < tv->indexOfTVprogram->name.size())  				{ spacesArray[3] = tv->indexOfTVprogram->name.size();}
		if(spacesArray[4] < tv->indexOfTVprogram->indexOfGenre->name.size())	{ spacesArray[4] = tv->indexOfTVprogram->indexOfGenre->name.size();}
		
		if(tv->indexTVpresenter 			!= nullptr && spacesArray[5] < tv->indexTVpresenter->name.size())  							{ spacesArray[5] = tv->indexTVpresenter->name.size();}
		if(tv->indexTTVpresenterReplacement != nullptr && spacesArray[6] < tv->indexTTVpresenterReplacement->name.size())  				{ spacesArray[6] = tv->indexTTVpresenterReplacement->name.size();}
		TVshowsPrograms++;
	}
}

void DataBase::print(std::string command)
{
	
	std::vector<char*> splited = split2(command, '\"');
	FILE* f = stdout;
	if(splited.size() > 1)
	{
		return;
	}
	if(splited.size() == 1)
	{
		f = fopen(splited[0], "w");
		if(f==nullptr)
		{
			printf("wrong file name\n");
			return;
		}
	}
	TVshowsProgram* tv;
	int temp;
	printTittle(f);
	
	TVshowsPrograms.begin();
	for(uint j = 0; j < TVshowsPrograms.size(); j++)
	{
		tv =  TVshowsPrograms.get();
		printElement(f, tv);
		TVshowsPrograms++;
	}
	if(splited.size() == 1)
	{
		fclose(f);
	}
}

void DataBase::save(const char* filename) 
{
	FILE* f = fopen(filename, "wb");
	if(f == NULL)
	{
		printf("file couldn't create\n");
		return;
	}
	uint size = 0;
	uint sizeStr = 0;
	
	genres.begin();
	size = genres.size();
	fwrite(&size, sizeof(uint), 1, f);
	for(uint i = 0; i < size; i++)
	{
		sizeStr = genres.get()->name.size();
		fwrite(&genres.get()->index, 		sizeof(uint), 1, f);
		fwrite(&sizeStr,  					sizeof(uint), 1, f);
		fwrite(genres.get()->name.c_str(),  sizeof(char), genres.get()->name.size(), f);
		genres++;
	}
	
	showMans.begin();
	size = showMans.size();
	fwrite(&size, sizeof(uint), 1, f);
	for(uint i = 0; i < size; i++)
	{
		sizeStr = showMans.get()->name.size();
		fwrite(&showMans.get()->index, 		 sizeof(uint), 1, 	   f);
		fwrite(&sizeStr,  					 sizeof(uint), 1, 	   f);
		fwrite(showMans.get()->name.c_str(), sizeof(char), sizeStr, f);
		showMans++;
	}
	
	chanels.begin();
	size = chanels.size();
	fwrite(&size, sizeof(uint), 1, f);
	for(uint i = 0; i < size; i++)
	{
		sizeStr = chanels.get()->name.size();
		fwrite(&chanels.get()->index, 		sizeof(uint), 1, 						 f);
		fwrite(&sizeStr,  					sizeof(uint), 1, 						 f);
		fwrite(chanels.get()->name.c_str(), sizeof(char), chanels.get()->name.size(), f);
		chanels++;
	}
	
	TVprograms.begin();
	size = TVprograms.size();
	fwrite(&size, sizeof(uint), 1, f);
	for(uint i = 0; i < size; i++)
	{
		sizeStr = TVprograms.get()->name.size();
		fwrite(&TVprograms.get()->index, 				sizeof(uint),		 1, f);
		fwrite(&sizeStr, 								sizeof(uint),		 1, f);
		fwrite(TVprograms.get()->name.c_str(),  		sizeof(char), TVprograms.get()->name.size(), f);
		
		sizeStr = TVprograms.get()->description.size();
		fwrite(&sizeStr, 								sizeof(uint),		 1, f);
		fwrite(TVprograms.get()->description.c_str(),   sizeof(char), TVprograms.get()->description.size(), f);
		fwrite(&TVprograms.get()->indexOfGenre->index,  sizeof(uint), 1, f);
		TVprograms++;
	}
	
	TVshowsPrograms.begin();
	size = TVshowsPrograms.size();
	fwrite(&size, sizeof(uint), 1, f);
	uint minusOne = 0xFF'FF'FF'FF;
	for(uint i = 0; i < size; i++)
	{
		fwrite(&TVshowsPrograms.get()->index, 								sizeof(uint),  		1, f);
		fwrite(&TVshowsPrograms.get()->startTime,  							sizeof(struct tm),  1, f);
		fwrite(&TVshowsPrograms.get()->endTime,  							sizeof(struct tm),	1, f);
		
		if(TVshowsPrograms.get()->indexTVpresenter == nullptr) { 			fwrite(&minusOne, sizeof(uint), 1, f);}
		else {fwrite(&TVshowsPrograms.get()->indexTVpresenter->index, sizeof(uint), 1, f);}
		
		if(TVshowsPrograms.get()->indexTTVpresenterReplacement == nullptr) { fwrite(&minusOne, sizeof(uint), 1, f);}
		else { fwrite(&TVshowsPrograms.get()->indexTTVpresenterReplacement->index,	sizeof(uint), 1, f);}
		
		fwrite(&TVshowsPrograms.get()->indexOfTVprogram->index, 			sizeof(uint), 		1, f);
		fwrite(&TVshowsPrograms.get()->indexOfChanel->index,				sizeof(uint), 		1, f);
		TVshowsPrograms++;
	}

	fclose(f);
}

void DataBase::load(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	if(f == nullptr)
	{
		printf("file couldn't get save file\n");
		return;
	}
	
	uint indexToFind = 0;
	char* str;
	uint str_size = 0;
	uint size = 0;
	fread(&size, sizeof(uint), 1, f);
	for(uint i = 0; i < size; i++)
	{
		Genre ge;
		fread(&ge.index, sizeof(uint), 1, f);
		fread(&str_size, sizeof(uint), 1, f);
		str = new char[str_size + 1];
		str[str_size] = 0;
		fread(str, sizeof(char), str_size, f);
		ge.name = std::string(str);
		
		genres.add(ge);
	}
	
	fread(&size, sizeof(uint), 1, f);
	for(uint i = 0; i < size; i++)
	{
		showMan sm;
		fread(&sm.index, sizeof(uint), 1, f);
		fread(&str_size, sizeof(uint), 1, f);
		str = new char[str_size + 1];
		str[str_size] = 0;
		fread(str, sizeof(char), str_size, f);
		sm.name = std::string(str);
		
		showMans.add(sm);
	}
	
	fread(&size, sizeof(uint), 1, f);
	for(uint i = 0; i < size; i++)
	{
		Chanel ch;
		fread(&ch.index, sizeof(uint), 1, f);
		fread(&str_size, sizeof(uint), 1, f);
		str = new char[str_size + 1];
		str[str_size] = 0;
		fread(str, sizeof(char), str_size, f);
		ch.name = std::string(str);
		
		chanels.add(ch);
	}
	
	fread(&size, sizeof(uint), 1, f);
	for(uint i = 0; i < size; i++)
	{
		TVprogram pr;
		fread(&pr.index, sizeof(uint), 1, f);
		fread(&str_size, sizeof(uint), 1, f);
		str = new char[str_size + 1];
		str[str_size] = 0;
		fread(str, sizeof(char), str_size, f);
		pr.name = std::string(str);
		
		
		fread(&str_size, sizeof(uint), 1, f);
		str = new char[str_size + 1];
		str[str_size] = 0;
		fread(str, sizeof(char), str_size, f);
		pr.description = std::string(str);
		
		
		fread(&indexToFind,  sizeof(uint), 1, f);
		
		
		genres.begin();
		for(uint i = 0; i < genres.size(); i++)
		{
			if(genres.get()->index == indexToFind)
			{
				pr.indexOfGenre = genres.get();
			}
			genres++;
		}
		
		TVprograms.add(pr);
	}
	
	
	fread(&size, sizeof(uint), 1, f);
	for(uint i = 0; i < size; i++)
	{
		TVshowsProgram tv;
		fread(&tv.index, sizeof(uint), 			1, f);
		fread(&tv.startTime, sizeof(struct tm), 1, f);
		fread(&tv.endTime,   sizeof(struct tm), 1, f);
		
		fread(&indexToFind, sizeof(uint),	1, f);
		if(indexToFind == 0xFF'FF'FF'FF)
		{
			tv.indexTVpresenter = nullptr;
		}
		else
		{
			showMans.begin();
			for(uint i = 0; i < showMans.size(); i++)
			{
				if(showMans.get()->index == indexToFind)
				{
					tv.indexTVpresenter = showMans.get();
				}
				showMans++;
			}
		}
		
		fread(&indexToFind, sizeof(uint),	1, f);
		if(indexToFind == 0xFF'FF'FF'FF)
		{
			tv.indexTTVpresenterReplacement = nullptr;
		}
		else
		{
			showMans.begin();
			for(uint i = 0; i < showMans.size(); i++)
			{
				if(showMans.get()->index == indexToFind)
				{
					tv.indexTTVpresenterReplacement = showMans.get();
				}
				showMans++;
			}
		}
		
		fread(&indexToFind, sizeof(uint),	1, f);
		TVprograms.begin();
		for(uint i = 0; i < TVprograms.size(); i++)
		{
			if(TVprograms.get()->index == indexToFind)
			{
				tv.indexOfTVprogram = TVprograms.get();
			}
			TVprograms++;
		}
		
		
		fread(&indexToFind, sizeof(uint),	1, f);
		chanels.begin();
		for(uint i = 0; i < chanels.size(); i++)
		{
			if(chanels.get()->index == indexToFind)
			{
				tv.indexOfChanel = chanels.get();
			}
			chanels++;
		}
		
		TVshowsPrograms.add(tv);
	}
	
	fclose(f);
	calculateSpacesArray();
}