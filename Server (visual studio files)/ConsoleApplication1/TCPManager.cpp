#include "TCPManager.h"

//Initializing SDL and tools
bool TCPManager::Initialize(const char* IP, int port)
{
	m_Tools = std::make_shared<TTools>();
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		m_Tools->Debug("SDL could not initialize", RED);
		return false;
	}
	if (SDLNet_Init() == -1)
	{
		m_Tools->Debug("SDLNet could not initialize", RED);
		return false;
	}
	if (SDLNet_ResolveHost(&m_IP, IP, port) == -1) //null because this is the server
	{
		m_Tools->Debug("Error creating a server", RED);
		return false;
	}
	return true;
}

//Opening the listening socket
bool TCPManager::OpenSocket()
{
	m_ListenSocket = SDLNet_TCP_Open(&m_IP);//nullptr because this is the server

	if (!m_ListenSocket) {
		m_Tools->Debug("Error opening socket for connection", RED);
		return false;
	}
	return true;
}

//Listening for users
void TCPManager::ListenSocket()
{
	std::cout << "Server waiting for connection from clients" << std::endl;
	while (m_AmListeningForUsers)//to keep the thread running
	{
		while (!m_ClientSocket)
		{
			if (CheckEnd() == "end") //to make sure the code stops running when "end" sent 
			{
				m_AmListeningForUsers = false;
				break; // breaks out of the while loop
			}
			m_UserID = "User" + std::to_string(m_UserCount+1); 
			m_ClientSocket = SDLNet_TCP_Accept(m_ListenSocket);
			m_ClientList[m_UserID] = m_ClientSocket;//adds the user to a user list
		}
		m_UserCount++; //to ensure taht each user is different
		m_Tools->LogNoPause("Client connected!");
		std::string IDMessage = "User" + std::to_string(m_UserCount);
		Send(IDMessage, m_UserID);
		m_ClientSocket = nullptr;
		if (m_UserCount > 1) { m_AmListeningForUsers = false; }
	}
	SDLNet_TCP_Close(m_ListenSocket);//closing the listening socket when the app needs to shut down
}

//Sending messages
bool TCPManager::Send(const std::string& message, std::string ToWho)
{
	if (m_ClientList.empty())//checks if there are users
	{
		return false;
	}
	else 
	{
		if (message != "end") 
		{
			std::string SendingMessage = ToWho + "="+ message;
			m_MessageLength = SendingMessage.length() + 1; //to determine the size of the package
			if (SDLNet_TCP_Send(m_ClientList[ToWho], SendingMessage.c_str(), m_MessageLength) < m_MessageLength) //is the retun value is < length of message it failled/ there's an error
			{
				m_Tools->Debug("Error sending message to client", YELLOW);
				return false;
			}
		}
		else 
		{
			m_MessageLength = message.length() + 1; //to determine the size of the package
			if (SDLNet_TCP_Send(m_ClientList[ToWho], message.c_str(), m_MessageLength) < m_MessageLength) //is the retun value is < length of message it failled/ there's an error
			{
				m_Tools->Debug("Error sending message to client", YELLOW);
				return false;
			}
		}
		if (message == "end") { m_CheckMessage = message; } // to ensure that listening stops
	}
	return true;
}

//Receiving messages
bool TCPManager::Receive(std::string& message, std::string ToWho)
{
	if (m_ClientList.empty())//checks if there are users
	{
		return false;
	}
	else
	{
		char RecievedMessage[2000] = { '\0' };
		if (SDLNet_TCP_Recv(m_ClientList[ToWho], RecievedMessage, C_BUFFER) <= 0) //is the retun value is < length of message it failled/ there's an error
		{
			m_Tools->Debug("Error recieveing message", YELLOW);
			return false;
		}
		else { message = RecievedMessage;} //stores the recieved message before it is deleted
		if (message == "end") { m_CheckMessage = message; }// to ensure that listening stops

		//return true;
	}
	return true;
}

//Closign client sockets
void TCPManager::CloseSocket()
{
	SDLNet_TCP_Close(m_ClientSocket);
	for (int i = 0; i < m_UserCount; i++) //looping theouch the clients
	{
		m_ClosingSocketID = "User" + std::to_string(i);
		SDLNet_TCP_Close(m_ClientList[m_ClosingSocketID]);
	}
}

//Closes SDL and deletes TTools pointer
void TCPManager::ShutDown()
{
	SDLNet_Quit();
	SDL_Quit();
}

//Constructer
TCPManager::TCPManager()
{
	m_MessageLength = 0;
	m_ListenSocket = nullptr;
	m_ClientSocket = nullptr;
	m_Tools = nullptr;
	m_IP = { 0,0 };
	m_UserCount = 0;
	m_AmListeningForUsers = true;
}

//Deconstructer
TCPManager::~TCPManager()
{
}

