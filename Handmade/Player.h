#pragma once
#include <array>
#include "Bread.h"
#include "Sprite.h"
class Player
{
public:
	int GetGuess() { return m_Guess; };
	int GetGuess()const { return m_Guess; };
	int GetSecretNumber() { return m_Guess; };
	int GetSecretNumber()const { return m_Guess; };

	void SetGuess(int guess) { m_Guess = guess; };
	void SetSecretNumber(int secretnumber) { m_SecretNumber = secretnumber; };

	void IncreaseBread(int index);
	void IncreaseEnemyBread(int index);
	void Draw();

	void UpdateCounter();
	bool CheckCounter(int ElementToCheck);
	void UpdateBreadCount();
	bool CheckWin();
	void Initialize();
	void PressedNumber(int number);
	void SetIdentity(std::string Identity) { m_Identity = Identity; }
	std::string GetIdentity() { return m_Identity; }
	void ResetTurn();
	void GuessPlaced() { m_GuessPlaced = true; }
	bool CheckGuessPlaced() { return m_GuessPlaced; }
	void PlaceIndicator(int x, int y, bool OnOff);
	void PlaceEnemyIndicator(int x, int y, bool OnOff);
	
private:
	int m_Guess = 0;
	int m_SecretNumber = 0;
	bool m_HaveWon = false;
	bool m_MyTurn = false;
	bool m_GuessPlaced = false;
	std::array<Bread, 10> m_BreadArray;
	std::array<Bread, 10> m_EnemyBreadArray;
	std::array<bool, 10> m_BreadTracker = { false, false, false, false, false, false, false, false, false, false};
	int m_BreadCount = 0;
	std::string m_Identity;
	Sprite m_Indicator;
	int m_IndicatorX = 0, m_IndicatorY = 0;
	Sprite m_EnemyIndicator;
	int m_EnemyIndicatorX = 0, m_EnemyIndicatorY = 0;
};

