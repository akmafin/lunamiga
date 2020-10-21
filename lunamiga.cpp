/*
 * LunAmiga V0.10 2020-10-06
 * by AkmaFin
 * 
 * Built with https://github.com/bebbo/amiga-gcc
 */

#define SWVER			0
#define SWREV			10

#include "lunamiga.h"
#include "font.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
//#include <iostream>
//#include <chrono>

int main( int argc, char* args[] ) {
	maindata* lunadata;

	lunadata = (maindata*)AllocMem(sizeof(maindata), MEMF_PUBLIC | MEMF_CLEAR);
	GameInit(lunadata);

	while(lunadata->gamestate != GAMESTATE_QUIT) {
		switch(lunadata->gamestate) {
			case GAMESTATE_RUNNING:
				GameLoop(lunadata);
				break;
			case GAMESTATE_GAMEOVER:
				GameOver(lunadata);
				break;
			default:
				MainMenu(lunadata);
				break;
		}
	}
/*Move(lunadata->MyWindow->RPort, 1 * 8, 20);
Text(lunadata->MyWindow->RPort, "B", 1);
std::cout << "B\n";*/

	GameClean(lunadata);
	FreeMem(lunadata, sizeof(maindata));

	return 0;
}

void MainMenu(maindata *lunadata) {
	struct IntuiMessage* msg;

/*MOD	if(lunadata->MusicActive)
		Mix_PlayMusic(lunadata->sound.musicgame, -1);*/
	MapInit(lunadata);
	GameClearScreen(lunadata);

	lunadata->IntroActive = 1;
	lunadata->map.MinGap = 17;
	lunadata->map.HscrollSpeed = 2;
	lunadata->SPRITE_ENA = 0;
	lunadata->MessageIndex = lunadata->MessageLength - 35;
	for(int i = 0; i < 8; i++)
		lunadata->SPRITE_PTRS[i] = 0;
	for(int i = 0; i < 40; i++) {
		MapGenerateColumn(lunadata);
		lunadata->map.NeedToShift = 1;
		MapScreenShift(lunadata);
	}

	for(int i = 0; i < 440; i++)
		lunadata->SCREEN[6 * 40 + i] = lunadata->IntroMap[i];

	DrawHighscore(lunadata);

	while(lunadata->gamestate == GAMESTATE_MENU) {
		GameDelay(lunadata);
		GameDrawScreen(lunadata);

		lunadata->GameOverSinTicker++;
		
		lunadata->SCREEN[1] = 40;
		lunadata->SCREEN[3] = 41;
		if(lunadata->MusicActive)
			lunadata->SCREEN[2] = 36;
		else
			lunadata->SCREEN[2] = 37;
		
		MapAdvanceMap(lunadata);
		MapScreenShift(lunadata);
		
		lunadata->keyioreq->io_Command = KBD_READMATRIX;
		lunadata->keyioreq->io_Data = lunadata->keymatrix;
		lunadata->keyioreq->io_Length = KEYMATRIX_LEN;
		DoIO((struct IORequest*)lunadata->keyioreq);
		
		if(lunadata->keymatrix[8] & 0x20)
			lunadata->gamestate = GAMESTATE_QUIT;
		if((lunadata->keymatrix[12] & 0x10) || (lunadata->keymatrix[6] & 0x08))
			lunadata->gamestate = GAMESTATE_RUNNING;

		while((msg = (struct IntuiMessage*)GetMsg(lunadata->MyWindow->UserPort)) != NULL)
		{
			if(msg->Class == IDCMP_CLOSEWINDOW)
				lunadata->gamestate = GAMESTATE_QUIT;
		}

/*MOD
		while(SDL_PollEvent(&e)) {

			switch(e.type) {

				case SDL_KEYDOWN:	
					switch(e.key.keysym.scancode) {

						case SDL_SCANCODE_ESCAPE:
							lunadata->gamestate = GAMESTATE_QUIT;
							break;
							
						case SDL_SCANCODE_A:
						case SDL_SCANCODE_KP_4:
						case SDL_SCANCODE_LEFT:
							lunadata->MusicActive = 0;
							Mix_HaltMusic();
							break;
							
						case SDL_SCANCODE_D:
						case SDL_SCANCODE_KP_6:
						case SDL_SCANCODE_RIGHT:
							if(!lunadata->MusicActive) {
								lunadata->MusicActive = 1;
								Mix_PlayMusic(lunadata->sound.musicgame, -1);
							}
							break;
							
						case SDL_SCANCODE_RCTRL:
						case SDL_SCANCODE_LCTRL:
						case SDL_SCANCODE_C:
							lunadata->gamestate = GAMESTATE_RUNNING;
							break;
							
						case SDL_SCANCODE_F1:
							lunadata->fullscreen ^= 1;
							if(lunadata->fullscreen)
								SDL_SetWindowFullscreen(lunadata->mainwin, SDL_WINDOW_FULLSCREEN);
							else
								SDL_SetWindowFullscreen(lunadata->mainwin, 0);
							break;
							
						default:
							break;
					}
					break;

				case SDL_QUIT:
					lunadata->gamestate = GAMESTATE_QUIT;
					break;

				default:
					break;
			}
		}
*/	}
}

