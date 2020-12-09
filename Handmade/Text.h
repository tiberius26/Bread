#ifndef TEXT_H
#define TEXT_H

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
| 'Text' source files last updated on 28 July 2020							                    |
#==============================================================================================*/

#include <map>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class Text
{

public:

	enum class FontSize { SMALL = 72, MEDIUM = 144, LARGE = 300 };  

public:

	static bool Initialize();
	static bool Load(const std::string& filename, 
		             const std::string& mapIndex, FontSize fontSize = FontSize::SMALL);
	static void Unload(const std::string& mapIndex = "");
	static void Shutdown();

public:

	Text();
	Text(const Text& copy);
	~Text();

public:

	const SDL_Point& GetSize() const;
	const std::string& GetText() const;

public:

	void SetSize(int width, int height);
	void SetText(const std::string& text);
	void SetColor(Uint8 r, Uint8 g, Uint8 b);
	bool SetFont(const std::string& mapIndex);

public:

	void Draw(int positionX = 0, int positionY = 0);

private:

	void CreateText();

private:

	static std::map<std::string, TTF_Font*>* s_fonts;

private:

	std::string m_text;               
	SDL_Point m_textSize;             

	TTF_Font* m_font;                
	SDL_Color m_color;                
	SDL_Texture* m_texture;           

};

#endif