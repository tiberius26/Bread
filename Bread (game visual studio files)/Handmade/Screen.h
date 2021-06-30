#ifndef SCREEN_H
#define SCREEN_H

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
| 'Screen' source files last updated on 6 August 2020							                |
#==============================================================================================*/

#include <string>
#include <SDL.h>
#include "Vector.h"

class Screen
{

public:

	static Screen* Instance();

public:

	SDL_Window* GetWindow();
	SDL_Renderer* GetRenderer();
	Vector<int> GetResolution();

public:

	void SetMousePosition(int x, int y);
	void SetClearColor(Uint8 r, Uint8 g, Uint8 b);

public:

	bool Initialize(const std::string& windowTitle, int width = 1024, 
		            int height = 768, bool fullscreen = false);
	void Update();
	void Draw();
	void Shutdown();

private:

	Screen();
	Screen(const Screen&);
	Screen& operator=(const Screen&);

private:

	int m_width;
	int m_height;

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	
};

#endif