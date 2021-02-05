#include "FileReader.h"

void FileReader::ReadFile(std::string FilePath)
{
	m_OptionsFile.open(FilePath);
	while (!m_OptionsFile.eof())//loads te stats and stores them in a map
	{
		std::getline(m_OptionsFile, m_OptionLine);
		auto LineSplitter = m_OptionLine.find("=");
		std::string Id = m_OptionLine.substr(0, LineSplitter);//where to start and how much to go
		m_SettupOptions[Id] = m_OptionLine.substr(LineSplitter + 1, m_OptionLine.size() - (LineSplitter + 1));
	}
	m_OptionsFile.close();

}

std::string FileReader::GetOptions(std::string index)
{
	return m_SettupOptions[index];
}
