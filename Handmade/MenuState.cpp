#include "MenuState.h"
#include "PlayState.h"

//======================================================================================================
MenuState::MenuState()
{
    m_menu = nullptr;
    m_image = nullptr;
}
//======================================================================================================
bool MenuState::OnEnter()
{
    m_menu = new MainMenu;
    m_image = new Background("Assets/Images/Bread.jpg", "Assets/Music/Journeys.ogg");
    m_menu->SetMenuText("Play game");
    m_menu->SetMenuText("Quit game");
    return true;
}
//======================================================================================================
GameState* MenuState::Update(int deltaTime)
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
bool MenuState::Draw()
{
    m_image->Draw();
    m_menu->Draw();
    return true;
}
//======================================================================================================
void MenuState::OnExit()
{
    delete m_image;
    delete m_menu;
}