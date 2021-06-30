#include "Debug.h"
#include "Music.h"

std::map<std::string, Mix_Music*>* Music::s_music = new std::map<std::string, Mix_Music*>;

//======================================================================================================
bool Music::Initialize(int frequency, int chunkSize)
{
	Debug::Log("Initializing audio sub-system...");

	if (Mix_OpenAudio(frequency, AUDIO_S16SYS, 2, chunkSize) == -1)
	{
		Debug::Log("Audio sub-system did not initialize properly.", Debug::ErrorCode::FAILURE);
		Debug::Log("===============================================================");
		return false;
	}

	Debug::Log("Audio sub-system successfully setup", Debug::ErrorCode::SUCCESS);
	Debug::Log("===============================================================");

	return true;
}
//======================================================================================================
bool Music::Load(const std::string& filename, const std::string& mapIndex)
{
	Debug::Log("Opening and reading music file: '" + filename + "'");

	if (s_music->find(mapIndex) != s_music->end())
	{
		Debug::Log("Music data already loaded in memory.", Debug::ErrorCode::WARNING);
		Debug::Log("===============================================================");
		return false;
	}
	
	Mix_Music* music = Mix_LoadMUS(filename.c_str());

	if (!music)
	{
		Debug::Log("File could not be loaded.", Debug::ErrorCode::FAILURE);
		Debug::Log("===============================================================");
		return false;
	}

	(*s_music)[mapIndex] = music;
	
	Debug::Log("File opened and read successfully.", Debug::ErrorCode::SUCCESS);
	Debug::Log("===============================================================");

	return true;
}
//======================================================================================================
void Music::Unload(const std::string& mapIndex)
{
	if (!mapIndex.empty())
	{
		Debug::Log("Unloading music data: '" + mapIndex + "'");

		auto it = s_music->find(mapIndex);

		if (it == s_music->end())
		{
			Debug::Log("Music data not found. Please enter a valid ID.", Debug::ErrorCode::WARNING);
			Debug::Log("===============================================================");
		}

		else
		{
			Mix_FreeMusic(it->second);
			s_music->erase(it);

			Debug::Log("Music data unloaded successfully.", Debug::ErrorCode::SUCCESS);
			Debug::Log("===============================================================");
		}
	}

	else 
	{
		Debug::Log("Unloading all music data.");

		for (auto it = s_music->begin(); it != s_music->end(); it++)
		{
			Mix_FreeMusic(it->second);
		}

		s_music->clear();

		Debug::Log("All music data unloaded successfully.", Debug::ErrorCode::SUCCESS);
		Debug::Log("===============================================================");
	}
}
//======================================================================================================
void Music::Shutdown()
{
	Mix_CloseAudio();
}
//======================================================================================================
Music::Music()
{
	m_music = nullptr;
}
//======================================================================================================
void Music::SetVolume(int volume)
{
	Mix_VolumeMusic(volume);
}
//======================================================================================================
bool Music::SetMusic(const std::string& mapIndex)
{
	auto it = s_music->find(mapIndex);

	if (it == s_music->end())
	{
		Debug::Log("Music data not found. Please enter a valid ID.", Debug::ErrorCode::WARNING);
		Debug::Log("===============================================================");
		return false;
	}

	m_music = (*it).second;
	return true;
}
//======================================================================================================
bool Music::Play(LoopType loopType)
{
	if(!Mix_PlayingMusic())
	{
		if(Mix_PlayMusic(m_music, static_cast<int>(loopType)) == -1)
		{
			Debug::Log("Music could not be played.", Debug::ErrorCode::FAILURE);
			Debug::Log("===============================================================");
			return false;
		}
	}
	
	return true;
}
//======================================================================================================
void Music::Pause()
{
	if(!Mix_PausedMusic())
	{
		Mix_PauseMusic();
	}
}
//======================================================================================================
void Music::Resume()
{
	if(Mix_PausedMusic())
	{
		Mix_ResumeMusic();
	}
}
//======================================================================================================
void Music::Stop()
{
	if(Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
}