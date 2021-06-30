#include "TTools.h"

//Constructer
TTools::TTools()
{
	Initialize();
}

//For errors
void TTools::Debug(std::string Message, ErrorRank Colour) // Enum class causes errors so it was ommited
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colour);

	std::cout << "Error: " << Message << std::endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
	system("Pause");
}

//For messages
void TTools::Log(std::string Message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);

	std::cout << "Log Message: " << Message << std::endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
	system("Pause");
}

//For messages with 1 value
void TTools::Log(std::string Message, int X)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);

	std::cout << "Log Message: " << Message << "\n Value: " << X << std::endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
	system("Pause");
}

//For messages with 2 values
void TTools::Log(std::string Message, int X, int Y)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);

	std::cout << "Log Message: " << Message << "\n Values: " << X << "," << Y << std::endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
	system("Pause");
}

//For messages with 3 values
void TTools::Log(std::string Message, int X, int Y, int Z)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);

	std::cout << "Log Message: " << Message << "\n Values: " << X << "," << Y << "," << Z << std::endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
	system("Pause");
}

//Setting default colour console for the app
void TTools::Initialize()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
}

//For messages with no pause
void TTools::LogNoPause(std::string Message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);

	std::cout << "Log Message: " << Message << std::endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
}

//Reading from a file
void TTools::ReadFile(std::string FilePath)
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

//Getting information stored in map
std::string TTools::GetOptions(std::string index)
{
	return m_SettupOptions[index];
}
