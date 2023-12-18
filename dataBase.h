#pragma once
#include <string>
#include <stdio.h>
#include <vector>
#include "Prerequisites.h"
#include "List.h"

struct Genre
{
	uint index;
	std::string name;
	
	bool operator==(const Genre& g1) const
	{
		if(this->name == g1.name)
		{
			return true;
		}
		return false;
	}
};

struct TVprogram
{
	uint index;
	std::string name;
	Genre* indexOfGenre;
	std::string description;
	
	bool operator==(const TVprogram& p1) const
	{
		if(this->name == p1.name && this->indexOfGenre == p1.indexOfGenre)
		{
			return true;
		}
		return false;
	}
};

struct Chanel
{
	uint index;
	std::string name;
	
	bool operator==(const Chanel& c1) const
	{
		if(this->name == c1.name)
		{
			return true;
		}
		return false;
	}
};

struct showMan
{
	uint index;
	std::string name;
	
	bool operator==(const showMan& s1) const
	{
		if(this->name == s1.name)
		{
			return true;
		}
		return false;
	}
};

struct TVshowsProgram
{
	uint index;
	struct tm startTime;
	struct tm endTime;
	showMan* indexTVpresenter;
	showMan* indexTTVpresenterReplacement;
	TVprogram* indexOfTVprogram;
	Chanel* indexOfChanel;
	
	bool operator==(const TVshowsProgram& t1) const
	{
		if(this->indexOfTVprogram == t1.indexOfTVprogram && this->indexOfChanel == t1.indexOfChanel)
		{
			return true;
		}
		return false;
	}
};


class DataBase
{
public:
	DataBase();
	~DataBase();
	
	char find(std::string& str);
	char add(std::string& str);
	void remove(uint index);
	void sort(std::string command);
	void print(std::string command);
	void save(const char* filename);
	void load(const char* filename);
private:
	char addAnalizer(std::vector<char*> splited);
	void printTittle(FILE* f);
	void printElement(FILE* f, TVshowsProgram* tv);
	void calculateSpacesArray();
private:
	list<showMan>         showMans;
	list<Genre>           genres;
	list<TVprogram>       TVprograms;
	list<Chanel>          chanels;
	list<TVshowsProgram>  TVshowsPrograms;
	
	//print data
	int namesArray [10] = {5, 7, 14, 7, 5, 7, 12, 10,  8, 0};
	int spacesArray[10] = {5, 7, 14, 7, 5, 7, 12, 16, 16, 0};
	const char* tableNames[10] = {
		"index",
		"channel",
		"channel number",
		"program",
		"genre",
		"showman",
		"remplacement",
		"start time",
		"end time",
		"description",
	};
	uint distBetween = 5;
};