void GameLoop(maindata *lunadata) {
	struct IntuiMessage* msg;

	lunadata->IntroActive = 0;
	MapInit(lunadata);
	PlayerInit(lunadata);
	EnemiesInit(lunadata);
	HudInit(lunadata);
	GenerateStars(lunadata);
/*MOD	if(lunadata->MusicActive)
		Mix_PlayMusic(lunadata->sound.musicgame, -1);*/

	while(lunadata->gamestate == GAMESTATE_RUNNING) {
		GameDelay(lunadata);
		GameDrawScreen(lunadata);

Move(lunadata->MyWindow->RPort, 1 * 8, 20);
Text(lunadata->MyWindow->RPort, "B", 1);
		ClearStars(lunadata);
		EnemiesDraw(lunadata);
		BulletsClear(lunadata);
		BulletsUpdate(lunadata);
		MapAdvanceMap(lunadata);
		
		MapScreenShift(lunadata);
		BulletsDraw(lunadata);
		DrawStars(lunadata);
		EnemiesUpdate(lunadata);
		lunadata->player.Joy = 0;

		lunadata->keyioreq->io_Command = KBD_READMATRIX;
		lunadata->keyioreq->io_Data = lunadata->keymatrix;
		lunadata->keyioreq->io_Length = KEYMATRIX_LEN;
		DoIO((struct IORequest*)lunadata->keyioreq);
		
		if(lunadata->keymatrix[8] & 0x20)
			lunadata->gamestate = GAMESTATE_QUIT;

		while((msg = (struct IntuiMessage*)GetMsg(lunadata->MyWindow->UserPort)) != NULL)
		{
			if(msg->Class == IDCMP_CLOSEWINDOW)
				lunadata->gamestate = GAMESTATE_QUIT;
		}
/*MOD
		while(SDL_PollEvent(&e)) {

			switch(e.type) {

				case SDL_KEYDOWN:	
					switch(e.key.keysym.scancode) {

						case SDL_SCANCODE_ESCAPE:
							lunadata->gamestate = GAMESTATE_QUIT;
							break;
							
						default:
							break;
					}
					break;

				case SDL_QUIT:
					lunadata->gamestate = GAMESTATE_QUIT;
					break;

				default:
					break;
			}
		}

		keystate=SDL_GetKeyboardState(NULL);
		
		if((keystate[SDL_SCANCODE_KP_8]) || (keystate[SDL_SCANCODE_W]) || (keystate[SDL_SCANCODE_UP]))
			lunadata->player.Joy |= JOY_UP;
		if((keystate[SDL_SCANCODE_KP_2]) || (keystate[SDL_SCANCODE_S]) || (keystate[SDL_SCANCODE_DOWN]))
			lunadata->player.Joy |= JOY_DOWN;
		if((keystate[SDL_SCANCODE_KP_4]) || (keystate[SDL_SCANCODE_A]) || (keystate[SDL_SCANCODE_LEFT]))
			lunadata->player.Joy |= JOY_LEFT;
		if((keystate[SDL_SCANCODE_KP_6]) || (keystate[SDL_SCANCODE_D]) || (keystate[SDL_SCANCODE_RIGHT]))
			lunadata->player.Joy |= JOY_RIGHT;
		if((keystate[SDL_SCANCODE_RCTRL]) || (keystate[SDL_SCANCODE_LCTRL]) || (keystate[SDL_SCANCODE_C]))
			lunadata->player.Joy |= JOY_FIRE;
*/
		PlayerUpdate(lunadata);
		PlayerDraw(lunadata);
		
		if(lunadata->player.PlayerIsDead)
			lunadata->gamestate = GAMESTATE_GAMEOVER;
		
		if((lunadata->player.PowerUpActive) && ((lunadata->ZP_COUNTER & 7) == 0))
			HudDecPower(lunadata);
		
		lunadata->ZP_COUNTER++;
	}
}

