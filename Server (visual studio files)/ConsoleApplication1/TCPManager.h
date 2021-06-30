#pragma once
#include <string>
#include <SDL.h>
#include <SDL_net.h>
#include "TTools.h"
#include <map>
#include <memory>

class TCPManager
{
public:
	TCPManager();
	~TCPManager();
	//Bool because it can fail
	bool Initialize(const char* IP, int port);
	bool OpenSocket();
	bool Send(const std::string& message, std::string ToWho);
	bool Receive(std::string& message, std::string ToWho);

	void ListenSocket();
	void CloseSocket();
	void ShutDown();
	void TurnListeningOff() { m_AmListeningForUsers = false;} //one line set

	int GetUserCount() { return m_UserCount; } //one line get
private:
	//IP 
	IPaddress m_IP;

	//Sockets
	TCPsocket m_ListenSocket;
	TCPsocket m_ClientSocket;

	//Strings:
	std::string m_UserID;
	std::string m_ClosingSocketID;
	std::string m_CheckMessage;

	//Ints:
	int m_MessageLength;
	int m_UserCount;
	const int C_PORT = 1234;
	const int C_BUFFER = 2000;


	//Pointers:
	std::shared_ptr<TTools> m_Tools;

	//Maps:
	std::map<std::string, TCPsocket> m_ClientList;

	//Booleans:
	bool m_AmListeningForUsers;
	
	//Private methods:
	std::string CheckEnd() { return m_CheckMessage; };


};

