#include "EndState.h"
#include "Input.h"
#include "MenuState.h"
#include "PlayState.h"
#include <iostream>
#include "Debug.h"
//======================================================================================================
PlayState::PlayState()
{
	//bool default
	m_AmDrawingRound = true; // bool to see if the round loss/win image is drawn
	m_SecretNumberRecieved = false;
	m_AmListeningForKeys = true;
	m_IsPlayerAssigned = false;
	m_MyTurnToGuess = false;
	m_IsSoundMuted = false;

	//pointers default
	m_MyManager = nullptr;
	m_Player = nullptr;
	m_image = nullptr;

	//timer default
	m_DrawStart = time(0);
}
//======================================================================================================
bool PlayState::OnEnter()
{
	//bool setting
	m_RecievingMessages = true;

	//setting player up
	m_Player = std::make_shared<Player>();
	m_Player->Initialize();


	
	//setting background up
	m_image = std::make_shared<Background>("Assets/Images/BG.png", "Assets/Music/Aircord.ogg");

	//getting IP and port form file
	Reader.ReadFile("Data/Options.ini");
	std::string IP = Reader.GetOptions("Ip");
	std::string Port = Reader.GetOptions("Port");

	//debugging
	Debug TemporaryDebugger;
	TemporaryDebugger.Log("The connection will be made using the IP:" + IP, Debug::ErrorCode::WARNING);
	TemporaryDebugger.Log("If you want to use another IP, go to Data/Options.ini, then restart the application",Debug::ErrorCode::WARNING);

	//setting connection up
	m_MyManager = std::make_shared <TCPManager>();

	m_MyManager->Initialize(IP.c_str(), std::stoi(Port));
	m_MyManager->OpenSocket();

	//thread for recieving messages
	m_RecievingThread = std::thread(&PlayState::ServerMessageRecieving, this); // look at recieve in chatapp, the one in chatting takes no string, the one in TCP does
	m_RecievingThread.detach();

	//round win sprite
	m_RoundWin = std::make_shared<Sprite>();
	m_RoundWin->Load("Assets/Images/Won.png", "Won");
	m_RoundWin->SetImage("Won");
	m_RoundWin->Disable();
	m_RoundWin->SetSpriteDimension(200, 50);
	m_RoundWin->SetImageDimension(1, 1, 200, 50);
	m_RoundWin->SetImageCel(1, 1);

	//round loss sprite
	m_RoundLoss = std::make_shared<Sprite>();
	m_RoundLoss->Load("Assets/Images/Lost.png", "Lost");
	m_RoundLoss->SetImage("Lost");
	m_RoundLoss->Disable();
	m_RoundLoss->SetSpriteDimension(200, 50);
	m_RoundLoss->SetImageDimension(1, 1, 200, 50);
	m_RoundLoss->SetImageCel(1, 1);

	//round turn sprite
	m_MyTurn = std::make_shared<Sprite>();
	m_MyTurn->Load("Assets/Images/YourTurn.png", "YourTurn");
	m_MyTurn->SetImage("YourTurn");
	m_MyTurn->Enable();
	m_MyTurn->SetSpriteDimension(200, 50);
	m_MyTurn->SetImageDimension(1, 1, 200, 50);
	m_MyTurn->SetImageCel(1, 1);

	//round enemy turn sprite
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
	//thread loop
	while (m_RecievingMessages)
	{
		m_MyManager->Receive(m_ServerMessage);
	}
}

