#include <iostream>
#include "Debug.h"

HANDLE Debug::s_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

//======================================================================================================
void Debug::PauseLog()
{
	system("pause");
}
//======================================================================================================
void Debug::ClearLog()
{
	system("cls");
}
//======================================================================================================
void Debug::Log(float value, const std::string& label)
{
	SetConsoleTextAttribute(s_consoleHandle, static_cast<WORD>(ErrorCode::DEFAULT));

	if (!label.empty())
	{
		std::cout << "[" << label << "] ";
	}

	std::cout << value << std::endl;
}
//======================================================================================================
void Debug::Log(int x, int y, const std::string& label)
{
	Log({ x, y }, label);
}
//======================================================================================================
void Debug::Log(const Vector<int>& point, const std::string& label)
{
	SetConsoleTextAttribute(s_consoleHandle, static_cast<WORD>(ErrorCode::DEFAULT));

	if (!label.empty())
	{
		std::cout << "[" << label << "] ";
	}

	std::cout << "x = " << point.x << ", y = " << point.y << std::endl;
}
//======================================================================================================
void Debug::Log(const std::string& message, ErrorCode errorCode)
{
	SetConsoleTextAttribute(s_consoleHandle, static_cast<WORD>(errorCode));
	std::cout << message << std::endl;
	SetConsoleTextAttribute(s_consoleHandle, static_cast<WORD>(ErrorCode::DEFAULT));
}