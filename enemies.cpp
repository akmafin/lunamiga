#include "lunamiga.h"
#include <math.h>
//#include <iostream>
void EnemiesInit(struct maindata *lunadata) {
	lunadata->enemies.CurrentEnemyCount = 0;
	lunadata->enemies.CurrentWaveNumber = 0;
	for(int i = 0; i < MAX_ENEMIES; i++) {
		lunadata->enemies.EnemyX0[i] = 0;
		lunadata->enemies.EnemyX1[i] = 0;
		lunadata->enemies.EnemyX2[i] = 0;
		lunadata->enemies.EnemyY0[i] = 0;
		lunadata->enemies.EnemyY1[i] = 0;
		lunadata->enemies.EnemyActive[i] = 0;
		lunadata->enemies.EnemyAnimIndex[i] = 0;
		lunadata->enemies.EnemyDying[i] = 0;
		lunadata->enemies.EnemyDeathIndex[i] = 0;
		lunadata->enemies.EnemyDeathFrame[i] = 0;
	};
	lunadata->enemies.SectorTransition = 0x20;
	lunadata->enemies.CurrentWaveType = 0;
}

void EnemiesAddWave(struct maindata *lunadata) {
	int gap, sp;

	if((lunadata->enemies.SectorTransition == 0) || (lunadata->enemies.SectorTransition >= 40)) {
		lunadata->enemies.SectorTransition = 0;
		lunadata->enemies.CurrentWaveNumber++;
		HudDisplayWaveNumber(lunadata);

		gap = lunadata->enemies.CurrentWaveNumber / 8;
		if(gap > 13)
			gap = 13;
		lunadata->map.MinGap = 17 - gap;
		
		sp = (lunadata->enemies.CurrentWaveNumber / 4) + 1;
		if(sp > 7)
			sp = 7;
		lunadata->map.HscrollSpeed = sp;
		
		lunadata->enemies.EnemyFrameIndex = 0;
		lunadata->enemies.CurrentEnemyCount = MAX_ENEMIES;
		lunadata->enemies.CurrentEnemyKillCount = 0;
		do {
			RandomGet(lunadata);
		} while ((lunadata->RandomNum & 7) >= 6);
		lunadata->enemies.CurrentEnemyType = lunadata->RandomNum & 7;

		do {
			RandomGet(lunadata);
			lunadata->enemies.CurrentWaveType = (lunadata->RandomNum & 7) + 1;
		} while (lunadata->enemies.EnemyWaveTypes[lunadata->enemies.CurrentEnemyType][lunadata->enemies.CurrentWaveType] == 0);
/*lunadata->enemies.CurrentWaveType++;
if(lunadata->enemies.CurrentWaveType > 8)
	lunadata->enemies.CurrentWaveType = 1;
std::cout << "Enemy wave type: " << lunadata->enemies.CurrentWaveType << std::endl;*/
		EnemiesDoWaveAction(lunadata, lunadata->enemies.CurrentWaveType, ENEMY_INIT);
		
		for(int i = 0; i < MAX_ENEMIES; i++) {
			lunadata->enemies.EnemyActive[i] = 1;
			lunadata->enemies.EnemyDying[i] = 0;
			lunadata->enemies.EnemyDeathIndex[i] = 0;
		}
	}
}

