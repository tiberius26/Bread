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
	m_AmListeningForKeys = true;
	m_IsSoundMuted = false;
	m_GameEnded = false;
	m_AmDrawingRound = true; // bool to see if the round loss/win image is drawn
	m_DrawStart = time(0);
}
//======================================================================================================
bool PlayState::OnEnter()
{
	m_Player = std::make_shared<Player>();
	m_Player->Initialize();
	m_image = std::make_shared<Background>("Assets/Images/BG.png", "Assets/Music/Aircord.ogg");
	m_MyManager = std::make_shared <TCPManager>();
	m_MyManager->Initialize("127.0.0.1", 1234);
	m_MyManager->OpenSocket();
	m_RecievingThread = std::thread(&PlayState::ServerMessageRecieving, this); // look at recieve in chatapp, the one in chatting takes no string, the one in TCP does
	m_RecievingThread.detach();

	m_RoundWin = std::make_shared<Sprite>();
	m_RoundWin->Load("Assets/Images/Won.png", "Won");
	m_RoundWin->SetImage("Won");
	m_RoundWin->Disable();
	m_RoundWin->SetSpriteDimension(200, 50);
	m_RoundWin->SetImageDimension(1, 1, 200, 50);
	m_RoundWin->SetImageCel(1, 1);

	m_RoundLoss = std::make_shared<Sprite>();
	m_RoundLoss->Load("Assets/Images/Lost.png", "Lost");
	m_RoundLoss->SetImage("Lost");
	m_RoundLoss->Disable();
	m_RoundLoss->SetSpriteDimension(200, 50);
	m_RoundLoss->SetImageDimension(1, 1, 200, 50);
	m_RoundLoss->SetImageCel(1, 1);

	m_MyTurn = std::make_shared<Sprite>();
	m_MyTurn->Load("Assets/Images/YourTurn.png", "YourTurn");
	m_MyTurn->SetImage("YourTurn");
	m_MyTurn->Enable();
	m_MyTurn->SetSpriteDimension(200, 50);
	m_MyTurn->SetImageDimension(1, 1, 200, 50);
	m_MyTurn->SetImageCel(1, 1);

	m_EnemyTurn = std::make_shared<Sprite>();
	m_EnemyTurn->Load("Assets/Images/EnemyTurn.png", "EnemyTurn");
	m_EnemyTurn->SetImage("EnemyTurn");
	m_EnemyTurn->Enable();
	m_EnemyTurn->SetSpriteDimension(200, 50);
	m_EnemyTurn->SetImageDimension(1, 1, 200, 50);
	m_EnemyTurn->SetImageCel(1, 1);
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
void PlayState::EnableWin()
{
	m_RoundWin->Enable();
	Sleep(1000);
	m_DrawStart = time(0);
	m_AmListeningForKeys = true;
}
void PlayState::EnableLoss()
{
	m_RoundLoss->Enable();
	Sleep(1000);
	m_DrawStart = time(0);
	m_AmListeningForKeys = true;
}
//======================================================================================================
GameState* PlayState::Update(int deltaTime)
{
	m_Player->UpdateCounter();
	m_Player->UpdateBreadCount();
	if (m_ServerMessage == "end")
	{
		m_MyManager->Send("end");
		m_GameEnded = true;
		//std::cout << "Hit"<<std::endl;
		m_image->StopMusic();
		return new EndState;
	}
	if (!m_IsSoundMuted) 
	{
		m_image->PlayMusic();
	}

	//m_MyManager->Send("Test");
	if (Input::Instance()->IsKeyPressed(HM_KEY_M))
	{
		m_image->StopMusic();
		m_IsSoundMuted = true;
	}
	if (Input::Instance()->IsKeyPressed(HM_KEY_BACKSPACE))
	{
		m_image->StopMusic();
		return new MenuState;
	}
	if (Input::Instance()->IsKeyPressed(HM_KEY_Q))
	{
		m_GameEnded = true;
		m_image->StopMusic();
		m_MyManager->Send("end");
		m_ServerMessage = "end";
		//return new EndState;
	}



	if (m_Player->CheckWin())
	{
		m_GameEnded = true;
		SetWin(true);
		m_image->StopMusic();
		m_MyManager->Send("end");
		m_ServerMessage = "end";
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
		
		CheckInput();

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

					m_ServerMessage.clear();

					EnableWin();

				}
				else if(m_Player->GetGuess() != m_Player->GetSecretNumber())
				{
					m_Player->IncreaseEnemyBread(m_Player->GetSecretNumber());
					m_Player->ResetTurn();
					m_MyTurnToGuess = true;
					m_SecretNumberRecieved = false;

					m_ServerMessage.clear();

					EnableLoss();

				}
			}
		}
		else if (!m_MyTurnToGuess)
		{
			if (m_Player->CheckGuessPlaced() && m_SecretNumberRecieved)
			{
				if (m_Player->GetGuess() == m_Player->GetSecretNumber())
				{
					m_Player->IncreaseEnemyBread(m_Player->GetSecretNumber());
					m_Player->ResetTurn();
					m_MyTurnToGuess = true;
					m_SecretNumberRecieved = false;

					m_ServerMessage.clear();

					EnableLoss();
				}
				else if (m_Player->GetGuess() != m_Player->GetSecretNumber())
				{
					m_Player->IncreaseBread(m_Player->GetGuess());
					m_Player->ResetTurn();
					m_MyTurnToGuess = false;
					m_SecretNumberRecieved = false;

					m_ServerMessage.clear();

					EnableWin();
				}
			}
		}
	}
	if (m_Message == "end")
	{
		m_ServerMessage = "end";
		m_MyManager->Send(m_ServerMessage);
		//std::cout << "Hit"<<std::endl;
		m_image->StopMusic();
		return new EndState;
	}
	if (m_GameEnded) 
	{
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
	m_Player->Draw();
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		if ((*it)->IsActive() && (*it)->IsVisible())
		{
			(*it)->Draw();
		}
	}
	m_RoundLoss->Draw(540, 335, 0.0, Sprite::FlipType::NO_FLIP); // in the middle of the screen, adjusted fro the image's size
	m_RoundWin->Draw(540, 335, 0.0, Sprite::FlipType::NO_FLIP);

	m_SecondsSinceDrawStarted = difftime(time(0), m_DrawStart);
	if (m_SecondsSinceDrawStarted > 2) 
	{ 
		m_RoundLoss->Disable();
		m_RoundWin->Disable();
	}

	if (m_MyTurnToGuess) 
	{
		m_MyTurn->Draw(540, 135, 0.0, Sprite::FlipType::NO_FLIP);
	}
	else if (!m_MyTurnToGuess)
	{
		m_EnemyTurn->Draw(540, 535, 0.0, Sprite::FlipType::NO_FLIP);
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
	m_MyManager->CloseSocket();
	m_MyManager->ShutDown();
}