void GameOver(maindata *lunadata) {
	struct IntuiMessage* msg;

//MOD	Mix_HaltMusic();
	lunadata->map.HscrollSpeed = 0;
	lunadata->DeathAnimIndex = 0;
	lunadata->GameOverFireCountdown = 0x40;
	CheckForHighscore(lunadata);
	
	while(lunadata->gamestate == GAMESTATE_GAMEOVER) {
		GameDelay(lunadata);
		GameDrawScreen(lunadata);

		if(lunadata->GameOverFireCountdown > 0)
			lunadata->GameOverFireCountdown--;
		lunadata->DeathAnimTimer[0]--;
		if(lunadata->DeathAnimTimer[0] < 0) {
			lunadata->DeathAnimTimer[0] = lunadata->DeathAnimTimer[1];
			if(lunadata->DeathAnimIndex < 4) {
				lunadata->DeathAnimIndex++;
				lunadata->SPRITE_PTRS[0] = lunadata->DeathFrames[lunadata->DeathAnimIndex];
/*MOD				if((lunadata->DeathAnimIndex == 4) && (lunadata->MusicActive))
					Mix_PlayMusic(lunadata->sound.musicgameover, 0);*/
			}
		}
		
		BulletsClear(lunadata);
		BulletsUpdate(lunadata);
		BulletsDraw(lunadata);

		if(lunadata->DeathAnimIndex < 4) {
			ClearStars(lunadata);
			MapAdvanceMap(lunadata);
			MapScreenShift(lunadata);
			DrawStars(lunadata);
			
			EnemiesUpdate(lunadata);
			EnemiesDraw(lunadata);
		} else {
			lunadata->map.HscrollSpeed = 0;
			
			if(lunadata->HighscoreAchieved) {
				for(int i = 0; i < 26; i++)
					lunadata->SCREEN[18 * 40 + 7 + i] = lunadata->HighScoreText[i];
			}
			
			lunadata->player.PlayerIsDead = 0;
			lunadata->SPRITE_ENA = 0xFF;
			for(int i = 0; i < 8; i++)
				lunadata->SPRITE_PTRS[i] = 144 + i;
			lunadata->GameOverSinTicker++;
			if(lunadata->GameOverSinTicker > 255)
				lunadata->GameOverSinTicker = 0;
			
			for(int i = 0; i < 4; i++)
				lunadata->SPRITE_X[i] = 70 + i * 26;
			for(int i = 4; i < 8; i++)
				lunadata->SPRITE_X[i] = 94 + i * 26;
			
			for(int i = 0; i < 8; i++)
				lunadata->SPRITE_Y[i] = lunadata->GameOverSinY[(i * 8 + lunadata->GameOverSinTicker) & 0xFF];
		}

		lunadata->keyioreq->io_Command = KBD_READMATRIX;
		lunadata->keyioreq->io_Data = lunadata->keymatrix;
		lunadata->keyioreq->io_Length = KEYMATRIX_LEN;
		DoIO((struct IORequest*)lunadata->keyioreq);
		
		if(lunadata->keymatrix[8] & 0x20)
			lunadata->gamestate = GAMESTATE_QUIT;
		if((lunadata->keymatrix[12] & 0x10) || (lunadata->keymatrix[6] & 0x08)) {
			if(lunadata->GameOverFireCountdown == 0)
				lunadata->gamestate = GAMESTATE_MENU;
		}

		while((msg = (struct IntuiMessage*)GetMsg(lunadata->MyWindow->UserPort)) != NULL)
		{
			if(msg->Class == IDCMP_CLOSEWINDOW)
				lunadata->gamestate = GAMESTATE_QUIT;
		}

/*MOD
		while(SDL_PollEvent(&e)) {

			switch(e.type) {

				case SDL_KEYDOWN:	
					switch(e.key.keysym.scancode) {

						case SDL_SCANCODE_ESCAPE:
							lunadata->gamestate = GAMESTATE_QUIT;
							break;
							
						case SDL_SCANCODE_RCTRL:
						case SDL_SCANCODE_LCTRL:
						case SDL_SCANCODE_C:
							if(lunadata->GameOverFireCountdown == 0)
								lunadata->gamestate = GAMESTATE_MENU;
							break;
							
						default:
							break;
					}
					break;

				case SDL_QUIT:
					lunadata->gamestate = GAMESTATE_QUIT;
					break;

				default:
					break;
			}
		}
*/	}
}

