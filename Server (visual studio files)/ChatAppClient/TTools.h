#pragma once
#include <string>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <map>
//Enum for colouring the console
const enum ErrorRank { RED = 4, YELLOW = 6, GREEN = 2, DEFAULT = 14 };

class TTools
{
public:
	TTools();

	void Initialize();
	void Debug(std::string Message, ErrorRank Colour);
	void Log(std::string Message);
	void Log(std::string Message, int X);
	void Log(std::string Message, int X, int Y);
	void Log(std::string Message, int X, int Y, int Z);
	void LogNoPause(std::string Message);
	void ReadFile(std::string FilePath);

	std::string GetOptions(std::string index);
private:
	//For reading files
	std::fstream m_OptionsFile;
	std::string m_OptionLine;

	//Place to store data from file
	std::map<std::string, std::string> m_SettupOptions;

};

