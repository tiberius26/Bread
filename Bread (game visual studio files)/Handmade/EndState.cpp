#include "EndState.h"
#include "PlayState.h"
#include "Input.h"
//======================================================================================================
EndState::EndState()
{
	m_menu = nullptr;
	m_image = nullptr;
}
//======================================================================================================
bool EndState::OnEnter()
{
	m_menu = new MainMenu;
	if (GetWin())
	{
		m_image = new Background("Assets/Images/Win.jpg", "Assets/Music/Glory.ogg");
	}
	else 
	{
		m_image = new Background("Assets/Images/GameOver.jpg", "Assets/Music/Sad.ogg");

	}
	m_menu->SetMenuText("Quit game");
	return true;
}
//======================================================================================================
GameState* EndState::Update(int deltaTime)
{
	m_image->PlayMusic();
	m_menu->Update(deltaTime);

	if (m_menu->GetMenuOption() == static_cast<int>(MenuOption::QUIT))
	{
		m_image->StopMusic();
		return nullptr;
	}
	if(Input::Instance()->IsKeyPressed(HM_KEY_RETURN))
	{
		m_image->StopMusic();
		return nullptr;
	}
	return this;
}
//======================================================================================================
bool EndState::Draw()
{
	m_image->Draw();
	m_menu->Draw();
	return true;
}
//======================================================================================================
void EndState::OnExit()
{
	delete m_image;
	delete m_menu;
}