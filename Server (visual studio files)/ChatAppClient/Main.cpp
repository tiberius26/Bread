#include <string>
#include <iostream>
#include "TTools.h"
#include "TCPManager.h"
#include "Chatting.h"
//Any warnings left are not caused by this app, SDL
//There is an errors that happens ocassionally and I believe that it is from my computer, it takes me to ctree at for .length which is only used twice in the entire programme
//Works without error if run feom the release folder, or when it wants to run

//Tested with 3 users and it worked, the maximum will be determined by computer specs
int main(int argc, char* argv[])
{
	//Display
	std::cout << "========================================" << std::endl;
	std::cout << "=     BSF Communications department    =" << std::endl;
	std::cout << "========================================" << std::endl;

	//Creating pointers to classes
	TTools* Tools = new TTools;
	Chatting* Chat = new Chatting;
	TCPManager ClientSide;

	//Reading options
	Tools->ReadFile("Data/Options.ini");

	//Getting information from the read file
	std::string IP = Tools->GetOptions("Ip");
	std::string Port = Tools->GetOptions("Port");

	//Informing the user of the IP that Will be used
	std::cout << "The connection will be made using the IP:" << IP << std::endl;
	std::cout << "If you want to use another IP, go to Data/Options.ini, then restart the application" << std::endl;
	system("pause");

	//Initializing the user based on te read information
	ClientSide.Initialize(IP.c_str(), std::stoi(Port));

	//Opening the socket for the connection
	ClientSide.OpenSocket();

	//Display
	system("cls");
	std::cout << "========================================" << std::endl;
	std::cout << "=     BSF Communications department    =" << std::endl;
	std::cout << "========================================" << std::endl;
	std::cout << "Send end to close the app, case sensitive" << std::endl;
	std::cout << "========================================" << std::endl;

	//Main loop
	Chat->ChatLoop(ClientSide);

	//Shutting things down
	ClientSide.CloseSocket();
	ClientSide.ShutDown();
	Chat->CloseChat();

	//Deleting pointers
	delete Tools;
	delete Chat;

	//Display final message
	system("cls");
	std::cout << "========================================" << std::endl;
	std::cout << "=     BSF Communications department    =" << std::endl;
	std::cout << "========================================" << std::endl;
	std::cout << "The chat has been closed" << std::endl;
	system("Pause");
	return 0;
}