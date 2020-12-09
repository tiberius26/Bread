#include "EndState.h"
#include "Input.h"
#include "MenuState.h"
#include "PlayState.h"
#include <iostream>
//======================================================================================================
PlayState::PlayState()
{
	m_image = nullptr;
}
//======================================================================================================
bool PlayState::OnEnter()
{
	m_image = new Background("Assets/Images/Play_1280x720.png", "Assets/Music/Aircord.ogg");	
	m_MyManager = new TCPManager;
	m_MyManager->Initialize("127.0.0.1", 1234);
	m_MyManager->OpenSocket();
	m_RecievingThread = std::thread(&PlayState::ServerMessageRecieving, this); // look at recieve in chatapp, the one in chatting takes no string, the one in TCP does
	m_RecievingThread.detach();
	return true;
}
//======================================================================================================
void PlayState::ServerMessageRecieving() 
{
	while (m_ServerMessage != "end") 
	{
		m_MyManager->Receive(m_ServerMessage);
	}
}
//======================================================================================================
GameState* PlayState::Update(int deltaTime)
{
	m_image->PlayMusic();
	//m_MyManager->Send("Test");
	if (Input::Instance()->IsKeyPressed(HM_KEY_M))
	{
		m_image->StopMusic();
		return new MenuState;
	}

	if (Input::Instance()->IsKeyPressed(HM_KEY_Q))
	{
		m_image->StopMusic();
		m_MyManager->Send("end");
		return new EndState;
	}
	
	if (m_ServerMessage == "end")
	{
		//std::cout << "Hit"<<std::endl;
		m_image->StopMusic();
		return new EndState;
	}

	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		if ((*it)->IsActive())
		{
			(*it)->Update(deltaTime);
		}
	}

	return this;
}
//======================================================================================================
bool PlayState::Draw()
{
	m_image->Draw();

	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		if ((*it)->IsActive() && (*it)->IsVisible())
		{
			(*it)->Draw();
		}
	}
	
	return true;
}
//======================================================================================================
void PlayState::OnExit()
{
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		delete (*it);
	}
	m_gameObjects.clear();
	delete m_image;
	m_MyManager->CloseSocket();
	m_MyManager->ShutDown();
}