void GameInit(maindata *lunadata) {
	struct NewScreen screen = {0, 0, 320, 200, 4, 0, 0, 0, 0, 0, (UBYTE*)"LunAmiga", 0, 0};
	struct NewWindow window = {0, 0, 320, 200, 0, 0, IDCMP_CLOSEWINDOW | IDCMP_VANILLAKEY | IDCMP_RAWKEY, WFLG_CLOSEGADGET, 0, 0, (UBYTE*)"LunAmiga", 0, 0, 320, 200, 320, 200, CUSTOMSCREEN};
	int DeathFrames[5] = {68, 69, 70, 71, 67};
	int GameOverColRamp[8] = {1, 11, 3, 12, 4, 2, 9, 8};
	char Hiscore[41] = "\0\0\0\0\0\0\0\0\0\0\0\0\x8\x9\x13\x3\xF\x12\x5\0\0\x30\x30\x30\x30\x30\x30\x30\0\0\0\0\0\0\0\0\0\0\0\0";
	char HighScoreText[27] = "\0\0\x1F\x1F\x1F\0\xE\x5\x17\0\x8\x9\x7\x8\0\x13\x3\xF\x12\x5\0\x1F\x1F\x1F\0\0";
	unsigned char title[440] = {
		0x00, 0x43, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x43, 0x45, 0x44, 0x00, 0x43, 0x45, 0x44, 0x00, 0x43, 0x45, 0x45,
		0x00, 0x03, 0x0F, 0x04, 0x05, 0x21, 0x00, 0x00, 0x01, 0x0B, 0x0D, 0x01,
		0x06, 0x09, 0x0E, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x41, 0x20, 0x42,
		0x20, 0x43, 0x45, 0x44, 0x20, 0x41, 0x00, 0x42, 0x00, 0x41, 0x00, 0x42,
		0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00,
		0x20, 0x41, 0x00, 0x42, 0x20, 0x41, 0x00, 0x42, 0x20, 0x41, 0x45, 0x42,
		0x00, 0x41, 0x46, 0x48, 0x00, 0x41, 0x00, 0x00, 0x00, 0x09, 0x04, 0x05,
		0x01, 0x21, 0x00, 0x00, 0x13, 0x08, 0x01, 0x0C, 0x0C, 0x01, 0x0E, 0x00,
		0x00, 0x49, 0x46, 0x4A, 0x5A, 0x49, 0x4B, 0x48, 0x00, 0x49, 0x20, 0x4A,
		0x00, 0x49, 0x20, 0x4A, 0x00, 0x41, 0x00, 0x20, 0x00, 0x41, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x20, 0x00, 0x00, 0x00, 0x69, 0x20, 0x69, 0x20, 0x6A, 0x69, 0x00,
		0x00, 0x69, 0x00, 0x6A, 0x00, 0x69, 0x00, 0x6A, 0x00, 0x41, 0x00, 0x20,
		0x00, 0x47, 0x46, 0x46, 0x00, 0x13, 0x0F, 0x15, 0x0E, 0x04, 0x21, 0x00,
		0x13, 0x14, 0x05, 0x10, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x68, 0x00, 0x68,
		0x20, 0x67, 0x68, 0x00, 0x00, 0x68, 0x00, 0x67, 0x00, 0x68, 0x00, 0x67,
		0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00,
		0x00, 0x00, 0x01, 0x00, 0x17, 0x09, 0x14, 0x03, 0x08, 0x22, 0x05, 0x0D,
		0x22, 0x15, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x12,
		0x14, 0x21, 0x00, 0x13, 0x08, 0x01, 0x0C, 0x0C, 0x01, 0x0E, 0x23, 0x00,
		0x06, 0x15, 0x12, 0x12, 0x0F, 0x19, 0x23, 0x00, 0x0D, 0x0F, 0x0E, 0x13,
		0x14, 0x05, 0x12, 0x13, 0x07, 0x0F, 0x02, 0x0F, 0x0F, 0x0D, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x00, 0x10, 0x12, 0x05, 0x13, 0x13,
		0x00, 0x06, 0x09, 0x12, 0x05, 0x00, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	lunadata->DOSBase = OpenLibrary("dos.library", 0);
	lunadata->GraphicsBase = OpenLibrary("graphics.library", 0);
	lunadata->IntuitionBase = OpenLibrary("intuition.library", 0);
	
	lunadata->MusicActive = 1;
	lunadata->RandomNum = 0x62;
	lunadata->RandomNumState = 0x62;
	lunadata->DeathAnimTimer[0] = 0;
	lunadata->DeathAnimTimer[1] = 4;
	for(int i = 0; i < 5; i++)
		lunadata->DeathFrames[i] = DeathFrames[i];
	for(int i = 0; i < 8; i++)
		lunadata->GameOverColRamp[i] = GameOverColRamp[i];
	for(int i = 0; i < 41; i++)
		lunadata->Hiscore[i] = Hiscore[i];
	for(int i = 0; i < 27; i++)
		lunadata->HighScoreText[i] = HighScoreText[i];

	lunadata->MyScreen = OpenScreen(&screen);
	window.Screen = lunadata->MyScreen;
	lunadata->MyWindow = OpenWindow(&window);
	ActivateWindow(lunadata->MyWindow);
	
	lunadata->keymp = AmiCreateMsgPort();
	lunadata->keyioreq = (struct IOStdReq*)AmiCreateIORequest((struct MsgPort*)lunadata->keymp, sizeof(struct IOStdReq));
	OpenDevice("keyboard.device", 0, (struct IORequest*)lunadata->keyioreq, 0);
	lunadata->mycharplanes = (char*)AllocMem(40 * 200 * 4, MEMF_PUBLIC | MEMF_CLEAR | MEMF_CHIP);
	InitBitMap(&lunadata->mycharbitmap, 4, 320, 200);
	lunadata->mycharbitmap.Planes[0] = (PLANEPTR)lunadata->mycharplanes;
	lunadata->mycharbitmap.Planes[1] = (PLANEPTR)lunadata->mycharplanes + 40 * 200;
	lunadata->mycharbitmap.Planes[2] = (PLANEPTR)lunadata->mycharplanes + 40 * 200 * 2;
	lunadata->mycharbitmap.Planes[3] = (PLANEPTR)lunadata->mycharplanes + 40 * 200 * 3;
	for(int i = 0; i < 16000; i++)
		((UWORD*)lunadata->mycharplanes)[i] = myfontData[i];
	for(int i = 0; i < 16; i++)
		SetRGB4(&lunadata->MyScreen->ViewPort, i, (myfontPaletteRGB4[i] & 0xF00) >> 8, (myfontPaletteRGB4[i] & 0x0F0) >> 4, myfontPaletteRGB4[i] & 0x00F);
/*MOD
	IMG_Init(IMG_INIT_PNG);
	surf = IMG_Load("assets/font.png");
	lunadata->gamefonttex = SDL_CreateTextureFromSurface(lunadata->mainrend, surf);
	SDL_FreeSurface(surf);
	surf = IMG_Load("assets/sprites.png");
	lunadata->gamespritetex = SDL_CreateTextureFromSurface(lunadata->mainrend, surf);
	SDL_FreeSurface(surf);
*/	
	lunadata->gamestate = GAMESTATE_MENU;
	RandomInit(lunadata);
	for(int i = 0; i < MAX_BULLETS; i++)
		lunadata->bullets.BulletType[i] = 0;
	SoundInit(lunadata);

	for(int i = 0; i < 256; i++) {
		lunadata->enemies.SinY[0][i] = (int)(sin((i / 256.0) * (M_PI * 2)) * 80 + 88 + 50);
		lunadata->enemies.SinY[1][i] = lunadata->enemies.SinY[0][i];
		lunadata->enemies.SinY[2][i] = lunadata->enemies.SinY[0][i];
		lunadata->enemies.SinX[2][i] = (int)((cos(((i - 1) / 128.0) * (M_PI * 2)) * 64 + 32) - (cos(( i / 128.0) * (M_PI * 2)) * 64 + 32));
		lunadata->enemies.SinY[3][i] = (int)(sin((i / 128.0) * (M_PI * 2)) * 80 + 88 + 50);
		lunadata->enemies.SinX[3][i] = (int)((cos(((i - 1) / 64.0) * (M_PI * 2)) * 64 + 64) - (cos((i / 64.0) * (M_PI * 2)) * 64 + 64));
		lunadata->enemies.SinY[4][i] = (int)(sin((i / 128.0) * (M_PI * 2) * 0.5) * 80 + 88 + 50);
		lunadata->enemies.SinX[4][i] = (int)(((cos(((i - 1) / 64.0) * (M_PI * 2)) * 64 + 64) - (cos((i / 64.0) * (M_PI * 2)) * 64 + 64)) / 2);
		lunadata->enemies.SinX[5][i] = (int)(((cos(((i - 1) / 128.0) * (M_PI * 2)) * 96 + 32) - (cos((i / 128.0) * (M_PI * 2)) * 96 + 32)));
		lunadata->enemies.SinY[6][i] = lunadata->enemies.SinY[0][i];
		lunadata->enemies.SinY[7][i] = lunadata->enemies.SinY[3][i];
		lunadata->enemies.SinX[7][i] = lunadata->enemies.SinX[3][i];
		lunadata->GameOverSinY[i] = (int)(sin((i / 128.0) * (M_PI * 2)) * 0x18 + 0x80);
	}

	sprintf(lunadata->MessageText, "     === LunAmiga V%d.%d === by akmafin in 2020 = thanks to shallan, stepz, furroy and monstersgoboom for the c64 original = movement: WASD, cursor keys or numpad 2468 = fire: right or left CTRL or C", SWVER, SWREV);
	lunadata->MessageLength = StrToDispStr(lunadata->MessageText, sizeof(lunadata->MessageText));

	for(int i = 0; i < 440; i++)
		lunadata->IntroMap[i] = title[i];
}

void GameClean(maindata *lunadata) {
/*MOD
	Mix_HaltMusic();
	if(lunadata->sound.musicgame)
		Mix_FreeMusic(lunadata->sound.musicgame);
	if(lunadata->sound.musicgameover)
		Mix_FreeMusic(lunadata->sound.musicgameover);
	
	for (int i = 0; i < NUM_SOUNDS; i++)
		Mix_FreeChunk(lunadata->sound.mixchunk[i]);

	SDL_DestroyTexture(lunadata->gamefonttex);
	SDL_DestroyTexture(lunadata->gamespritetex);*/

	if(lunadata->MyWindow)
		FreeMem(lunadata->mycharplanes, 40 * 200 * 4);
	if(lunadata->keyioreq != NULL) {
		CloseDevice((struct IORequest*)lunadata->keyioreq);
		AmiDeleteIORequest((struct IORequest*)lunadata->keyioreq);
	}
	if(lunadata->keymp != NULL)
		AmiDeleteMsgPort(lunadata->keymp);

	if(lunadata->MyWindow)
		CloseWindow(lunadata->MyWindow);
	if(lunadata->MyScreen)
		CloseScreen(lunadata->MyScreen);
	if(lunadata->IntuitionBase)
		CloseLibrary(lunadata->IntuitionBase);
	if(lunadata->GraphicsBase)
		CloseLibrary(lunadata->GraphicsBase);
	if(lunadata->DOSBase)
		CloseLibrary(lunadata->DOSBase);
}

void GameDrawScreen(maindata *lunadata) {
//MOD	SDL_Rect chsrc = {0, 0, FONTTILE_WIDTH, FONTTILE_HEIGHT}, chdest = {0, 0, FONTTILE_WIDTH, FONTTILE_HEIGHT};
//MOD	SDL_Rect spsrc = {0, 0, SPRITETILE_WIDTH, SPRITETILE_HEIGHT}, spdest = {0, 0, SPRITETILE_WIDTH, SPRITETILE_HEIGHT};
	int ch, sp, fsx = 72, fsy = 100;
	int chsrcx, chsrcy, chdestx, chdesty;
	static int qq = 0;

fsx=0;
fsy=0;
	for(int y = 0; y < 25; y++) {
		for( int x = 0; x < 40; x++) {
			ch = lunadata->SCREEN[y * 40 + x];
			chsrcx = (ch % 32) * FONTTILE_WIDTH;
			chsrcy = (ch / 32) * FONTTILE_HEIGHT;
			if(lunadata->gamestate == GAMESTATE_MENU) {
				if(((y > 1) && (y < 6)) || ((y > 16) && (y < 25)))
					chdestx = x * FONTTILE_WIDTH + lunadata->map.Hscroll + fsx;
				else
					chdestx = x * FONTTILE_WIDTH + fsx;
			} else {
				if(y < 24)
					chdestx = x * FONTTILE_WIDTH + lunadata->map.Hscroll + fsx;
				else
					chdestx = x * FONTTILE_WIDTH + fsx;
			}
			chdesty = y * FONTTILE_HEIGHT + fsy;
if(chdestx > 312)
	chdestx = 312;
if((qq == 0) || ((x == 39) && ((qq % 8) == 0)))
			BltBitMap(&lunadata->mycharbitmap, chsrcx, chsrcy, lunadata->MyWindow->RPort->BitMap, chdestx, chdesty, FONTTILE_WIDTH, FONTTILE_HEIGHT, 0xC0, 0xFF, NULL);
		}
	}
//if((qq % 8) == 0)
	BltBitMap(lunadata->MyWindow->RPort->BitMap, 0 + (qq % 8), 0, lunadata->MyWindow->RPort->BitMap, 0, 0, 320 - (qq % 8), 200, 0xC0, 0xFF, NULL);
	
/*MOD	ch = 128;
	for(int i = 7; i >= 0; i--) {
		if(lunadata->SPRITE_ENA & ch) {
			sp = lunadata->SPRITE_PTRS[i] - 64;
			if(sp >= 0) {
				spsrc.x = (sp % 8) * SPRITETILE_WIDTH;
				spsrc.y = (sp / 8) * SPRITETILE_HEIGHT;
				spdest.x = (lunadata->SPRITE_X[i] - 0x18) * 2 + fsx;
				spdest.y = (lunadata->SPRITE_Y[i] - 0x32) * 2 + fsy;
				SDL_RenderCopy(lunadata->mainrend, lunadata->gamespritetex, &spsrc, &spdest);
			}
		}
		ch = ch >> 1;
	}
*/	
//qq=1;
}

void GameDelay(maindata *lunadata) {
/*	static std::chrono::high_resolution_clock::time_point last_time;
	std::chrono::high_resolution_clock::time_point time;
	std::chrono::duration<double, std::milli> time_span;

	do {
		time = std::chrono::high_resolution_clock::now();
		time_span = time - last_time;
		if(time_span.count() < 18.0)
			SDL_Delay(1);
	} while(time_span.count() < 20.0);

	time = std::chrono::high_resolution_clock::now();
	last_time = time;*/

	Delay(1);
}

void GameClearScreen(maindata *lunadata) {
	for(int i = 0; i < 1000; i++)
		lunadata->SCREEN[i] = 0;
}

void GameClearColor(struct maindata *lunadata) {
	for(int i = 0; i < 1000; i++)
		lunadata->COLORRAM[i] = 0;
}

void RandomInit(struct maindata *lunadata) {
	lunadata->RandomNum = 0x62;
}

void RandomGet(struct maindata *lunadata) {
	lunadata->RandomNumState ^= lunadata->RandomNumState << 13;
	lunadata->RandomNumState ^= lunadata->RandomNumState >> 17;
	lunadata->RandomNumState ^= lunadata->RandomNumState << 5;
	lunadata->RandomNum = lunadata->RandomNumState & 0xFF;
	if(lunadata->RandomNum > 0x80)
		lunadata->RandomNum -= 0x100;
}

void DrawHighscore(struct maindata *lunadata) {
	for(int i = 0; i < 40; i++)
		lunadata->SCREEN[i] = lunadata->Hiscore[i];
}

void GenerateStars(struct maindata *lunadata) {
}

void DrawStars(struct maindata *lunadata) {
}

void ClearStars(struct maindata *lunadata) {
}

void CheckForHighscore(struct maindata *lunadata) {
	lunadata->HighscoreAchieved = 0;
	
	for(int i = 0; i < 7; i++) {
		if(lunadata->SCREEN[24 * 40 + 7 + i] > lunadata->Hiscore[21 + i])
			lunadata->HighscoreAchieved = 1;
		else if(lunadata->SCREEN[24 * 40 + 7 + i] < lunadata->Hiscore[21 + i])
			break;
	}
	
	if(lunadata->HighscoreAchieved) {
		for(int i = 0; i < 7; i++)
			lunadata->Hiscore[21 + i] = lunadata->SCREEN[24 * 40 + 7 + i];
	}
}

void StopSounds(struct maindata *lunadata) {
}

unsigned char CharToDispChar(unsigned char inchr) {
	unsigned char outchr = 0;

	if ((inchr >= 'a') && (inchr <= 'z'))
		outchr = inchr - 'a' + 1;
	else if ((inchr >= 'A') && (inchr <= 'Z'))
		outchr = inchr - 'A' + 1;
	else if ((inchr >= '0') && (inchr <= '9'))
		outchr = inchr - '0' + 48;
	else if (inchr == ',')
		outchr = 44;
	else if (inchr == '.')
		outchr = 46;
	else if (inchr == '=')
		outchr = 31;
	else if (inchr == ':')
		outchr = 33;

	return outchr;
}

int StrToDispStr(char *str, int outbuflen) {
	int len;

	len = strlen(str);
	if (outbuflen < len)
		len = outbuflen;

	for (int i = 0; i < len; i++) {
		str[i] = CharToDispChar(str[i]);
	}

	return len;
}
