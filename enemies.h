#ifndef enemies_h
#define enemies_h

#define MAX_ENEMIES					6
#define ENEMY_INIT					0
#define ENEMY_UPDATE				3

struct enemies {
	int EnemyX0[MAX_ENEMIES] = {0}, EnemyX1[MAX_ENEMIES] = {0}, EnemyX2[MAX_ENEMIES] = {0};
	int EnemyY0[MAX_ENEMIES] = {0}, EnemyY1[MAX_ENEMIES] = {0};
	int EnemyActive[MAX_ENEMIES] = {0}, EnemyAnimIndex[MAX_ENEMIES] = {0};
	int EnemyDying[MAX_ENEMIES] = {0}, EnemyDeathIndex[MAX_ENEMIES] = {0}, EnemyDeathFrame[MAX_ENEMIES] = {0};
	int CurrentEnemyType = 0, CurrentWaveType = 0;
	int CurrentEnemyCount = 0, CurrentEnemyKillCount = 0, CurrentWaveNumber = 0;
	int EnemyFrame = 0, EnemyFrameIndex = 0, EnemyAnimationTimer[2] = {0, 3};
	int DeathFrames[5] = {68, 69, 70, 71, 67};
	int EnemyStartFrames[6] = {72, 88, 96, 112, 120, 128};
	int EnemyFrameCount[6] = {10, 6, 4, 4, 4, 4};
	int SectorTransition = 0, SinTicker[8] = {0};
	int SinX[8][256] = {{0}}, SinY[8][256] = {{0}};
	int EnemyWaveTypes[MAX_ENEMIES][8] = {
		{1, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 1},
		{0, 1, 1, 0, 1, 0, 0, 0},
		{0, 0, 0, 1, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 1} };
};

void EnemiesInit(struct maindata *lunadata);
void EnemiesAddWave(struct maindata *lunadata);
void EnemiesUpdate(struct maindata *lunadata);
void EnemiesDraw(struct maindata *lunadata);
void EnemiesCheckVsBullets(struct maindata *lunadata, int num);
void EnemiesDoWaveAction(struct maindata *lunadata, int type, int action);

#endif
