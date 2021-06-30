#pragma once
#include <array>
#include "Sprite.h"
#include <memory>

class Bread
{
public:
	//Getters
	int GetCounter() { return m_Counter; }; //checks the number of points for this bread

	bool GetMark() { return m_Marked; } //checks if the bread has been marked (reached over 2 points)

	//void methods, updates, increments etc
	void UpdataData(int X, int Y, int BreadIdentity); // identity = image of the bread
	void Initialize();
	void Unload();
	void Draw();

	//increment
	void IncrementCounter();

	//inline set
	void MarkBread() { m_Marked = true; }; //marks the bread

private:
	//ints
	int m_Counter;
	int m_Identity;
	int m_PositionX;
	int m_PositionY;

	//bools
	bool m_Marked;

	//sprite pointers
	std::shared_ptr<Sprite> m_NormalSkin; //skin when bread is not made
	std::shared_ptr<Sprite> m_MarkedSkin; //skin when bread is made

};

