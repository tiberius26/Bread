#include "TCPManager.h"

//Setting SDL and TTools up
bool TCPManager::Initialize(const char* IP, int port) //IP can be found and edited in Data/Options.ini
{
	Debugger = std::make_shared<Debug>();
	if (SDLNet_Init() == -1)
	{
		Debugger->Log("SDLNet could not initialize", Debug::ErrorCode::FAILURE);
			//Log("SDLNet could not initialize", FAILURE);
		return false;
	}
	if (SDLNet_ResolveHost(&m_IP, IP, port) == -1)
	{
		Debugger->Log("Error creating a client", Debug::ErrorCode::FAILURE);
		return false;
	}
	return true;
}

//opening socket for connection to the server
bool TCPManager::OpenSocket()
{
	m_Socket = SDLNet_TCP_Open(&m_IP);
	
	if (!m_Socket) {
		Debugger->Log("Error opening socket for connection", Debug::ErrorCode::FAILURE);
		return false;
	}
	Debugger->Log("Client Created", Debug::ErrorCode::SUCCESS);
	return true;
}

//for setting messages
bool TCPManager::Send(const std::string& message)
{
	m_MessageLength = message.length() + 1;
	if (SDLNet_TCP_Send(m_Socket, message.c_str(), m_MessageLength) < m_MessageLength) //is the retun value is < length of message it failled/ there's an error
	{
		Debugger->Log("Error sending message to client", Debug::ErrorCode::WARNING);
		return false;
	}
	return true;
}

//for receiving messages
bool TCPManager::Receive(std::string& message)
{
	if (message != "end") 
	{
		char RecievedMessage[2000] = { '\0' };
		if (SDLNet_TCP_Recv(m_Socket, RecievedMessage, C_BUFFER) <= 0) //is the retun value is < length of message it failled/ there's an error
		{
			Debugger->Log("Error recieveing message", Debug::ErrorCode::WARNING);
			return false;
		}
		else { message = RecievedMessage; }
	}
	return true;
}

//closing socket
void TCPManager::CloseSocket()
{
	SDLNet_TCP_Close(m_Socket);
}

//shutting SDL and TTools down
void TCPManager::ShutDown()
{
	SDLNet_Quit();
}

//constructer
TCPManager::TCPManager()
{
	m_Socket = nullptr;
	m_MessageLength = 0;
	Debugger = nullptr;
	m_IP = {0,0};
}

//deconstructer
TCPManager::~TCPManager()
{
}

