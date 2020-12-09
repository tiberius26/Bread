#ifndef BACKGROUND_H
#define BACKGROUND_H

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
| 'Background' source files last updated on 11 August 2020							            |
#==============================================================================================*/

#include "GameObject.h"
#include "Music.h"
#include "Sprite.h"

class Background : public GameObject
{

public:

	Background(const std::string& imageFilename, const std::string& audioFilename);
	virtual ~Background();

public:

	virtual void Update(int deltaTime) {}
	virtual bool Draw();

public:

	void PlayMusic();
	void StopMusic();

protected:

	bool m_isPlaying;
	std::string m_imageID;
	std::string m_audioID;

	Music m_music;
	Sprite m_image;

};

#endif
