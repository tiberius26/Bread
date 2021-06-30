#pragma once
#include "TTools.h"
#include "TCPManager.h"
#include <thread>
#include <Windows.h>
class Chatting
{
public:
	Chatting();
	
	void ChatLoop(TCPManager& ClientSide);
	void CloseChat();

private:
	//Strings:
	std::string m_SentMessage;
	std::string m_RecievedMessage;

	//Threads:
	std::thread m_ListeningThread;
	std::thread m_SendingThread;

	//Pointers:
	TTools* m_Tools;
	TCPManager* m_ClientLocal;

	//Private methods:
	void Receive();
	void Send();
};

