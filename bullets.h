#ifndef bullets_h
#define bullets_h

#define MAX_BULLETS					8

struct bullets {
	int BulletIndex;
	int BulletType[MAX_BULLETS], BulletX[MAX_BULLETS], BulletY[MAX_BULLETS];
};

void BulletsInit(struct maindata *lunadata);
void BulletsAdd(struct maindata *lunadata, int x, int y);
void BulletsUpdate(struct maindata *lunadata);
void BulletsClear(struct maindata *lunadata);
void BulletsDraw(struct maindata *lunadata);

#endif
