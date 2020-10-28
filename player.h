#ifndef player_h
#define player_h

#define JOY_UP						1
#define JOY_DOWN					2
#define JOY_LEFT					4
#define JOY_RIGHT					8
#define JOY_FIRE					16

struct player {
	int PlayerFrames[3];
	int PlayerAnimIndex, PlayerAnimTimer[2];
	int PlayerX[3], PlayerY[2];
	int PlayerScreenX, PlayerScreenY;
	int PlayerSpeedX[2], PlayerSpeedY[2];
	int PlayerFireTimer[2];
	int PlayerDrawn, PlayerPower;
	int PowerUpActive, PlayerIsDead;
	int Joy;
};

void PlayerInit(maindata *lunadata);
void PlayerUpdate(maindata *lunadata);
void PlayerGetCollision(maindata *lunadata);
void PlayerDraw(maindata *lunadata);

#endif
