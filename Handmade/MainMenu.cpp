#include "Input.h"
#include "MainMenu.h"
#include "Screen.h"
#include "Vector.h"

//======================================================================================================
MainMenu::MainMenu()
{
	m_menuOptionActive = 0;
	m_menuOptionChoice = -1;
	Text::Load("Assets/Fonts/Quikhand.ttf", "Menu_Font", Text::FontSize::SMALL);
}
//======================================================================================================
MainMenu::~MainMenu()
{
	Text::Unload("Menu_Font");
}
//======================================================================================================
int MainMenu::GetMenuOption() const
{
	return m_menuOptionChoice;
}
//======================================================================================================
void MainMenu::SetMenuText(const std::string& text)
{
	Text menuText;
	menuText.SetFont("Menu_Font");
	menuText.SetColor(255, 255, 255);
	menuText.SetSize(text.size() * MENU_TEXT_CHAR_W, MENU_TEXT_CHAR_H);
	menuText.SetText(text);
	m_menuText.push_back(menuText);
}
//======================================================================================================
void MainMenu::Update(int deltaTime)
{
	static bool isKeyPressed = false;

	if (Input::Instance()->IsKeyPressed(HM_KEY_DOWN) && !isKeyPressed)
	{
		if (m_menuOptionActive == m_menuText.size() - 1)
		{
			m_menuOptionActive = 0;
		}
		
		else
		{
			m_menuOptionActive++;
		}
	}

	if (Input::Instance()->IsKeyPressed(HM_KEY_UP) && !isKeyPressed)
	{
		if (m_menuOptionActive == 0)
		{
			m_menuOptionActive = m_menuText.size() - 1;
		}

		else
		{
			m_menuOptionActive--;
		}
	}

	if (Input::Instance()->IsKeyPressed(HM_KEY_RETURN))
	{
		m_menuOptionChoice = m_menuOptionActive;
	}

	isKeyPressed = Input::Instance()->IsKeyPressed();

	for (size_t i = 0; i < m_menuText.size(); i++)
	{
		m_menuText[i].SetColor(255, 255, 255);
	}

	m_menuText[m_menuOptionActive].SetColor(240, 0, 0);
}
//======================================================================================================
bool MainMenu::Draw()
{
	Vector<int> resolution = Screen::Instance()->GetResolution();

	int posY = static_cast<int>((resolution.y - resolution.y / 4) - 
		                        (m_menuText.size() / 2 * MENU_TEXT_CHAR_H));

	for (size_t i = 0; i < m_menuText.size(); i++)
	{
		m_menuText[i].Draw(static_cast<int>((resolution.x / 2) - m_menuText[i].GetSize().x / 2),  //x
			               static_cast<int>(posY + i * MENU_TEXT_CHAR_H));                        //y
	}

	return true;
}
//======================================================================================================
void MainMenu::Reset()
{
	m_menuOptionActive = 0;
	m_menuOptionChoice = -1;
}