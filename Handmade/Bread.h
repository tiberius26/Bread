#pragma once
#include <array>
#include "Sprite.h"
#include "AABB.h"
class Bread
{
public:
	int GetCounter() { return m_Counter; };
	int GetCounter()const { return m_Counter; };
	bool GetMark() { return m_Marked; }
	bool GetMark()const { return m_Marked; }

	bool MarkBread() { m_Marked = true; };
	void IncrementCounter();
	bool Clicked(const AABB& bound) { return m_Collision.IsColliding(bound); };
	void Draw();
	void Initialize();
private:
	int m_Counter;
	bool m_Marked;
	Sprite m_NormalSkin; //skin when bread is not made
	Sprite m_MarkedSkin; //skin when bread is made
	AABB m_Collision;
};

