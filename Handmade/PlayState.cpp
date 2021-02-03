#include "EndState.h"
#include "Input.h"
#include "MenuState.h"
#include "PlayState.h"
#include <iostream>

//======================================================================================================
PlayState::PlayState()
{
	m_MyManager = nullptr;
	m_image = nullptr;
	m_Player = nullptr;
	m_MyTurnToGuess = false;
	m_SecretNumberRecieved = false;
	m_IsPlayerAssigned = false;
}
//======================================================================================================
bool PlayState::OnEnter()
{
	m_Player = new Player;
	m_Player->Initialize();
	m_image = new Background("Assets/Images/BG.png", "Assets/Music/Aircord.ogg");	
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
bool PlayState::IsNumber(const std::string& StringToCheck)
{
	std::string::const_iterator it = StringToCheck.begin();
	while (it != StringToCheck.end() && std::isdigit(*it)) ++it;
	return !StringToCheck.empty() && it == StringToCheck.end();
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

	if (m_Player->CheckWin()) 
	{
		m_image->StopMusic();
		m_MyManager->Send("end");
		return new EndState;
	}


	
	std::string::size_type EqualFind = m_ServerMessage.find("=");
	std::string::size_type CollomFind = m_ServerMessage.find(":");
	std::string WhoItWasSentTo = m_ServerMessage.substr(0, EqualFind);
	if (CollomFind != std::string::npos) 
	{
		m_WhoSentIt = m_ServerMessage.substr(EqualFind + 1, IDSIZE); // 5 = length of user1/user2 name
	}

	if (!m_IsPlayerAssigned)
	{
		m_Message = m_ServerMessage.substr(EqualFind + 1, m_ServerMessage.size() - (EqualFind + 1));
		if (m_Message == "User1")
		{
			m_Player->SetIdentity("User1");
			m_MyTurnToGuess = true;
			m_IsPlayerAssigned = true;
		}
		else if (m_Message == "User2")
		{
			m_Player->SetIdentity("User2"); 
			m_MyTurnToGuess = false;
			m_IsPlayerAssigned = true;
		}
		
	}
	else 
	{
		m_Message = m_ServerMessage.substr(CollomFind + 1, m_ServerMessage.size() - (CollomFind + 1));
		//logic for getting player 1 recieved messages
		if (m_Player->GetIdentity() == "User1" && m_WhoSentIt == "User2")
		{
			if (IsNumber(m_Message))
			{
				m_Player->SetSecretNumber(stoi(m_Message));
				m_SecretNumberRecieved = true;
				m_Player->PlaceEnemyIndicator(1100, 600, true);
			}
		}//logic for getting player 2 recieved messages
		else if (m_Player->GetIdentity() == "User2" && m_WhoSentIt == "User1")
		{
			if (IsNumber(m_Message))
			{
				m_Player->SetSecretNumber(stoi(m_Message));
				m_SecretNumberRecieved = true;
				m_Player->PlaceEnemyIndicator(1100, 600, true);
			}
		}
		if (Input::Instance()->IsKeyPressed(HM_KEY_1)) { m_Player->PressedNumber(1); m_MyManager->Send(m_Player->GetMessageToSend()); }
		if (Input::Instance()->IsKeyPressed(HM_KEY_2)) { m_Player->PressedNumber(2); m_MyManager->Send(m_Player->GetMessageToSend()); }
		if (Input::Instance()->IsKeyPressed(HM_KEY_3)) { m_Player->PressedNumber(3); m_MyManager->Send(m_Player->GetMessageToSend()); }
		if (Input::Instance()->IsKeyPressed(HM_KEY_4)) { m_Player->PressedNumber(4); m_MyManager->Send(m_Player->GetMessageToSend()); }
		if (Input::Instance()->IsKeyPressed(HM_KEY_5)) { m_Player->PressedNumber(5); m_MyManager->Send(m_Player->GetMessageToSend()); }
		if (Input::Instance()->IsKeyPressed(HM_KEY_6)) { m_Player->PressedNumber(6); m_MyManager->Send(m_Player->GetMessageToSend()); }
		if (Input::Instance()->IsKeyPressed(HM_KEY_7)) { m_Player->PressedNumber(7); m_MyManager->Send(m_Player->GetMessageToSend()); }
		if (Input::Instance()->IsKeyPressed(HM_KEY_8)) { m_Player->PressedNumber(8); m_MyManager->Send(m_Player->GetMessageToSend()); }
		if (Input::Instance()->IsKeyPressed(HM_KEY_9)) { m_Player->PressedNumber(9); m_MyManager->Send(m_Player->GetMessageToSend()); }
		if (Input::Instance()->IsKeyPressed(HM_KEY_0)) { m_Player->PressedNumber(0); m_MyManager->Send(m_Player->GetMessageToSend()); }
		//turn
		if (m_MyTurnToGuess)
		{
			if (m_Player->CheckGuessPlaced() && m_SecretNumberRecieved)
			{
				if (m_Player->GetGuess() == m_Player->GetSecretNumber())
				{
					m_Player->IncreaseBread(m_Player->GetSecretNumber());
					m_Player->ResetTurn();
					m_MyTurnToGuess = false;
					m_SecretNumberRecieved = false;
				}
				else
				{
					m_Player->IncreaseEnemyBread(m_Player->GetSecretNumber());
					m_Player->ResetTurn();
					m_MyTurnToGuess = false;
					m_SecretNumberRecieved = false;
				}
			}
		}
		else if (!m_MyTurnToGuess)
		{
			if (m_Player->CheckGuessPlaced() && m_SecretNumberRecieved)
			{
				if (m_Player->GetGuess() == m_Player->GetSecretNumber())
				{
					m_Player->ResetTurn();
					m_Player->IncreaseEnemyBread(m_Player->GetSecretNumber());
					m_MyTurnToGuess = true;
					m_SecretNumberRecieved = false;
				}
				else
				{
					m_Player->IncreaseBread(m_Player->GetSecretNumber());
					m_Player->ResetTurn();
					m_MyTurnToGuess = false;
					m_SecretNumberRecieved = false;
				}
			}
		}
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
	m_Player->Draw();
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
	m_MyManager->Send("end");
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		delete (*it);
	}
	if (m_Player->CheckWin()) { m_Win = true; }
	m_gameObjects.clear();
	delete m_image;
	m_MyManager->CloseSocket();
	m_MyManager->ShutDown();
	delete m_Player;
	delete m_MyManager;
}