void EnemiesUpdate(struct maindata *lunadata) {
	int pos;
	
	lunadata->enemies.EnemyAnimationTimer[0]--;
	if(lunadata->enemies.EnemyAnimationTimer[0] < 0) {
		lunadata->enemies.EnemyAnimationTimer[0] = lunadata->enemies.EnemyAnimationTimer[1];
		lunadata->enemies.EnemyFrameIndex++;
		if(lunadata->enemies.EnemyFrameIndex == lunadata->enemies.EnemyFrameCount[lunadata->enemies.CurrentEnemyType])
			lunadata->enemies.EnemyFrameIndex = 0;
		lunadata->enemies.EnemyFrame = lunadata->enemies.EnemyStartFrames[lunadata->enemies.CurrentEnemyType] + lunadata->enemies.EnemyFrameIndex;
	}
	
	for(int i = 0; i < MAX_ENEMIES; i++) {
		if(lunadata->enemies.EnemyActive[i] != 0) {
			if(lunadata->enemies.EnemyDying[i] != 0) {
				lunadata->enemies.EnemyDeathFrame[i] = lunadata->enemies.DeathFrames[lunadata->enemies.EnemyDeathIndex[i]];
				lunadata->enemies.EnemyDeathIndex[i]++;
				if(lunadata->enemies.EnemyDeathIndex[i] == 5) {
					lunadata->enemies.EnemyDeathIndex[i] = 4;
					lunadata->enemies.EnemyActive[i] = 0;
					lunadata->enemies.CurrentEnemyCount--;
				}
			} else {
				pos = ((lunadata->enemies.EnemyX2[i] << 8) | (lunadata->enemies.EnemyX1[i])) - 2;
				lunadata->enemies.EnemyX1[i] = pos & 0xFF;
				lunadata->enemies.EnemyX2[i] = pos >> 8;
				if(pos < 16) {
					lunadata->enemies.EnemyActive[i] = 0;
					lunadata->enemies.CurrentEnemyCount--;
				}
			}
		}
	}
	
	if(lunadata->enemies.CurrentWaveType != 0)
		EnemiesDoWaveAction(lunadata, lunadata->enemies.CurrentWaveType, ENEMY_UPDATE);
	
	if(lunadata->enemies.CurrentEnemyCount <= 0) {
		if(lunadata->enemies.SectorTransition == 0) {
			if((lunadata->enemies.CurrentWaveNumber & 15) == 8)
				lunadata->enemies.SectorTransition++;
			else
				EnemiesAddWave(lunadata);
		}
	}
}

void EnemiesDraw(struct maindata *lunadata) {
	for(int i = 0; i < MAX_ENEMIES; i++) {
		if(lunadata->enemies.EnemyActive[i] == 1) {
			if(lunadata->enemies.EnemyDying[i] == 0)
				lunadata->SPRITE_PTRS[i + 2] = lunadata->enemies.EnemyFrame;
			else
				lunadata->SPRITE_PTRS[i + 2] = lunadata->enemies.EnemyDeathFrame[i];
			
			lunadata->SPRITE_X[i + 2] = lunadata->enemies.EnemyX1[i] | (lunadata->enemies.EnemyX2[i] << 8);
			lunadata->SPRITE_Y[i + 2] = lunadata->enemies.EnemyY1[i];
			
			EnemiesCheckVsBullets(lunadata, i);
		} else {
			lunadata->SPRITE_X[i + 2] = 0;
			lunadata->SPRITE_Y[i + 2] = 0;
		}
	}
}

void EnemiesCheckVsBullets(struct maindata *lunadata, int num) {
	int x, y, enemyscry, enemyscrx;

	if(lunadata->enemies.EnemyDying[num] == 0) {
		enemyscry = (lunadata->enemies.EnemyY1[num] - 0x32) / 8;
		enemyscrx = ((lunadata->enemies.EnemyX1[num] | (lunadata->enemies.EnemyX2[num] << 8)) - 0x18) / 8;
		for(int i = 0; i < MAX_BULLETS; i++) {
			if(lunadata->bullets.BulletType[i] != 0) {
				y = lunadata->bullets.BulletY[i] - enemyscry;
				if((y >= 0) && (y < 3)) {
					x = lunadata->bullets.BulletX[i] - enemyscrx;
					if((x >= 0) && (x < 3)) {
						lunadata->bullets.BulletType[i] = -1;
						lunadata->enemies.EnemyDying[num] = 1;
						lunadata->enemies.EnemyDeathIndex[num] = 0;
						SoundSfxExplode(lunadata);
						HudAddScore(lunadata, lunadata->enemies.CurrentWaveNumber / 4 + 1);
						HudAddPower(lunadata);
						lunadata->enemies.CurrentEnemyKillCount++;
						if(lunadata->enemies.CurrentEnemyKillCount == MAX_ENEMIES) {
							SoundSfxBonus(lunadata);
							HudAddScore(lunadata, (lunadata->enemies.CurrentWaveNumber / 4 + 1) * 10);
						}
					}
				}
			}
		}
	}
}

