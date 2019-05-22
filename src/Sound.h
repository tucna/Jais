//-----------------------------------------------------------------
// Sound Object
// C++ Header - Sound.h
//-----------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <vector>
#include <string>

#define SDL_SUBSYSTEMS SDL_INIT_AUDIO

const int MAX_SOUNDS = 10;

using namespace std;
//-----------------------------------------------------------------
// Sound Class
//-----------------------------------------------------------------
class Sound
{
	private:
		Uint8 				g_win_flags;
		SDL_Surface*		g_screen;				// Surface okna
		Mix_Chunk* 			_mSounds;
		vector<Mix_Music>	m_music;
		
		int 			_iCountSounds;
		int 			g_volume ;				// Hlasitost
		int 			g_volume_left;			// Hlasitost leveho kanalu
		int 			g_sound_distance;		// Vzdalenost od zdroje zvuku
		bool 			g_reverse_stereo;		// Prohodi levy a pravy kanal

	public:
		Sound();
		~Sound();
		
		void PlayMusic(int);
		
		bool AddMyMusic(string);
		
		void FadeOutMusic(int);
//		void PlayMySound(int,char*);
		void PlayMySound(int,int);		
		bool AddSound(char*);
		bool InitSound();
};
