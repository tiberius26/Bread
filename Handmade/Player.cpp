#include "Player.h"

void Player::UpdateCounter()
{
    for (int i=0; i < 10; i++)
    {
        if (m_BreadArray[i].GetMark()) { m_BreadTracker[i] = true; }
    }
}

bool Player::CheckCounter(int ElementToCheck)
{
    if (m_BreadCount > 8) { return true; }
    return false;
}
