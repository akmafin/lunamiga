#ifndef hud_h
#define hud_h

struct hud {
	int Score;
};

void HudInit(struct maindata *lunadata);
void HudAddScore(struct maindata *lunadata, int value);
void HudDisplayWaveNumber(struct maindata *lunadata);
void HudDecPower(struct maindata *lunadata);
void HudAddPower(struct maindata *lunadata);
void HudDrawPower(struct maindata *lunadata);

#endif
