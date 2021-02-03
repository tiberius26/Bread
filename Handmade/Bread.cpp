#include "Bread.h"
#include "Input.h"

void Bread::IncrementCounter()
{
	m_Counter++;
	if (m_Counter > 2) 
	{
		m_Marked = true;
		m_NormalSkin->Disable();
		m_MarkedSkin->Enable();
		m_Collision->Disable();
	}
}


void Bread::Draw()
{

	if (m_Marked) 
	{
		m_MarkedSkin->Draw(m_PositionX, m_PositionY, 0.0, Sprite::FlipType::NO_FLIP);
	}
	else 
	{
		m_NormalSkin->Draw(m_PositionX, m_PositionY, 0.0, Sprite::FlipType::NO_FLIP);
	}
}

void Bread::Initialize()
{
	m_Counter = 0;
	m_Identity = 0;

	m_Marked = false;
	m_PositionX = 0;
	m_PositionY = 0;

	m_NormalSkin = std::make_shared<Sprite>();
	m_NormalSkin->Load("Assets/Images/0.png", "0");
	m_NormalSkin->SetImage("0");
	m_NormalSkin->Enable();
	m_NormalSkin->SetSpriteDimension(32, 32);
	m_NormalSkin->SetImageDimension(1, 1, 32, 32);
	m_NormalSkin->SetImageCel(1, 1);

	m_MarkedSkin = std::make_shared<Sprite>();
	m_MarkedSkin->Load("Assets/Images/Mark.png", "BreadMark");
	m_NormalSkin->SetImage("BreadMark");
	m_MarkedSkin->Disable();
	m_MarkedSkin->SetSpriteDimension(32,32);
	m_NormalSkin->SetImageDimension(1, 1, 32, 32);
	m_NormalSkin->SetImageCel(1, 1);

	m_Collision = std::make_shared<AABB>();
	m_Collision->Enable();
	m_Collision->SetDimension(32, 32);
	m_Collision->SetPosition(m_PositionX, m_PositionY);
	m_Collision->Update();
}


void Bread::UpdataData(int X, int Y, int BreadIdentity)
{
	m_PositionX = X;
	m_PositionY = Y;
	m_Identity = BreadIdentity;
	char TempString[100];
	sprintf_s(TempString, "Assets/Images/%i.png", BreadIdentity);
	m_NormalSkin->Load(TempString, std::to_string(BreadIdentity));
	m_NormalSkin->SetImage(std::to_string(BreadIdentity));
	m_Collision->SetPosition(m_PositionX, m_PositionY);
}

void Bread::Unload()
{
	m_NormalSkin->Unload();
	m_MarkedSkin->Unload();
}


