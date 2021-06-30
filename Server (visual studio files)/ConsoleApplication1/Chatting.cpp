#include "Chatting.h"

//Chatting loop
void Chatting::ChatLoop(std::shared_ptr<TCPManager> ClientSide)
{
	m_ServerLocal = ClientSide; //gets the adress of the TCP manager
	for (int i = 0; i < ClientSide->GetUserCount(); i++)
	{
		std::string User = "User" + std::to_string(i+1);
		if (m_SendingThreads.find(User) == m_SendingThreads.end() && m_ListeningThreads.find(User) == m_ListeningThreads.end())  //creates a receiving and sending thread for each user
		{
			m_SendingThreads[User] = std::thread(&Chatting::Send, this, User);
			m_ListeningThreads[User] = std::thread(&Chatting::Receive, this, User);

			m_SendingThreads[User].detach();
			m_ListeningThreads[User].detach();
		}
	}
}

//Gets messages from users
void Chatting::Receive(std::string Client)
{
	while (m_SendMessage != "end")
	{
		if (m_ServerLocal->Receive(m_RecievedMessage, Client))
		{
			m_SendMessage = m_RecievedMessage;
			m_RecievedMessage.clear();
			m_ChatLog += m_SendMessage + "\n"; //adds each sent message to a string
		}
		if (m_SendMessage == "end") { break; }
	}
	m_ServerLocal->Send("end", "User1");
	m_ServerLocal->Send("end", "User2");
	m_IsChatOver = true; //To stop things
	m_ChatLog += "################### \n";
	m_ChatLog += "End Of Conversation";
}

//Sends messages to users
void Chatting::Send(std::string Client)
{
	while (m_SendMessage != "end")
	{
		if (m_SendMessage != "")
		{
			if (!m_ServerLocal->Send(m_SendMessage, Client))
			{
				m_Tools->Debug("Can't relay message", RED);
			}
			if (m_RecievedMessage != "end") { m_SendMessage.clear(); }
		}
		if (m_SendMessage == "end") { break; }
	}
	m_ServerLocal->Send("end", Client); //this sends end to both users to close their apps
}

//Constructer
Chatting::Chatting()
{
	m_Tools = std::make_shared<TTools>();

	m_ServerLocal = nullptr;
	m_IsChatOver = false;
}

//Delets the pointer
void Chatting::CloseChat()
{
}

//Saves the log to a file and updates the settings
void Chatting::SaveLog(int LogCount)
{
	int LogCountLocal = LogCount;
	char SaveLocation[50];
	sprintf_s(SaveLocation, "Data/Logs/Log%d.ini", LogCountLocal);

	bool NoDublicateLog = false;

	while (NoDublicateLog == false)//makes sure that the log is unique
	{
		std::fstream CheckFile(SaveLocation);
		if (!CheckFile)
		{
			NoDublicateLog = true;
		}
		else
		{
			LogCountLocal++;
			sprintf_s(SaveLocation, "Data/Logs/Log%d.ini", LogCountLocal);
		}

	}
	std::ofstream Log(SaveLocation);//saves Log:

	Log << m_ChatLog;
	Log.close();

	std::string OprionsPath = "Data/ServerOptions.ini";
	std::ofstream Options(OprionsPath);//saves Server Options:

	Options << "Port=1234" << std::endl;
	Options << "LogCount=" << LogCountLocal << std::endl;
	Options.close();
}