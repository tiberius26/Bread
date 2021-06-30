#pragma once
#include <array>
#include "Bread.h"
#include "Sprite.h"
#include <memory>
class Player
{
public:
	Player();

	//getters
	int GetGuess() { return m_Guess; };
	int GetSecretNumber() { return m_SecretNumber; };

	std::string GetMessageToSend();// prepares the message for sending before returning it

	//setters
	void SetGuess(int guess) { m_Guess = guess; };
	void SetSecretNumber(int secretnumber) { m_SecretNumber = secretnumber; };

	bool CheckWin();

	//increments
	void IncreaseBread(int index);
	void IncreaseEnemyBread(int index);

	//general void functions
	void PlaceEnemyIndicator(int x, int y, bool OnOff);  //indicators for enemy decision
	void PlaceIndicator(int x, int y, bool OnOff); //indicators for player decision
	void PressedNumber(int number); //called when an input is made
	void UpdateBreadCount(); // updates a tracker
	void UpdateCounter(); // updates an array
	void Initialize();
	void ResetTurn();
	void Draw();

	//inline setters
	void SetIdentity(std::string Identity) { m_Identity = Identity; }
	void GuessPlaced() { m_GuessPlaced = true; }

	//inline getters
	bool CheckGuessPlaced() { return m_GuessPlaced; }
	std::string GetIdentity() { return m_Identity; }

	
private:
	//ints
	int m_EnemyIndicatorX;
	int m_EnemyIndicatorY;
	int m_SecretNumber;
	int m_BreadCount;
	int m_IndicatorX;
	int m_IndicatorY;
	int m_Guess;

	//bools
	bool m_GuessPlaced;
	bool m_HaveWon;

	//bread pointer arrays
	std::array<std::shared_ptr<Bread>, 10> m_EnemyBreadArray;
	std::array<std::shared_ptr<Bread>, 10> m_BreadArray;

	//bool arrays
	std::array<bool, 10> m_BreadTracker;
	
	//strings
	std::string m_Identity;

	//sprite pointers
	std::shared_ptr <Sprite> m_EnemyIndicator;
	std::shared_ptr <Sprite> m_Indicator;
};

