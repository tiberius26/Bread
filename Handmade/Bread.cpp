#include "Bread.h"

void Bread::IncrementCounter()
{
	m_Counter++;
	if (m_Counter == 3) 
	{
		m_Marked = true;
		m_NormalSkin.Disable();
		m_MarkedSkin.Enable();
		m_Collision.Disable();
	}
}

void Bread::Draw()
{
	if (m_Marked) 
	{
		m_MarkedSkin.Draw();
	}
	else 
	{
		m_NormalSkin.Draw();
	}
}

void Bread::Initialize()
{
	m_Counter = 0;
	m_Marked = false;
	m_NormalSkin.
}

