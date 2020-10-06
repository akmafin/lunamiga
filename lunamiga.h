#ifndef lunapc_h
#define lunapc_h

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <exec/ports.h>
#include <exec/execbase.h>
#include <devices/keyboard.h>
#include "bullets.h"
#include "enemies.h"
#include "hud.h"
#include "map.h"
#include "player.h"
#include "sound.h"
#include "ami.h"

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

#define KEYMATRIX_LEN   			16

struct maindata {
/*MOD
	SDL_Window *mainwin;
	SDL_Renderer *mainrend;
	SDL_Texture *gamefonttex, *gamespritetex;
	SDL_GameController *gc;
*/
	int gamestate, IntroActive, MusicActive, fullscreen;
	int DeathAnimTimer[2];
	int DeathAnimIndex, DeathFrames[5];
	int GameOverFirePressed, GameOverFireCountdown, GameOverMusicTimer;
	int GameOverSinTicker, GameOverColRamp[8];
	int SCREEN[1000], COLORRAM[1000], RandomNum;
	int ZP_COUNTER;
	int SPRITE_ENA, SPRITE_PTRS[8], SPRITE_X[8], SPRITE_Y[8];
	int GameOverSinY[256];
	int HighscoreAchieved;
	int MessageIndex, MessageLength;
	char Hiscore[41];
	char HighScoreText[27];
	char MessageText[500];
	char IntroMap[440];
	int RandomNumState;
	struct bullets bullets;
	struct enemies enemies;
	struct hud hud;
	struct map map;
	struct player player;
	struct sound sound;
	struct Library *DOSBase, *GraphicsBase, *IntuitionBase;
	struct Screen *MyScreen;
	struct Window *MyWindow;
	struct MsgPort *keymp;
	struct IOStdReq *keyioreq;
	UBYTE keymatrix[KEYMATRIX_LEN];
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
int StrToDispStr(char *str, int outbuflen);

#endif
