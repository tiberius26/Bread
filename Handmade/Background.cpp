#include "Background.h"
#include "Screen.h"
#include "Vector.h"

//======================================================================================================
Background::Background(const std::string& imageFilename, const std::string& audioFilename)
{
	m_isPlaying = false;
	Vector<int> resolution = Screen::Instance()->GetResolution();

	m_image.Load(imageFilename, imageFilename);
	m_image.SetImage(imageFilename);
	m_image.SetSpriteDimension(resolution.x, resolution.y);
	m_image.SetImageDimension(1, 1, resolution.x, resolution.y);

	m_music.Load(audioFilename, audioFilename);
	m_music.SetMusic(audioFilename);

	m_imageID = imageFilename;
	m_audioID = audioFilename;
}
//======================================================================================================
Background::~Background()
{
	m_music.Unload(m_audioID);
	m_image.Unload(m_imageID);
}
//======================================================================================================
bool Background::Draw()
{
	m_image.Draw();
	return true;
}
//======================================================================================================
void Background::PlayMusic()
{
	if (!m_isPlaying)
	{
		m_music.Play(Music::LoopType::PLAY_ENDLESS);
		m_isPlaying = true;
	}
}
//======================================================================================================
void Background::StopMusic()
{
	m_music.Stop();
	m_isPlaying = false;
}