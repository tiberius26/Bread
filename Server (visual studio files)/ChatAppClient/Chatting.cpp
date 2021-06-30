#include "Chatting.h"

//Created and runs the needed threads
void Chatting::ChatLoop(TCPManager& ClientSide)
{
	m_ClientLocal = &ClientSide;
	m_SendingThread = std::thread(&Chatting::Send, this);
	m_ListeningThread = std::thread(&Chatting::Receive, this);
	m_SendingThread.detach();
	m_ListeningThread.join();
}

//Receiving messages
void Chatting::Receive()
{
	while (m_RecievedMessage != "end")
	{
		if (m_ClientLocal->Receive(m_RecievedMessage))
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); //Light Blue

			std::cout << std::endl << m_RecievedMessage << std::endl;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //Default colour
		}
		else { m_Tools->Debug("Can't recieve message", RED); }
	}
}

//Sending Messages
void Chatting::Send()
{
	while (m_RecievedMessage != "end")
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);// White

		std::cout << "> "; //To inform the user that they can type messages
		std::getline(std::cin, m_SentMessage); //gets the entire typed line

		if (!m_ClientLocal->Send(m_SentMessage))
		{
			m_Tools->Debug("Can't send message", RED);
		}
		if (m_SentMessage != "end") { m_SentMessage.clear(); }

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //Default colour
	}
}

//Constructer
Chatting::Chatting()
{
	m_Tools = new TTools;
	m_ClientLocal = nullptr;
}

//Deconstructer
void Chatting::CloseChat()
{
	delete m_Tools;
	m_ClientLocal = nullptr;
}
