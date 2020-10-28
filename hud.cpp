#include "lunamiga.h"
#include <stdio.h>

void HudInit(struct maindata *lunadata) {
	int HUDDATA[40] = {31, 19, 3, 15, 18, 5, 33, 48, 48, 48,
		48, 48, 48, 48, 31, 16, 15, 23, 5, 18,
		33, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		31, 23, 1, 22, 5, 33, 48, 48, 48, 31};
		
	for(int i = 0; i < 40; i++) {
		lunadata->SCREEN[24 * 40 + i] = HUDDATA[i];
	}
}

void HudAddScore(struct maindata *lunadata, int value) {
	char buf[8];

	if(lunadata->player.PlayerIsDead == 0) {
		lunadata->hud.Score += value;
		snprintf(buf, 8, "%07d", lunadata->hud.Score);
		for(int i = 0; i < 7; i++)
			lunadata->SCREEN[24 * 40 + 7 + i] = buf[i];
	}
}

void HudDisplayWaveNumber(struct maindata *lunadata) {
	char buf[4];
	
	snprintf(buf, 4, "%03d", lunadata->enemies.CurrentWaveNumber);
	for(int i = 0; i < 3; i++)
		lunadata->SCREEN[24 * 40 + 36 + i] = buf[i];
}

void HudDecPower(struct maindata *lunadata) {
	lunadata->player.PlayerPower--;
	if(lunadata->player.PlayerPower == 0) {
		lunadata->player.PlayerFireTimer[1] = 10;
		lunadata->player.PowerUpActive = 0;
	}
	HudDrawPower(lunadata);
}

void HudAddPower(struct maindata *lunadata) {
	lunadata->player.PlayerPower++;
	if(lunadata->player.PlayerPower >= 36) {
		lunadata->player.PlayerPower = 36;
		lunadata->player.PowerUpActive = 1;
		lunadata->player.PlayerFireTimer[1] = 5;
	}
	HudDrawPower(lunadata);
}

void HudDrawPower(struct maindata *lunadata) {
	int power;
	
	power = lunadata->player.PlayerPower;
	for(int i = 0; i < 9; i++) {
		if(power <= 0) {
			lunadata->SCREEN[24 * 40 + 21 + i] = 0;
			power = 0;
		} else if (power >= 4) {
			lunadata->SCREEN[24 * 40 + 21 + i] = 30;
		} else {
			lunadata->SCREEN[24 * 40 + 21 + i] = 26 + power;
		}
		
		power -= 4;
	}
}
