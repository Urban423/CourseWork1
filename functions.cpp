#include "functions.h"
#include <stdio.h>

std::vector<char*> split(std::string& str, const char splitSymbol)
{
	str[str.size()] = splitSymbol;
	int firstLetterAfterSplit = 0;
	std::vector<char*> charList;
	for(int i = 0; i < str.size() + 1; i++)
	{
		if(str[i] == splitSymbol)
		{
			int size = i - firstLetterAfterSplit;
			if(size > 0)
			{
				char* new_word = new char[size + 1];
				for(int j = firstLetterAfterSplit; j < i; j++)
				{
					new_word[j - firstLetterAfterSplit] = str[j];
				}
				new_word[size] = '\0';
				charList.push_back(new_word);
			}
			firstLetterAfterSplit = i + 1;
			continue;
		}
	}
	str[str.size()] = '\n';
	return charList;
}

std::vector<char*> split2(std::string& str, const char splitSymbol)
{
	int firstLetterAfterSplit = 0x00'FF'FF'FF;
	std::vector<char*> charList;
	for(int i = 0; i < str.size() + 1; i++)
	{
		if(str[i] == splitSymbol)
		{
			int size = i - firstLetterAfterSplit;
			if(size > 0)
			{
				char* new_word = new char[size + 1];
				for(int j = firstLetterAfterSplit; j < i; j++)
				{
					new_word[j - firstLetterAfterSplit] = str[j];
				}
				new_word[size] = '\0';
				charList.push_back(new_word);
				firstLetterAfterSplit = 0x00'FF'FF'FF;
				continue;
			}
			firstLetterAfterSplit = i + 1;
			continue;
		}
	}
	return charList;
}

bool dataHandler(struct tm& date, char* dataStr)
{
	std::string big = dataStr;
	std::vector<char*> bigOne = split(big, ' ');
	if(bigOne.size() != 2)
	{
		return false;
	}
	std::string time = bigOne[0];
	std::string data = bigOne[1];
	std::vector<char*> timeOne = split(time, ':');
	std::vector<char*> dateOne = split(data, '.');
	if(timeOne.size() != 2 || dateOne.size() != 3)
	{
		return false;
	}
	
	if(stringToInt(date.tm_hour, timeOne[0])) { return false;};
	if(stringToInt(date.tm_min,  timeOne[1])) { return false;};
	if(stringToInt(date.tm_mday, dateOne[0])) { return false;};
	if(stringToInt(date.tm_mon,  dateOne[1])) { return false;};
	if(stringToInt(date.tm_year, dateOne[2])) { return false;};
	
	if(date.tm_hour > 23 || date.tm_min > 59 || date.tm_mon > 12 || date.tm_mday > checkDayByMouthAndYear(date.tm_mon, date.tm_year)) { return false;}
	
	date.tm_mon  -= 1;
	date.tm_year -= 1900;
	return true;
}

void printDate(FILE* f, struct tm* date)
{
	if(date->tm_hour < 10)
	{
		fprintf(f, "0");
	}
	fprintf(f, "%d:", date->tm_hour);
	if(date->tm_min < 10)
	{
		fprintf(f, "0");
	}
	fprintf(f, "%d ", date->tm_min);
	if(date->tm_mday < 10)
	{
		fprintf(f, "0");
	}
	fprintf(f, "%d.", date->tm_mday);
	if(1 + date->tm_mon < 10)
	{
		fprintf(f, "0");
	}
	fprintf(f, "%d.%d", 1 + date->tm_mon,   1900 + date->tm_year);
}

bool is_digit(unsigned char symbol)
{
	if(47 < symbol && symbol < 58)
	{
		return true;
	}
	return false;
}



int numberToStringSize(int number)
{
	int size = 0;
	if(number == 0)
	{
		return 1;
	}
	while(number > 0)
	{
		number/=10;
		size++;
	}
	return size;
}

char stringToInt(int& number, char* str)
{
	number = 0;
	while(*str != '\0')
	{
		if(is_digit(*str) == false)
		{
			return 1;
		}
		number *= 10;
		number += *str - 48;
		str++;
	}
	return 0;
}

int checkDayByMouthAndYear(int m, int y)
{
	int daysCount[12] = {
		31, //january
		28, //february
		31, //march
		30, //april
		31, //may
		30, //june
		31, //july
		31, //august
		30, //september
		31, //october
		30, //november
		31  //december
	};
	if(m == 2)
	{
		return  daysCount[m - 1] + (y % 4 == 0 && ((y % 100 == 0 && y % 400 == 0) || y % 100 != 0));
	}
	return daysCount[m - 1];
}