void EnemiesDoWaveAction(struct maindata *lunadata, int type, int action) {
	int j, pos;

	type--;
	if(action == ENEMY_INIT) {
		switch(type) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			default:
				for(int i = 0; i < MAX_ENEMIES; i++) {
					lunadata->enemies.EnemyX1[i] = 0x60 + i * 0x10;
					lunadata->enemies.EnemyY1[i] = 0x38 + i * 0x18;
					lunadata->enemies.EnemyX2[i] = 1;
				}
				break;
			case 5:
			case 6:
			case 7:
				for(int i = 0; i < MAX_ENEMIES; i++) {
					RandomGet(lunadata);
					lunadata->enemies.EnemyX1[i] = 0x50 + (lunadata->RandomNum & 0x7F);
					RandomGet(lunadata);
					lunadata->enemies.EnemyY1[i] = 0x40 + (lunadata->RandomNum & 0x7F);
					lunadata->enemies.EnemyX2[i] = 1;
				}
				break;
		}
	} else {
		switch(type) {
			default:
			case 0:
				lunadata->enemies.SinTicker[type] = (lunadata->enemies.SinTicker[type] + 1) & 0xFF;
				for(int i = 0; i < MAX_ENEMIES; i++) {
					if(lunadata->enemies.EnemyDying[i] == 0) {
						j = (i * 16 + lunadata->enemies.SinTicker[type]) & 0xFF;
						lunadata->enemies.EnemyY1[i] = lunadata->enemies.SinY[type][j];
					}
				}
				break;
			case 1:
				lunadata->enemies.SinTicker[type] = (lunadata->enemies.SinTicker[type] + 1) & 0xFF;
				for(int i = 0; i < MAX_ENEMIES; i++) {
					if(lunadata->enemies.EnemyDying[i] == 0) {
						j = (i * 64 + lunadata->enemies.SinTicker[type]) & 0xFF;
						lunadata->enemies.EnemyY1[i] = lunadata->enemies.SinY[type][j];
					}
				}
				break;
			case 2:
				lunadata->enemies.SinTicker[type] = (lunadata->enemies.SinTicker[type] + 1) & 0xFF;
				for(int i = 0; i < MAX_ENEMIES; i++) {
					if(lunadata->enemies.EnemyDying[i] == 0) {
						j = (i * 16 + lunadata->enemies.SinTicker[type]) & 0xFF;
						lunadata->enemies.EnemyY1[i] = lunadata->enemies.SinY[type][j];
						pos = ((lunadata->enemies.EnemyX2[i] << 8) | (lunadata->enemies.EnemyX1[i])) + lunadata->enemies.SinX[type][j];
						lunadata->enemies.EnemyX1[i] = pos & 0xFF;
						lunadata->enemies.EnemyX2[i] = pos >> 8;
					}
				}
				break;
			case 3:
			case 4:
			case 7:
				lunadata->enemies.SinTicker[type] = (lunadata->enemies.SinTicker[type] + 1) & 0xFF;
				for(int i = 0; i < MAX_ENEMIES; i++) {
					if(lunadata->enemies.EnemyDying[i] == 0) {
						j = (i * 8 + lunadata->enemies.SinTicker[type]) & 0xFF;
						lunadata->enemies.EnemyY1[i] = lunadata->enemies.SinY[type][j];
						pos = ((lunadata->enemies.EnemyX2[i] << 8) | (lunadata->enemies.EnemyX1[i])) + lunadata->enemies.SinX[type][j];
						lunadata->enemies.EnemyX1[i] = pos & 0xFF;
						lunadata->enemies.EnemyX2[i] = pos >> 8;
					}
				}
				break;
			case 5:
				lunadata->enemies.SinTicker[type] = (lunadata->enemies.SinTicker[type] + 1) & 0xFF;
				for(int i = 0; i < MAX_ENEMIES; i++) {
					if(lunadata->enemies.EnemyDying[i] == 0) {
						j = (i * 16 + lunadata->enemies.SinTicker[type]) & 0xFF;
						pos = ((lunadata->enemies.EnemyX2[i] << 8) | (lunadata->enemies.EnemyX1[i])) + lunadata->enemies.SinX[type][j];
						lunadata->enemies.EnemyX1[i] = pos & 0xFF;
						lunadata->enemies.EnemyX2[i] = pos >> 8;
					}
				}
				break;
			case 6:
				lunadata->enemies.SinTicker[type] = (lunadata->enemies.SinTicker[type] + 1) & 0xFF;
				for(int i = 0; i < MAX_ENEMIES; i++) {
					if(lunadata->enemies.EnemyDying[i] == 0) {
						j = (i * 16 + lunadata->enemies.SinTicker[type]) & 0xFF;
						lunadata->enemies.EnemyY1[i] = lunadata->enemies.SinY[type][j];
						pos = ((lunadata->enemies.EnemyX2[i] << 8) | (lunadata->enemies.EnemyX1[i])) - 2;
						lunadata->enemies.EnemyX1[i] = pos & 0xFF;
						lunadata->enemies.EnemyX2[i] = pos >> 8;
					}
				}
				break;
		}
	}
}
