#ifndef lunapc_h
#define lunapc_h

#include "bullets.h"
#include "enemies.h"
#include "hud.h"
#include "map.h"
#include "player.h"
#include "sound.h"

#define GAMESTATE_QUIT				0
#define GAMESTATE_MENU				1
#define GAMESTATE_RUNNING			2
#define GAMESTATE_GAMEOVER			3

#define SUBTUNE_INTRO				0
#define SUBTUNE_GAME				0
#define SUBTUNE_GAMEOVER			1
#define SUBTUNE_BLANK				2

#define FONTTILE_WIDTH				16
#define FONTTILE_HEIGHT				16
#define SPRITETILE_WIDTH			48
#define SPRITETILE_HEIGHT			42

struct maindata {
	SDL_Window *mainwin;
	SDL_Renderer *mainrend;
	SDL_Texture *gamefonttex, *gamespritetex;
	SDL_GameController *gc;
	int gamestate, IntroActive = 0, MusicActive = 1, fullscreen = 0;
	int DeathAnimTimer[2] = {0, 4};
	int DeathAnimIndex = 0, DeathFrames[5] = {68, 69, 70, 71, 67};
	int GameOverFirePressed = 0, GameOverFireCountdown = 0, GameOverMusicTimer = 0;
	int GameOverSinTicker = 0, GameOverColRamp[8] = {1, 11, 3, 12, 4, 2, 9, 8};
	int SCREEN[1000], COLORRAM[1000], RandomNum = 0x62;
	int ZP_COUNTER = 0;
	int SPRITE_ENA = 0, SPRITE_PTRS[8] = {0}, SPRITE_X[8], SPRITE_Y[8];
	int GameOverSinY[256] = {0};
	int HighscoreAchieved = 0;
	int MessageIndex = 0, MessageLength = 0;
	char Hiscore[41] = "\0\0\0\0\0\0\0\0\0\0\0\0\x8\x9\x13\x3\xF\x12\x5\0\0\x30\x30\x30\x30\x30\x30\x30\0\0\0\0\0\0\0\0\0\0\0\0";
	char HighScoreText[27] = "\0\0\x1F\x1F\x1F\0\xE\x5\x17\0\x8\x9\x7\x8\0\x13\x3\xF\x12\x5\0\x1F\x1F\x1F\0\0";
	char MessageText[500] = {0};
	char IntroMap[440] = {0};
	uint32_t RandomNumState = 0x62;
	struct bullets bullets;
	struct enemies enemies;
	struct hud hud;
	struct map map;
	struct player player;
	struct sound sound;
};

void MainMenu(struct maindata *lunadata);
void GameLoop(maindata *lunadata);
void GameOver(maindata *lunadata);
void GameInit(struct maindata *lunadata);
void GameClean(struct maindata *lunadata);
void GameDrawScreen(maindata *lunadata);
void GameDelay(maindata *lunadata);
void ClearScreen(struct maindata *lunadata);
void ClearColor(struct maindata *lunadata);
void RandomInit(struct maindata *lunadata);
void RandomGet(struct maindata *lunadata);
void DrawHighscore(struct maindata *lunadata);
void GenerateStars(struct maindata *lunadata);
void DrawStars(struct maindata *lunadata);
void ClearStars(struct maindata *lunadata);
void CheckForHighscore(struct maindata *lunadata);
void StopSounds(struct maindata *lunadata);
unsigned char CharToDispChar(unsigned char inchr);
int StrToDispStr(const char* instr, char* outstr, int outbuflen);

#endif
