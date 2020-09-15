#ifndef hud_h
#define hud_h

struct hud {
	int HUDDATA[40] = {31, 19, 3, 15, 18, 5, 33, 48, 48, 48,
		48, 48, 48, 48, 31, 16, 15, 23, 5, 18,
		33, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		31, 23, 1, 22, 5, 33, 48, 48, 48, 31};
	int Score = 0;
};

void HudInit(struct maindata *lunadata);
void HudAddScore(struct maindata *lunadata, int value);
void HudDisplayWaveNumber(struct maindata *lunadata);
void HudDecPower(struct maindata *lunadata);
void HudAddPower(struct maindata *lunadata);
void HudDrawPower(struct maindata *lunadata);

#endif
