#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

/*==============================================================================================#
|                                                                                               |
| Handmade Lite is an educational Visual Studio framework, written by Karsten Vermeulen,        |
| designed to assist with educating other fellow programmers, programming students, and anyone  |
| else wishing to learn about game development, C++, and object-oriented programming. This      |
| project is not open source and therefore has no license affiliated with it. Please be         |
| respectful and do not distribute the engine or its code as your work. You may alter or tweak  |
| the engine code as a learning experience and you may also 'borrow' code and functionality     |
| and use it to create your own game engine. Finally, please do use Handmade Lite to develop    |
| a game.                                                                                       |
|                                                                                               |
| Designed to teach. Made from scratch. Built by hand.							                |
|	                                                                                            |
#===============================================================================================#
|                                                                                               |
| If you like Handmade Lite and wish to show your support, if you have any questions about      |
| the project, or if you just want to reach out, please do get in touch:                        |
|																						        |
| Facebook | https://www.facebook.com/Handmade-Lite-107188757745777								|
| Twitter  | https://twitter.com/KarstensCorner													|
| GitHub   | https://github.com/djkarstenv/Handmade-Lite									    |
|                                                                                               |
#===============================================================================================#
| 'GameObject' source files last updated on 11 August 2020							            |
#==============================================================================================*/

#include <string>
#include "Vector.h"

class GameObject 
{

public:

	GameObject();
	virtual ~GameObject() = 0 {}
	
public:

	bool IsAlive() const;
	bool IsActive() const;
	bool IsVisible() const;

	void IsAlive(bool flag);
	void IsActive(bool flag);
	void IsVisible(bool flag);

public:

	float GetAngle() const;
	int GetPriority() const;

	const std::string& GetTag() const;
	const Vector<int>& GetPosition() const;
	const Vector<int>& GetDimension() const;

	void SetPosition(int x, int y);
	void SetDimension(int x, int y);
	void SetTag(const std::string& tag);

public:

	virtual void Update(int deltaTime) = 0;
	virtual bool Draw() = 0;

protected:

	float m_angle;
	int m_priority;
	
	bool m_isAlive;
	bool m_isActive;
	bool m_isVisible;

	std::string m_tag;
	Vector<int> m_position;
	Vector<int> m_dimension;

};

#endif