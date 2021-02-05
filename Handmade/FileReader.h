#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <map>
class FileReader
{
public:

	void ReadFile(std::string FilePath);
	std::string GetOptions(std::string index);


private:

	std::fstream m_OptionsFile;
	std::string m_OptionLine;
	std::map<std::string, std::string> m_SettupOptions;

};

