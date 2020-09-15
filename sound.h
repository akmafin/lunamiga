#ifndef sound_h
#define sound_h

#define AUDIO_CHANNELS			16
#define AUDIO_RESERVED_CHS		8
#define NUM_SOUNDS				4

#define SOUND_SHOOT				0
#define SOUND_EXPLODE			1
#define SOUND_BONUS				2
#define SOUND_SCREAM			3

struct sound {
	Mix_Music *musicgame, *musicgameover;
	Mix_Chunk *mixchunk[NUM_SOUNDS] = {0};
	int sndchan[NUM_SOUNDS] = {-1, -1, -1, -1};
};

void SoundInit(maindata *lunadata);
void SoundGetNextChannel(maindata *lunadata);
void SoundSfxShoot(maindata *lunadata);
void SoundSfxExplode(maindata *lunadata);
void SoundSfxBonus(maindata *lunadata);
void SoundSfxScream(maindata *lunadata);

#endif
