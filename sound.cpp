#include "lunamiga.h"
//#include <iostream>

void SoundInit(maindata *lunadata) {
/*MOD
	int init = Mix_Init(MIX_INIT_OGG);

	if ((init&MIX_INIT_OGG) != MIX_INIT_OGG)
	{
		std::cout << "ERROR: Sound init failed: " << Mix_GetError() << std::endl;
		//sndena = false;
	}
	else
	{
		if ((init = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)) == 0)
		{
			Mix_AllocateChannels(AUDIO_CHANNELS); // Amount of mixed sound channels
			Mix_ReserveChannels(AUDIO_RESERVED_CHS); // How many channels to reserve for 'play on first free channel' (channel -1)
			if((lunadata->sound.mixchunk[SOUND_SHOOT] = Mix_LoadWAV("assets/hallow-shoot.wav")) == 0)
				std::cout << "ERROR: Cannot load shoot sound: " << Mix_GetError() << std::endl;
			if((lunadata->sound.mixchunk[SOUND_EXPLODE] = Mix_LoadWAV("assets/hallow-explode.wav")) == 0)
				std::cout << "ERROR: Cannot load explode sound: " << Mix_GetError() << std::endl;
			if((lunadata->sound.mixchunk[SOUND_BONUS] = Mix_LoadWAV("assets/hallow-bonus.wav")) == 0)
				std::cout << "ERROR: Cannot load bonus sound: " << Mix_GetError() << std::endl;
			if((lunadata->sound.mixchunk[SOUND_SCREAM] = Mix_LoadWAV("assets/hallow-scream.wav")) == 0)
				std::cout << "ERROR: Cannot load scream sound: " << Mix_GetError() << std::endl;
			Mix_Volume(-1, MIX_MAX_VOLUME * 0.4);
			
			if((lunadata->sound.musicgame = Mix_LoadMUS("assets/hallow-music.ogg")) == 0)
				std::cout << "ERROR: Cannot load main music: " << Mix_GetError() << std::endl;
			if((lunadata->sound.musicgameover = Mix_LoadMUS("assets/hallow-gameover.ogg")) == 0)
				std::cout << "ERROR: Cannot load gameover music: " << Mix_GetError() << std::endl;
			Mix_VolumeMusic(MIX_MAX_VOLUME * 0.9);
			//sndena = true;
		}
		else
		{
			std::cout << "ERROR: Unable to init sound source: " << Mix_GetError() << std::endl;
			//sndena = false;
		}
	}
*/
}

void SoundGetNextChannel(maindata *lunadata) {
}

void SoundSfxShoot(maindata *lunadata) {
//MOD	Mix_PlayChannel(lunadata->sound.sndchan[SOUND_SHOOT], lunadata->sound.mixchunk[SOUND_SHOOT], 0);
}

void SoundSfxExplode(maindata *lunadata) {
//MOD	Mix_PlayChannel(lunadata->sound.sndchan[SOUND_EXPLODE], lunadata->sound.mixchunk[SOUND_EXPLODE], 0);
}

void SoundSfxBonus(maindata *lunadata) {
//MOD	Mix_PlayChannel(lunadata->sound.sndchan[SOUND_BONUS], lunadata->sound.mixchunk[SOUND_BONUS], 0);
}

void SoundSfxScream(maindata *lunadata) {
//MOD	Mix_PlayChannel(lunadata->sound.sndchan[SOUND_SCREAM], lunadata->sound.mixchunk[SOUND_SCREAM], 0);
}
