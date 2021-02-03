#pragma once
#include <array>
#include "Sprite.h"
#include "AABB.h"
#include <memory>

class Bread
{
public:
	int GetCounter() { return m_Counter; };
	int GetCounter()const { return m_Counter; };
	bool GetMark() { return m_Marked; }
	bool GetMark()const { return m_Marked; }

	bool MarkBread() { m_Marked = true; };
	void IncrementCounter();
	void Draw();
	void Initialize();
	void UpdataData(int X, int Y, int BreadIdentity);

	void Unload();
private:
	int m_Counter = 0;
	int m_Identity = 0;
	int m_PositionX = 0;
	int m_PositionY = 0;
	bool m_Marked = false;
	std::shared_ptr<Sprite> m_NormalSkin; //skin when bread is not made
	std::shared_ptr<Sprite> m_MarkedSkin; //skin when bread is made
	std::shared_ptr<AABB>  m_Collision;
};

