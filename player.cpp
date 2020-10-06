#include "lunamiga.h"
#include <stdlib.h>

void PlayerInit(maindata *lunadata) {
	lunadata->SPRITE_ENA = 0xFD;
	lunadata->SPRITE_PTRS[0] = lunadata->player.PlayerFrames[0];
	lunadata->player.PlayerX[1] = 128;
	lunadata->player.PlayerY[1] = 128;
	lunadata->player.PlayerX[0] = 0;
	lunadata->player.PlayerX[2] = 0;
	lunadata->player.PlayerY[0] = 0;
	
	lunadata->player.PlayerIsDead = 0;
	lunadata->player.PlayerScreenX = 0;
	lunadata->player.PlayerScreenY = 0;
	lunadata->player.PlayerPower = 0;
	lunadata->player.PowerUpActive = 0;
	
	lunadata->player.PlayerDrawn = 8;
	lunadata->player.PlayerFireTimer[0] = 10;
	lunadata->player.PlayerFireTimer[1] = 10;
	
	lunadata->SPRITE_X[0] = 0;
	lunadata->SPRITE_Y[0] = 0;
}

void PlayerUpdate(maindata *lunadata) {
	int pos;

	lunadata->player.PlayerAnimTimer[0]--;
	if(lunadata->player.PlayerAnimTimer[0] < 0) {
		lunadata->player.PlayerAnimTimer[0] = lunadata->player.PlayerAnimTimer[1];
		lunadata->player.PlayerAnimIndex++;
		if(lunadata->player.PlayerAnimIndex == 3)
			lunadata->player.PlayerAnimIndex = 0;
	}

	if(lunadata->player.Joy & JOY_UP) {
		if(lunadata->player.PlayerY[1] >= 0x34) {
			pos = ((lunadata->player.PlayerY[1] << 8) | (lunadata->player.PlayerY[0])) - ((lunadata->player.PlayerSpeedY[1] << 8) | (lunadata->player.PlayerSpeedY[0]));
			lunadata->player.PlayerY[0] = pos & 0xFF;
			lunadata->player.PlayerY[1] = pos >> 8;
		}
	}

	if(lunadata->player.Joy & JOY_DOWN) {
		if(lunadata->player.PlayerY[1] < 0xDE) {
			pos = ((lunadata->player.PlayerY[1] << 8) | (lunadata->player.PlayerY[0])) + ((lunadata->player.PlayerSpeedY[1] << 8) | (lunadata->player.PlayerSpeedY[0]));
			lunadata->player.PlayerY[0] = pos & 0xFF;
			lunadata->player.PlayerY[1] = pos >> 8;
		}
	}

	if(lunadata->player.Joy & JOY_LEFT) {
		if((lunadata->player.PlayerX[2]) || ((lunadata->player.PlayerX[2] == 0) && (lunadata->player.PlayerX[1] >= 0x24))) {
			pos = ((lunadata->player.PlayerX[2] << 16) | (lunadata->player.PlayerX[1] << 8) | (lunadata->player.PlayerX[0])) - ((lunadata->player.PlayerSpeedX[1] << 8) | (lunadata->player.PlayerSpeedX[0]));
			lunadata->player.PlayerX[0] = pos & 0xFF;
			lunadata->player.PlayerX[1] = (pos >> 8) & 0xFF;
			lunadata->player.PlayerX[2] = pos >> 16;
		}
	}

	if(lunadata->player.Joy & JOY_RIGHT) {
		if((lunadata->player.PlayerX[2] == 0) || ((lunadata->player.PlayerX[2]) && (lunadata->player.PlayerX[1] < 0x36))) {
			pos = ((lunadata->player.PlayerX[2] << 16) | (lunadata->player.PlayerX[1] << 8) | (lunadata->player.PlayerX[0])) + ((lunadata->player.PlayerSpeedX[1] << 8) | (lunadata->player.PlayerSpeedX[0]));
			lunadata->player.PlayerX[0] = pos & 0xFF;
			lunadata->player.PlayerX[1] = (pos >> 8) & 0xFF;
			lunadata->player.PlayerX[2] = pos >> 16;
		}
	}

	if(lunadata->player.Joy & JOY_FIRE) {
		if(lunadata->player.PlayerFireTimer[0] == 0) {
			lunadata->player.PlayerFireTimer[0] = lunadata->player.PlayerFireTimer[1];
			BulletsAdd(lunadata, lunadata->player.PlayerScreenX + 3, lunadata->player.PlayerScreenY + 1);
			SoundSfxShoot(lunadata);
		}
		else
			lunadata->player.PlayerFireTimer[0]--;
	}

	PlayerGetCollision(lunadata);
}

void PlayerGetCollision(maindata *lunadata) {
	int ch, x, y;
	
	y = lunadata->player.PlayerY[1] - 0x34;
	y = y >> 3;
	if(y < 0)
		y = 0;
	if(y >= 0x1A)
		y = 0;
	lunadata->player.PlayerScreenY = y;

	x = ((lunadata->player.PlayerX[2] << 8) | lunadata->player.PlayerX[1]) - 0x1C - lunadata->map.Hscroll;
	x = x >> 3;
	if(x < 0)
		x = 0;
	if(x >= 39)
		x = 0;
	lunadata->player.PlayerScreenX = x;
	
	x = lunadata->player.PlayerScreenX + 41;
	y = lunadata->player.PlayerScreenY * 40;
	ch = lunadata->SCREEN[y + x];
	ch = ch | (lunadata->SCREEN[y + x + 1]);
	x += 39;
	ch = ch | (lunadata->SCREEN[y + x]);
	ch = ch | (lunadata->SCREEN[y + x + 1]);

	if(ch > 0) {
		lunadata->player.PlayerIsDead = 1;
		SoundSfxScream(lunadata);
	} else {
		for(int i = 0; i < MAX_ENEMIES; i++) {
			if((lunadata->enemies.EnemyActive[i]) && ((abs(lunadata->SPRITE_X[0] - lunadata->SPRITE_X[i + 2]) < 24) && (abs(lunadata->SPRITE_Y[0] - lunadata->SPRITE_Y[i + 2]) < 21))) {
				lunadata->player.PlayerIsDead = 1;
				SoundSfxScream(lunadata);
				break;
			}
		}
	}
}

void PlayerDraw(maindata *lunadata) {
	lunadata->SPRITE_Y[0] = lunadata->player.PlayerY[1];
	lunadata->SPRITE_X[0] = (lunadata->player.PlayerX[2] << 8) | lunadata->player.PlayerX[1];
	lunadata->SPRITE_PTRS[0] = lunadata->player.PlayerFrames[lunadata->player.PlayerAnimIndex];
	if(lunadata->player.PlayerDrawn != 0)
		lunadata->player.PlayerDrawn --;
}
