#include <iostream>
#include <string>
#include <vector>
#include "dataBase.h"
#include "functions.h"
#include <algorithm>
using namespace std;

const char* commands[] = {
	"help",    //0
	"exit",    //1
	"print",   //2
	"add ",    //3
	"find ",   //4
	"sort ",   //5
	"remove ", //6
};	

const char* commandsDescriptions[] = {
	":\n\tshow u all the commands ant their descriptions",    //0
	":\n\tfinish the main programm",					         //1
	":\n\twill print the table of data base",			     //2
	"\b:\n\tadd \"Channel Name\" \"start time of the tv programm\" \"end time of the tv programm\" \"programm name\" \"programm genre\" \"programm description\"\n\tadd new data to the data base",     //3
	"\b:\n\tfind {conditions}\n\tconditions:-index=""-genre=""-program=""-channel=""\n\tprint all the elementd of data base which satisfy the conditions",   //4
	"\b:\n\tsort {parametr}\n\tparametrs: {index}, {genre}, {channel}\n\twill sort the data base by the parametr",   //5
	"\b:\n\tremove {index}\n\tremove the element of data base by index", //6
};	

int main()
{
	system("cls");
	const char* saveFile = "./save.txt";
	uint commands_size = sizeof(commands) / sizeof(const char*);
	string command;
	DataBase dataBase;
	dataBase.load(saveFile);
	
	while(1)
	{
		printf("input command: ");
		getline(cin, command);
		if(command == "0" ||command == commands[0])
		{
			for(uint i = 0; i < commands_size; i++)
			{
				printf("%d) %s%s\n", i, commands[i], commandsDescriptions[i]);
			}
		}
		else if(command == "1" ||command == commands[1])
		{
			break;
		}
		else if(command == "2")
		{
			command[0] = ' ';
			dataBase.print(command);
		}
		else if(command.find(commands[2]) == 0)
		{
			command[0] = ' ';
			command[1] = ' ';
			command[2] = ' ';
			command[3] = ' ';
			command[4] = ' ';
			dataBase.print(command);
		}
		else if(command.find("3") == 0)
		{
			command[0] = ' ';
			dataBase.add(command);
		}
		else if(command.find(commands[3]) == 0)
		{
			command[0] = ' ';
			command[1] = ' ';
			command[2] = ' ';
			dataBase.add(command);
		}
		else if(command.find("4") == 0 || command.find(commands[4]) == 0)
		{
			command[0] = '-';
			command[1] = '-';
			command[2] = '-';
			command[3] = '-';
			dataBase.find(command);
		}
		else if(command.find("5") == 0 || command.find(commands[5]) == 0)
		{
			std::vector<char*> l = split(command, ' ');
			dataBase.sort(l[1]);
		}
		else if(command.find("6") == 0 || command.find(commands[6]) == 0)
		{
			int number = 0;
			for(uint i = 0; i < 6; i++) {command[i] = ' ';};
			command.erase(std::remove(command.begin(), command.end(), ' '), command.end());
			if(stringToInt(number, const_cast<char*>(command.c_str())) == true)
			{
				printf("'%s' is not number\n", command.c_str());
			}
			dataBase.remove(number);
		}
		else
		{
			printf("command: \"%s\" is unknown.\nuse \"help\" to know more\n", command.c_str());
		}
		printf("\n");
	}
	dataBase.save(saveFile);
	return 0;
}