bool PlayState::IsNumber(const std::string& StringToCheck) //check to see if a string is actually a number
{
	std::string::const_iterator it = StringToCheck.begin();
	while (it != StringToCheck.end() && std::isdigit(*it)) ++it;
	return !StringToCheck.empty() && it == StringToCheck.end();
}
void PlayState::EnableWin()
{
	//shows a win message and allows for key reading agin
	m_RoundWin->Enable();
	Sleep(1000);
	m_DrawStart = time(0);
	m_AmListeningForKeys = true;
}
void PlayState::EnableLoss()
{
	//shows a lose message and allows for key reading agin
	m_RoundLoss->Enable();
	Sleep(1000);
	m_DrawStart = time(0);
	m_AmListeningForKeys = true;
}
//======================================================================================================
GameState* PlayState::Update(int deltaTime)
{
	//updates counters
	m_Player->UpdateCounter();
	m_Player->UpdateBreadCount();

	//check for xlosure
	if (m_ServerMessage == "end")
	{
		m_RecievingMessages = false;
		m_MyManager->Send("end");
		Sleep(100);
		m_image->StopMusic();
		return new EndState;
	}
	//check if sound is muted
	if (!m_IsSoundMuted) 
	{
		m_image->PlayMusic();
	}

	//game inputs
	if (Input::Instance()->IsKeyPressed(HM_KEY_M)) //mute
	{
		m_image->StopMusic();
		m_IsSoundMuted = true;
	}

	if (Input::Instance()->IsKeyPressed(HM_KEY_Q)) //close app
	{
		m_RecievingMessages = false;
		m_MyManager->Send("end");
		Sleep(100);
		m_MyManager->Send("end");
		m_image->StopMusic();
		return new EndState;
	}

	if (m_Player->CheckWin()) //checks for win
	{
		m_RecievingMessages = false;
		SetWin(true);
		m_MyManager->Send("end");
		Sleep(100);
		m_MyManager->Send("end");
		m_image->StopMusic();
		return new EndState;
	}

	if (!m_RecievingMessages) //check for close safety
	{
		m_image->StopMusic();
		return new EndState;
	}
	//message splitting
	std::string::size_type EqualFind = m_ServerMessage.find("=");
	std::string::size_type CollomFind = m_ServerMessage.find(":");
	std::string WhoItWasSentTo = m_ServerMessage.substr(0, EqualFind);
	if (CollomFind != std::string::npos)//check if : exists in the message
	{
		m_WhoSentIt = m_ServerMessage.substr(EqualFind + 1, IDSIZE); // 5 = length of user1/user2 name
	}

	if (!m_IsPlayerAssigned) //assigne players their IDs
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
	else //sees how the logic needs to be applied for recieved messages
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
		
		CheckInput(); //checks for input

		//turn
		if (m_MyTurnToGuess)
		{
			if (m_Player->CheckGuessPlaced() && m_SecretNumberRecieved) // won round
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
				else if(m_Player->GetGuess() != m_Player->GetSecretNumber()) //lost round
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
				if (m_Player->GetGuess() == m_Player->GetSecretNumber()) //lost round
				{
					m_Player->IncreaseEnemyBread(m_Player->GetSecretNumber());
					m_Player->ResetTurn();
					m_MyTurnToGuess = true;
					m_SecretNumberRecieved = false;

					m_ServerMessage.clear();

					EnableLoss();
				}
				else if (m_Player->GetGuess() != m_Player->GetSecretNumber()) //won round
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
	if (m_Message == "end") // safety check
	{
		m_MyManager->Send("Eend");
		Sleep(100);
		m_MyManager->Send("end");

		m_image->StopMusic();
		return new EndState;
	}






	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++) //updates any other objects
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
	m_image->Draw();//bg
	m_Player->Draw();//player

	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		if ((*it)->IsActive() && (*it)->IsVisible())
		{
			(*it)->Draw();
		}
	}

	//loss/win round
	m_RoundLoss->Draw(540, 335, 0.0, Sprite::FlipType::NO_FLIP); // in the middle of the screen, adjusted fro the image's size
	m_RoundWin->Draw(540, 335, 0.0, Sprite::FlipType::NO_FLIP);

	//timer and check
	m_SecondsSinceDrawStarted = difftime(time(0), m_DrawStart);
	if (m_SecondsSinceDrawStarted > 2) 
	{ 
		m_RoundLoss->Disable();
		m_RoundWin->Disable();
	}

	//draws a sign to tell who turn it is to guess
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
void PlayState::OnExit() //closes everything
{
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		delete (*it);
	}
	m_gameObjects.clear();

	m_MyManager->CloseSocket();
	m_MyManager->ShutDown();
}

void PlayState::CheckInput() //input.  for each key a different message is sent to the other app
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