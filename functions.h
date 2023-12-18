#pragma once
#include <time.h>
#include <vector>
#include <string>

std::vector<char*> split(std::string& str, const char splitSymbol);
std::vector<char*> split2(std::string& str, const char splitSymbol);

bool dataHandler(struct tm& date, char* dataStr);
void printDate(FILE* f, struct tm* date);

int numberToStringSize(int number);
char stringToInt(int& number, char* str);
bool is_digit(unsigned char symbol);
int checkDayByMouthAndYear(int m, int y);