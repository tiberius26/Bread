#include "Debug.h"
#include "Sound.h"

std::map<std::string, Mix_Chunk*>* Sound::s_sounds = new std::map<std::string, Mix_Chunk*>;

//======================================================================================================
bool Sound::Load(const std::string& filename, const std::string& mapIndex)
{
	Debug::Log("Opening and reading sound file: '" + filename + "'");

	if (s_sounds->find(mapIndex) != s_sounds->end())
	{
		Debug::Log("Sound data already loaded in memory.", Debug::ErrorCode::WARNING);
		Debug::Log("===============================================================");
		return false;
	}

	Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());

	if (!sound)
	{
		Debug::Log("File could not be loaded.", Debug::ErrorCode::FAILURE);
		Debug::Log("===============================================================");
		return false;
	}
	
	(*s_sounds)[mapIndex] = sound;

	Debug::Log("File opened and read successfully.", Debug::ErrorCode::SUCCESS);
	Debug::Log("===============================================================");

	return true;
}
//======================================================================================================
void Sound::Unload(const std::string& mapIndex)
{
	if (!mapIndex.empty())
	{
		Debug::Log("Unloading sound data: '" + mapIndex + "'");

		auto it = s_sounds->find(mapIndex);

		if (it == s_sounds->end())
		{
			Debug::Log("Sound data not found. Please enter a valid ID.", Debug::ErrorCode::WARNING);
			Debug::Log("===============================================================");
		}

		else
		{
			Mix_FreeChunk(it->second);
			s_sounds->erase(it);

			Debug::Log("Sound data unloaded successfully.", Debug::ErrorCode::SUCCESS);
			Debug::Log("===============================================================");
		}
	}

	else
	{
		Debug::Log("Unloading all sound data.");

		for (auto it = s_sounds->begin(); it != s_sounds->end(); it++)
		{
			Mix_FreeChunk(it->second);
		}

		s_sounds->clear();

		Debug::Log("All sound data unloaded successfully.", Debug::ErrorCode::SUCCESS);
		Debug::Log("===============================================================");
	}
}
//======================================================================================================
Sound::Sound()
{
	m_sound = nullptr;
}
//======================================================================================================
void Sound::SetVolume(int volume)
{
	Mix_VolumeChunk(m_sound, volume);
}
//======================================================================================================
bool Sound::SetSound(const std::string& mapIndex)
{
	auto it = s_sounds->find(mapIndex);

	if (it == s_sounds->end())
	{
		Debug::Log("Sound data not found. Please enter a valid ID.", Debug::ErrorCode::WARNING);
		Debug::Log("===============================================================");
		return false;
	}

	m_sound = (*it).second;
	return true;
}
//======================================================================================================
bool Sound::Play(int loop)
{
	if (Mix_PlayChannel(-1, m_sound, loop) == -1)
	{
		Debug::Log("Sound could not be played.", Debug::ErrorCode::FAILURE);
		Debug::Log("===============================================================");
		return false;
	}

	return true;
}