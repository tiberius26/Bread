#ifndef SOUND_H
#define SOUND_H

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
| 'Sound' source files last updated on 27 July 2020							                    |
#==============================================================================================*/

#include <map>
#include <string>
#include <SDL_mixer.h>

class Sound
{

public:

	static bool Load(const std::string& filename, const std::string& mapIndex);
	static void Unload(const std::string& mapIndex = "");

public:

	Sound();

public:

	void SetVolume(int volume);
	bool SetSound(const std::string& mapIndex);

public:

	bool Play(int loop = 0);			
	
private:

	static std::map<std::string, Mix_Chunk*>* s_sounds;

private:
	
	Mix_Chunk* m_sound;
					
};

#endif	