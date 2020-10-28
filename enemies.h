#ifndef enemies_h
#define enemies_h

#define MAX_ENEMIES					6
#define ENEMY_INIT					0
#define ENEMY_UPDATE				3

struct enemies {
	int EnemyX0[MAX_ENEMIES], EnemyX1[MAX_ENEMIES], EnemyX2[MAX_ENEMIES];
	int EnemyY0[MAX_ENEMIES], EnemyY1[MAX_ENEMIES];
	int EnemyActive[MAX_ENEMIES], EnemyAnimIndex[MAX_ENEMIES];
	int EnemyDying[MAX_ENEMIES], EnemyDeathIndex[MAX_ENEMIES], EnemyDeathFrame[MAX_ENEMIES];
	int CurrentEnemyType, CurrentWaveType;
	int CurrentEnemyCount, CurrentEnemyKillCount, CurrentWaveNumber;
	int EnemyFrame, EnemyFrameIndex, EnemyAnimationTimer[2];
	int DeathFrames[5];
	int EnemyStartFrames[6];
	int EnemyFrameCount[6];
	int SectorTransition, SinTicker[8];
	int SinX[8][256], SinY[8][256];
	int EnemyWaveTypes[MAX_ENEMIES][8];
};

void EnemiesInit(struct maindata *lunadata);
void EnemiesAddWave(struct maindata *lunadata);
void EnemiesUpdate(struct maindata *lunadata);
void EnemiesDraw(struct maindata *lunadata);
void EnemiesCheckVsBullets(struct maindata *lunadata, int num);
void EnemiesDoWaveAction(struct maindata *lunadata, int type, int action);

#endif
