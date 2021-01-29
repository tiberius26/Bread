#include <algorithm>
#include <SDL_image.h>
#include "Debug.h"
#include "Screen.h"
#include "Sprite.h"

std::map<std::string, SDL_Texture*>* Sprite::s_images = new std::map<std::string, SDL_Texture*>;

//======================================================================================================
bool Sprite::Load(const std::string& filename, const std::string& mapIndex)
{
	Debug::Log("Opening and reading image file: '" + filename + "'");

	if (s_images->find(mapIndex) != s_images->end())
	{
		Debug::Log("Image data already loaded in memory.", Debug::ErrorCode::WARNING);
		Debug::Log("===============================================================");
		return false;
	}

	SDL_Surface* imageData = IMG_Load(filename.c_str());

	if (!imageData)
	{
		Debug::Log("File could not be loaded.", Debug::ErrorCode::FAILURE);
		Debug::Log("===============================================================");
		return false;
	}

	SDL_Texture* image = SDL_CreateTextureFromSurface(Screen::Instance()->GetRenderer(), imageData);
	SDL_FreeSurface(imageData);

	(*s_images)[mapIndex] = image;

	Debug::Log("File opened and read successfully.", Debug::ErrorCode::SUCCESS);
	Debug::Log("===============================================================");

	return true;
}
//======================================================================================================
void Sprite::Unload(const std::string& mapIndex)
{
	if (!mapIndex.empty())
	{
		Debug::Log("Unloading image data: '" + mapIndex + "'");

		auto it = s_images->find(mapIndex);

		if (it == s_images->end())
		{
			Debug::Log("Image data not found. Please enter a valid ID.", Debug::ErrorCode::WARNING);
			Debug::Log("===============================================================");
		}

		else
		{
			SDL_DestroyTexture(it->second);
			s_images->erase(it);

			Debug::Log("Image data unloaded successfully.", Debug::ErrorCode::SUCCESS);
			Debug::Log("===============================================================");
		}
	}

	else
	{
		Debug::Log("Unloading all image data.");

		for (auto it = s_images->begin(); it != s_images->end(); it++)
		{
			SDL_DestroyTexture(it->second);
		}

		s_images->clear();

		Debug::Log("All image data unloaded successfully.", Debug::ErrorCode::SUCCESS);
		Debug::Log("===============================================================");
	}
}
//======================================================================================================
Sprite::Sprite()
{
	m_imageCel = 0;
	m_animationVelocity = 0.0f;
	
	m_isAnimated = false;
	m_isAnimationDead = false;
	m_isAnimationLooping = true;
	m_isAnimationLoopFinal = false;
	
	m_image = nullptr;
	m_celDimension = { 0, 0 };
	m_imageDimension = { 1, 1 };
	m_spriteDimension = { 0, 0 };
}
//======================================================================================================
bool Sprite::IsAnimationDead()
{
	return m_isAnimationDead;
}
//======================================================================================================
void Sprite::IsAnimated(bool flag)
{
	m_isAnimated = flag;
}
//======================================================================================================
bool Sprite::IsAnimationLooping()
{
	return m_isAnimationLooping;
}
//======================================================================================================
void Sprite::IsAnimationLooping(bool flag)
{
	m_isAnimationLooping = flag;
}
//======================================================================================================
void Sprite::SetImageCel(int column, int row)
{
	row = std::max(row, 1);
	column = std::max(column, 1);
	m_imageCel = ((row - 1) * m_imageDimension.x) + (column - 1);
}
//======================================================================================================
void Sprite::SetAnimationVelocity(float velocity)
{
	m_animationVelocity = velocity;
}
//======================================================================================================
bool Sprite::SetImage(const std::string& mapIndex)
{
	auto it = s_images->find(mapIndex);

	if (it == s_images->end())
	{
		Debug::Log("Image data not found. Please enter a valid ID.", Debug::ErrorCode::WARNING);
		Debug::Log("===============================================================");
		return false;
	}

	m_image = (*it).second;
	return true;
}
//======================================================================================================
void Sprite::SetSpriteDimension(int width, int height)
{
	m_spriteDimension.x = width;
	m_spriteDimension.y = height;
}
//======================================================================================================
void Sprite::SetImageDimension(int columns, int rows, int width, int height)
{
	rows = std::max(rows, 1);
	columns = std::max(columns, 1);

	m_imageDimension.x = columns;
	m_imageDimension.y = rows;

	m_celDimension.x = width / columns;
	m_celDimension.y = height / rows;
}
//======================================================================================================
void Sprite::Update(int deltaTime)
{
	//if sprite is an animation we have to calculate the 
	//image cel each frame because it won't be a static value
	if (m_isAnimated)
	{
		//we need to keep accumulating time for the formula below
		//this is done by adding on the delta time each frame
		//we divide by 1000.0f because its MS time and we need
		//to make sure that we don't trim the decimal portion 
		static float totalTime = 0.0f;
		totalTime += deltaTime / 1000.0f;

		//aquire index value of specific texture cel to 'cut out' using a formula
		//the image index is zero based and is a whole number value counting from
		//top left and going right and down the sprite sheet, and is capable of 'wrapping'
		m_imageCel = static_cast<int>(totalTime * m_animationVelocity) %
			         static_cast<int>(m_imageDimension.x * m_imageDimension.y);
		
		//if animation is set to cycle endlessly then set the kill and final flags
		//to false so that no other final checks are made and that the animation loops
		if (m_isAnimationLooping)
		{
			m_isAnimationDead = false;
			m_isAnimationLoopFinal = false;
		}

		//otherwise if animation is set to cycle once and the last image
		//cel has been reached then flag this as the final animation loop
		else if (!m_isAnimationLooping &&
			     m_imageCel == (m_imageDimension.x * m_imageDimension.y - 1))
		{
			m_isAnimationLoopFinal = true;
		}

		//if this is the final animation, flag to kill entire animation
		//because even though the animation is marked final, a few more 
		//frames will be called with the last image cel set, so only
		//mark it dead when the first image cel comes around again
		if (m_isAnimationLoopFinal && m_imageCel == 0)
		{
			m_isAnimationDead = true;
		}
	}
}
//======================================================================================================
void Sprite::Draw(int positionX, int positionY, double angle, FlipType flipType)
{
	if (m_IsEnabled) {
		//if animation is set to run, then render the
		//sprite using the internal blitting function
		if (!m_isAnimationDead)
		{
			//variables to store rectangular dimensions for the source 
			//sprite and destination portion of the screen to render to 
			SDL_Rect src;
			SDL_Rect dst;

			//use modulo and divide with the image index to get exact cel block xy coordinates
			//to 'cut out' and assign all of this as well as width and height to the source rect
			src.x = (m_imageCel % m_imageDimension.x) * m_celDimension.x;
			src.y = (m_imageCel / m_imageDimension.x) * m_celDimension.y;
			src.w = m_celDimension.x;
			src.h = m_celDimension.y;

			//assign dimension of rectangular block to which sprite will be rendered to on screen
			dst.x = positionX;
			dst.y = positionY;
			dst.w = m_spriteDimension.x;
			dst.h = m_spriteDimension.y;

			//if sprite needs to be rotated, use the centre of the sprite as its centre of rotation
			SDL_Point centrePoint;
			centrePoint.x = m_spriteDimension.x / 2;
			centrePoint.y = m_spriteDimension.y / 2;

			//render the sprite using all values passed and determined above
			SDL_RenderCopyEx(Screen::Instance()->GetRenderer(),
				m_image, &src, &dst, angle, &centrePoint, static_cast<SDL_RendererFlip>(flipType));
		}
	}
}