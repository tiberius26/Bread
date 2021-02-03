#ifndef PLAY_STATE_H
#define PLAY_STATE_H

/*==============================================================================================#
|                                                                                               |
| Handmade Lite is an educational Visual Studio framework, written by Karsten Vermeulen,        |
| designed to assist with educating other fellow programmers, programming students, and anyone  |
| else wishing to learn about game development, C++, and object-oriented programming. This      |
| project is not open source and therefore has no license affiliated with it. Please be         |
| respectful and do not distribute the engine or its code as your work. You may alter or tweak  |
| the engine code as a learning experience and you may also 'borrow' code and functionality     |
| and use it to create your own game engine. Finally, please do use Handmade Lite to develop    |
| a game.                                                                                       |
|                                                                                               |
| Designed to teach. Made from scratch. Built by hand.							                |
|	                                                                                            |
#===============================================================================================#
|                                                                                               |
| If you like Handmade Lite and wish to show your support, if you have any questions about      |
| the project, or if you just want to reach out, please do get in touch:                        |
|																						        |
| Facebook | https://www.facebook.com/Handmade-Lite-107188757745777								|
| Twitter  | https://twitter.com/KarstensCorner													|
| GitHub   | https://github.com/djkarstenv/Handmade-Lite									    |
|                                                                                               |
#===============================================================================================#
| 'PlayState' source files last updated on 11 August 2020						                |
#==============================================================================================*/

#include <vector>
#include "Background.h"
#include "GameObject.h"
#include "GameState.h"
#include "TCPManager.h"
#include <thread>
#include "Player.h"
class PlayState : public GameState
{

public:

	PlayState();
	virtual ~PlayState() {}

public:

	virtual bool OnEnter();
	virtual GameState* Update(int deltaTime);
	virtual bool Draw();
	virtual void OnExit();

private:

	void ServerMessageRecieving();
	Background* m_image;
	std::vector<GameObject*> m_gameObjects;
	TCPManager* m_MyManager;
	std::string m_ServerMessage;
	std::thread m_RecievingThread;

	Player* m_Player = nullptr;
	bool m_MyTurnToGuess = false;

	bool IsNumber(const std::string& StringToCheck);
	bool m_SecretNumberRecieved;
	std::string m_Message;
	std::string m_WhoSentIt;
	bool m_IsPlayerAssigned;
	const int IDSIZE = 5;
};

#endif