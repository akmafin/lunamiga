#ifndef lunapc_h
#define lunapc_h

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include <exec/ports.h>
#include <exec/execbase.h>
#include <devices/keyboard.h>
#include <graphics/rastport.h>
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

#define FONTTILE_WIDTH				8
#define FONTTILE_HEIGHT				8
#define SPRITETILE_WIDTH			24
#define SPRITETILE_HEIGHT			21

#define KEYMATRIX_LEN   			16

struct maindata {
	struct Library *DOSBase, *GraphicsBase, *IntuitionBase;
	struct Screen *MyScreen;
	struct Window *MyWindow;
	struct MsgPort *keymp;
	struct IOStdReq *keyioreq;
	struct BitMap mycharbitmap, myspritebitmap, mysbbitmap;
	char *mycharplanes, *myspriteplanes, *mysbplanes;
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
	int SPRITE_ENA, SPRITE_PTRS[8], SPRITE_X[8], SPRITE_Y[8], SPRITE_OLDX[8], SPRITE_OLDY[8];
	int GameOverSinY[256];
	int HighscoreAchieved;
	int MessageIndex, MessageLength;
	int RandomNumState;
	int ScreenOffsetX, ScreenClear;
	char Hiscore[41];
	char HighScoreText[27];
	char MessageText[500];
	char IntroMap[440];
	struct bullets bullets;
	struct enemies enemies;
	struct hud hud;
	struct map map;
	struct player player;
	struct sound sound;
	UBYTE keymatrix[KEYMATRIX_LEN];
};

void MainMenu(struct maindata *lunadata);
void GameLoop(maindata *lunadata);
void GameOver(maindata *lunadata);
void GameInit(struct maindata *lunadata);
void GameClean(struct maindata *lunadata);
void GameDrawScreen(maindata *lunadata);
void GameClearScreen(struct maindata *lunadata);
void GameClearColor(struct maindata *lunadata);
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