void PlayState::CheckInput() 
{
	if (Input::Instance()->IsKeyPressed(HM_KEY_1) && m_AmListeningForKeys) 
	{
		m_Player->PressedNumber(1); 
		m_MyManager->Send(m_Player->GetMessageToSend()); 
		m_AmListeningForKeys = false;
	}
	if (Input::Instance()->IsKeyPressed(HM_KEY_2) && m_AmListeningForKeys) 
	{
		m_Player->PressedNumber(2);
		m_MyManager->Send(m_Player->GetMessageToSend()); 
		m_AmListeningForKeys = false;
	}
	if (Input::Instance()->IsKeyPressed(HM_KEY_3) && m_AmListeningForKeys) 
	{
		m_Player->PressedNumber(3);
		m_MyManager->Send(m_Player->GetMessageToSend()); 
		m_AmListeningForKeys = false;
	}
	if (Input::Instance()->IsKeyPressed(HM_KEY_4) && m_AmListeningForKeys) 
	{
		m_Player->PressedNumber(4);
		m_MyManager->Send(m_Player->GetMessageToSend());
		m_AmListeningForKeys = false;
	}
	if (Input::Instance()->IsKeyPressed(HM_KEY_5) && m_AmListeningForKeys) 
	{
		m_Player->PressedNumber(5);
		m_MyManager->Send(m_Player->GetMessageToSend());
		m_AmListeningForKeys = false;
	}
	if (Input::Instance()->IsKeyPressed(HM_KEY_6) && m_AmListeningForKeys) 
	{ 
		m_Player->PressedNumber(6);
		m_MyManager->Send(m_Player->GetMessageToSend());
		m_AmListeningForKeys = false;
	}
	if (Input::Instance()->IsKeyPressed(HM_KEY_7) && m_AmListeningForKeys) 
	{
		m_Player->PressedNumber(7);
		m_MyManager->Send(m_Player->GetMessageToSend()); 
		m_AmListeningForKeys = false;
	}
	if (Input::Instance()->IsKeyPressed(HM_KEY_8) && m_AmListeningForKeys) 
	{ 
		m_Player->PressedNumber(8);
		m_MyManager->Send(m_Player->GetMessageToSend());
		m_AmListeningForKeys = false;
	}
	if (Input::Instance()->IsKeyPressed(HM_KEY_9) && m_AmListeningForKeys) 
	{
		m_Player->PressedNumber(9);
		m_MyManager->Send(m_Player->GetMessageToSend());
		m_AmListeningForKeys = false;
	}
	if (Input::Instance()->IsKeyPressed(HM_KEY_0) && m_AmListeningForKeys) 
	{
		m_Player->PressedNumber(10);
		m_MyManager->Send(m_Player->GetMessageToSend());
		m_AmListeningForKeys = false;
	}
}