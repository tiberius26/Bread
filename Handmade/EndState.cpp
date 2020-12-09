#include "EndState.h"
#include "PlayState.h"

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
	m_image = new Background("Assets/Images/End_1280x720.png", "Assets/Music/Glory.ogg");
	m_menu->SetMenuText("Play again");
	m_menu->SetMenuText("Quit game");
	return true;
}
//======================================================================================================
GameState* EndState::Update(int deltaTime)
{
	m_image->PlayMusic();
	m_menu->Update(deltaTime);

	if (m_menu->GetMenuOption() == static_cast<int>(MenuOption::PLAY))
	{
		m_image->StopMusic();
		return new PlayState;
	}

	if (m_menu->GetMenuOption() == static_cast<int>(MenuOption::QUIT))
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