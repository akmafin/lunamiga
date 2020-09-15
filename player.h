#ifndef player_h
#define player_h

#define JOY_UP						1
#define JOY_DOWN					2
#define JOY_LEFT					4
#define JOY_RIGHT					8
#define JOY_FIRE					16

struct player {
	int PlayerFrames[3] = {64, 65, 66};
	int PlayerAnimIndex = 0, PlayerAnimTimer[2] = {0, 4};
	int PlayerX[3] = {0, 0, 0}, PlayerY[2] = {0, 0};
	int PlayerScreenX = 0, PlayerScreenY = 0;
	int PlayerSpeedX[2] = {0, 2}, PlayerSpeedY[2] = {0, 2};
	int PlayerFireTimer[2] = {0, 10};
	int PlayerDrawn = 0, PlayerPower = 0;
	int PowerUpActive = 0, PlayerIsDead = 0;
	int Joy = 0;
};

void PlayerInit(maindata *lunadata);
void PlayerUpdate(maindata *lunadata);
void PlayerGetCollision(maindata *lunadata);
void PlayerDraw(maindata *lunadata);

#endif
