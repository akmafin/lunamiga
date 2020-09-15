#include "lunamiga.h"

void BulletsInit(struct maindata *lunadata) {
	lunadata->bullets.BulletIndex = 0;
	for(int i = 0; i < MAX_BULLETS; i++) {
		lunadata->bullets.BulletType[i] = 0;
		lunadata->bullets.BulletX[i] = 0;
		lunadata->bullets.BulletY[i] = 0;
	}
}

void BulletsAdd(struct maindata *lunadata, int x, int y) {
	int i;
	
	i = lunadata->bullets.BulletIndex;
	lunadata->bullets.BulletX[i] = x;
	lunadata->bullets.BulletY[i] = y;
	lunadata->bullets.BulletType[i] = 1;

	i++;
	if(i == MAX_BULLETS)
		i = 0;
	lunadata->bullets.BulletIndex = i;
}

void BulletsUpdate(struct maindata *lunadata) {
	for(int i = 0; i < MAX_BULLETS; i++) {
		if(lunadata->bullets.BulletType[i] != 0) {
			lunadata->bullets.BulletX[i]++;
			if(lunadata->bullets.BulletX[i] >= 40)
				lunadata->bullets.BulletType[i] = 0;
		}
	}
}

void BulletsClear(struct maindata *lunadata) {
	for(int i = 0; i < MAX_BULLETS; i++) {
		if(lunadata->bullets.BulletType[i] != 0) {
			lunadata->SCREEN[lunadata->bullets.BulletY[i] * 40 + lunadata->bullets.BulletX[i]] = 0;
		}
	}
}

void BulletsDraw(struct maindata *lunadata) {
	int pos;
	
	for(int i = 0; i < MAX_BULLETS; i++) {
		if(lunadata->bullets.BulletType[i] > 0) {
			pos = lunadata->bullets.BulletY[i] * 40 + lunadata->bullets.BulletX[i];
			if(lunadata->SCREEN[pos] == 0)
				lunadata->SCREEN[pos] = 59;
			else
				lunadata->bullets.BulletType[i] = 0;
		} else if(lunadata->bullets.BulletType[i] < 0) {
			lunadata->bullets.BulletType[i] = 0;
		}
	}
}
