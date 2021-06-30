#include "TTools.h"

TTools::TTools()
{
	Initialize(); //initializing variables when creating a TTools object
}

//For errors
void TTools::Debug(std::string Message, ErrorRank Colour) // using Enum class breaks parts of the code, enum works just fine
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colour); //change colour based on error

	std::cout << "Error: " << Message << std::endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT); //change back to default
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

//ChanginG the console windown form the very begining
void TTools::Initialize()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
}

//Message without pausing
void TTools::LogNoPause(std::string Message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);

	std::cout << "Log Message: " << Message << std::endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
}

//Reading a file and storing it's contents in a map
void TTools::ReadFile(std::string FilePath)
{
	m_OptionsFile.open(FilePath);
	while (!m_OptionsFile.eof())//loads the variables and stores them in a map
	{
		std::getline(m_OptionsFile, m_OptionLine);
		auto LineSplitter = m_OptionLine.find("=");
		std::string Id = m_OptionLine.substr(0, LineSplitter);
		m_SettupOptions[Id] = m_OptionLine.substr(LineSplitter + 1, m_OptionLine.size() - (LineSplitter + 1));
	}
	m_OptionsFile.close();//closing the file

}

//Getting information extracted from the file
std::string TTools::GetOptions(std::string index)
{
	return m_SettupOptions[index];
}