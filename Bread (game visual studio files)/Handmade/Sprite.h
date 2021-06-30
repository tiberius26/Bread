#ifndef SPRITE_H
#define SPRITE_H

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
| 'Sprite' source files last updated on 27 July 2020							                |
#==============================================================================================*/

#include <map>
#include <string>
#include <SDL_render.h>

class Sprite
{

public:

	enum class FlipType { NO_FLIP = SDL_FLIP_NONE, 
		                  HORIZONTAL = SDL_FLIP_HORIZONTAL, 
		                  VERTICAL = SDL_FLIP_VERTICAL };

public:

	static bool Load(const std::string& filename, const std::string& mapIndex);
	static void Unload(const std::string& mapIndex = "");

public:

	Sprite();           

public:

	bool IsAnimationDead();
	void IsAnimated(bool flag);
	bool IsAnimationLooping();
	void IsAnimationLooping(bool flag);

public:

	void SetImageCel(int column, int row);
	void SetAnimationVelocity(float velocity);
	bool SetImage(const std::string& mapIndex);
	void SetSpriteDimension(int width, int height);
	void SetImageDimension(int columns, int rows, int width, int height);
	
public:

	void Update(int deltaTime);
	void Draw(int positionX = 0, int positionY = 0, double angle = 0.0, 
		      FlipType flipType = FlipType::NO_FLIP);

	void Disable() { m_IsEnabled = false; }
	void Enable() { m_IsEnabled = true; }
private:

	static std::map<std::string, SDL_Texture*>* s_images;

private:

	int m_imageCel;
	float m_animationVelocity;
	
	bool m_isAnimated;
	bool m_isAnimationDead;
	bool m_isAnimationLooping;
	bool m_isAnimationLoopFinal;

	bool m_IsEnabled;

	SDL_Texture* m_image;               
	SDL_Point m_celDimension;           
	SDL_Point m_imageDimension;         
	SDL_Point m_spriteDimension;        

};

#endif