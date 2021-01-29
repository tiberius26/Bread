#pragma once
#include <array>
#include "Bread.h"
class Player
{
public:
	int GetGuess() { return m_Guess; };
	int GetGuess()const { return m_Guess; };
	int GetSecretNumber() { return m_Guess; };
	int GetSecretNumber()const { return m_Guess; };

	void UpdateCounter();
	bool CheckCounter(int ElementToCheck);
private:
	int m_Guess;
	int m_SecretNumber;
	std::array<Bread, 10> m_BreadArray;
	std::array<bool, 10> m_BreadTracker;
	int m_BreadCount;
};

