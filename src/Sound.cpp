//-----------------------------------------------------------------
// Sound Object
// C++ Source - Sound.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Sound.h"
//--------------------------

Sound::Sound()
{
	g_win_flags = SDL_HWSURFACE|SDL_DOUBLEBUF;
	g_volume = MIX_MAX_VOLUME;
	g_volume_left = 128;
	g_sound_distance = 128;
	g_reverse_stereo = false;

	_mSounds = NULL;
	_iCountSounds = -1;
}

Sound::~Sound()
{
/*
	if(g_music)
	{
		Mix_FreeMusic(g_music);
		g_music = NULL;
	}
*/
	Mix_CloseAudio();

	SDL_Quit();
	
	delete _mSounds;
}

void Sound::PlayMusic(int c)
{
	Mix_VolumeMusic(g_volume);
//	Mix_PlayMusic(m_music[c], -1);
}

bool Sound::AddMyMusic(string mus_filename)
{
/*	Mix_Music _music;
	
	if((_music = Mix_LoadMUS(mus_filename.data())) == NULL)
	{
		return false;
	}
	
	m_music.push_back(_music);*/
	return true;
}

//void Sound::PlayMySound(int channel,char* mus_filename)
void Sound::PlayMySound(int channel,int sound)
{
//	Mix_Chunk* chunk = Mix_LoadWAV(mus_filename);
//	Mix_PlayChannel(channel,&_mSounds[sound],0);	
//	SDL_Delay(10);

	Mix_PlayChannel(channel,_mSounds,0);
}

bool Sound::AddSound(char* mus_filename)
{
	if (++_iCountSounds < MAX_SOUNDS)
		{
//			_mSounds[_iCountSounds] = Mix_LoadWAV(mus_filename);
			_mSounds = Mix_LoadWAV(mus_filename);
		}
	else return false;
	
	return true;
}

bool Sound::InitSound()
{
	// Inicializace SDL
	if(SDL_Init(SDL_SUBSYSTEMS) == -1)
	{
		return false;
	}
	
	// Audio
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,	MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		return false;
	}
	
	return true;	
}

void Sound::FadeOutMusic(int ms)
{
	Mix_FadeOutMusic(ms);	
}
