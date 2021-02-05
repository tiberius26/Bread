#include "Player.h"

void Player::IncreaseBread(int index)
{
    if (index < 12 && index > 0) 
    {
        m_BreadArray[index - 1]->IncrementCounter();
    }
}

void Player::IncreaseEnemyBread(int index)
{
    if (index < 12 && index > 0)
    {
        m_EnemyBreadArray[index - 1]->IncrementCounter();
    }
}

void Player::Draw()
{
    for (int i = 0; i < 10; i++)
    {
        m_EnemyBreadArray[i]->Draw();
        m_BreadArray[i]->Draw();
    }

    m_EnemyIndicator->Draw(m_EnemyIndicatorX, m_EnemyIndicatorY, 0.0, Sprite::FlipType::NO_FLIP);
    m_Indicator->Draw(m_IndicatorX, m_IndicatorY, 0.0, Sprite::FlipType::NO_FLIP);
}

void Player::UpdateCounter()
{
    for (int i=0; i < 10; i++)
    {
        if (m_BreadArray[i]->GetMark()) { m_BreadTracker[i] = true; }
    }
}

void Player::UpdateBreadCount()
{
    m_BreadCount = 0;
    for (int i = 0; i < 10; i++) 
    {
        if (m_BreadTracker[i]) { m_BreadCount++; }
    }
    if (m_BreadCount > 8) { m_HaveWon = true; }
}

bool Player::CheckWin()
{
    if (m_HaveWon) { return true; }
    return false;
}

void Player::Initialize()
{
    //values
    m_Guess = 0;
    m_SecretNumber = 0;
    m_BreadCount = 0;
    m_HaveWon = false;

    //Own and Enemy Arrays
    for (int i = 0; i < 10; i++)
    {
        m_BreadArray[i] = std::make_shared<Bread>();
        m_BreadArray[i]->Initialize();
        m_BreadArray[i]->UpdataData( 100*(i+1), 100,i+1);
        m_EnemyBreadArray[i] = std::make_shared<Bread>();
        m_EnemyBreadArray[i]->Initialize();
        m_EnemyBreadArray[i]->UpdataData(100 * (i + 1), 600, i + 1);
    }

    //bool array
    for (int i = 0; i < 10; i++)
    {
        m_BreadTracker[i] = false;
    }

    //red indicator
    m_Indicator = std::make_shared<Sprite>();
    m_Indicator->Load("Assets/Images/Indicator.png", "Indicator");
    m_Indicator->SetImage("Indicator");
    m_Indicator->Disable();
    m_Indicator->SetSpriteDimension(32, 32);
    m_Indicator->SetImageDimension(1, 1, 32, 32);
    m_Indicator->SetImageCel(1, 1);

    //green indicator
    m_EnemyIndicator = std::make_shared<Sprite>();
    m_EnemyIndicator->Load("Assets/Images/Indicator2.png", "EnemyIndicator");
    m_EnemyIndicator->SetImage("EnemyIndicator");
    m_EnemyIndicator->Disable();
    m_EnemyIndicator->SetSpriteDimension(32, 32);
    m_EnemyIndicator->SetImageDimension(1, 1, 32, 32);
    m_EnemyIndicator->SetImageCel(1, 1);
}

void Player::PressedNumber(int number)
{
    m_Guess = number;
    m_GuessPlaced = true; 
    PlaceIndicator(1100, 100, true); 
}

void Player::ResetTurn()
{
    m_Guess = 99; //random valued, different to avoid errors
    m_BreadCount = 0;
    m_SecretNumber = 100;
    m_GuessPlaced = false;
    PlaceIndicator(1100, 600, false);
    PlaceEnemyIndicator(1100, 600, false);
}

void Player::PlaceIndicator(int x, int y, bool OnOff)
{
    m_IndicatorX = x;
    m_IndicatorY = y;
    if (OnOff) { m_Indicator->Enable(); }
    else { m_Indicator->Disable(); }
}

void Player::PlaceEnemyIndicator(int x, int y, bool OnOff)
{
    m_EnemyIndicatorX = x;
    m_EnemyIndicatorY = y;
    if (OnOff) { m_EnemyIndicator->Enable(); }
    else { m_EnemyIndicator->Disable(); }
}

Player::Player()
{
    m_BreadTracker = { false, false, false, false, false, false, false, false, false, false };

    m_EnemyIndicatorX = 0;
    m_EnemyIndicatorY = 0;
    m_SecretNumber = 0;
    m_BreadCount = 0;
    m_IndicatorX = 0;
    m_IndicatorY = 0;
    m_Guess = 0;

    m_GuessPlaced = false;
    m_HaveWon = false;
}

std::string Player::GetMessageToSend()
{
    std::string TempString = m_Identity + ":" + std::to_string(m_Guess);
    return TempString;
}
