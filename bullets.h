#ifndef bullets_h
#define bullets_h

#define MAX_BULLETS					8

struct bullets {
	int BulletIndex = 0;
	int BulletType[MAX_BULLETS] = {0}, BulletX[MAX_BULLETS] = {0}, BulletY[MAX_BULLETS] = {0};
};

void BulletsInit(struct maindata *lunadata);
void BulletsAdd(struct maindata *lunadata, int x, int y);
void BulletsUpdate(struct maindata *lunadata);
void BulletsClear(struct maindata *lunadata);
void BulletsDraw(struct maindata *lunadata);

#endif
