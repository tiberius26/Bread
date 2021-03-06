#ifndef DEBUG_H
#define DEBUG_H

/*==============================================================================================#
|                                                                                               |
| Handmade Lite is an educational Visual Studio framework, written by Karsten Vermeulen,        |
| designed to assist with educating other fellow programmers, programming students, and anyone  |
| else wishing to learn about game development, C++, and object-oriented programming. This      |
| project is not open source and therefore has no license affiliated with it. Please be         |
| respectful and do not distribute the engine or its code as your work. You may alter or tweak  |
| the engine code as a learning experience and you may also 'borrow' code and functionality     |
| and use it to create your own game engine. Finally, please do use Handmade Lite to develop    |
| a game.                                                                                       |
|                                                                                               |
| Designed to teach. Made from scratch. Built by hand.							                |
|	                                                                                            |
#===============================================================================================#
|                                                                                               |
| If you like Handmade Lite and wish to show your support, if you have any questions about      |
| the project, or if you just want to reach out, please do get in touch:                        |
|																						        |
| Facebook | https://www.facebook.com/Handmade-Lite-107188757745777								|
| Twitter  | https://twitter.com/KarstensCorner													|
| GitHub   | https://github.com/djkarstenv/Handmade-Lite									    |
|                                                                                               |
#===============================================================================================#
| 'Debug' source files last updated on 10 August 2020						                    |
#==============================================================================================*/

#include <string>
#include <Windows.h>
#include "Vector.h"

class Debug
{

public:

	enum class ErrorCode { FAILURE = 4, WARNING = 6, SUCCESS = 10, DEFAULT = 15 };

public:

	static void PauseLog();
	static void ClearLog();
	static void Log(float value, const std::string& label = "");
	static void Log(int x, int y, const std::string& label = "");
	static void Log(const Vector<int>& point, const std::string& label = "");
	static void Log(const std::string& message, ErrorCode errorCode = ErrorCode::DEFAULT);

private:

	static HANDLE s_consoleHandle;

};

#endif