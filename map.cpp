#include "lunamiga.h"
#include <iostream>
void MapInit(maindata *lunadata) {
	lunadata->map.MapCols = -1;
	MapChangeMapColors(lunadata);
	ClearScreen(lunadata);
	ClearColor(lunadata);
	for(int i = 0; i < 40; i++)
		lunadata->SCREEN[24 * 40 + i] = 0;
	lunadata->map.HscrollSpeed = 1;
	lunadata->map.MinGap = 16;
	lunadata->map.CeilHeight = 0;
	lunadata->map.PrevCeilHeight = 0;
	lunadata->map.FloorHeight = 0;
	lunadata->map.FloorDir = 0;
	lunadata->map.CeilDir = 0;
	lunadata->map.MinHeight = 2;
	lunadata->hud.Score = 0;
}

void MapGenerateColumn(maindata *lunadata) {
	int x = 0, y, mod, ch = 135;

	lunadata->map.MinHeight = 2;
	if(lunadata->enemies.SectorTransition != 0) {
		lunadata->map.MinHeight = 0;
		if(lunadata->map.CeilHeight != 0) {
			y = -1;
			lunadata->map.CeilHeight--;
			if(lunadata->map.CeilHeight < 0)
				lunadata->map.CeilHeight = 0;
			if(lunadata->map.CeilHeight == 0)
				y = 0;
			lunadata->map.CeilDir = y;
		}
		
		if(lunadata->map.FloorHeight != 0) {
			y = -1;
			lunadata->map.FloorHeight--;
			if(lunadata->map.FloorHeight < 0)
				lunadata->map.FloorHeight = 0;
			if(lunadata->map.FloorHeight == 0)
				y = 0;
			lunadata->map.FloorDir = y;
		}
		
		// EndTransitionFadeOut
		if((lunadata->map.FloorHeight + lunadata->map.CeilHeight) == 0) {
			lunadata->enemies.SectorTransition++;
			if(lunadata->enemies.SectorTransition >= 40) {
				MapChangeMapColors(lunadata);
				EnemiesAddWave(lunadata);
			} else {
				x = 1;
			}
		} else
			x = 1;
	}

	if(!x) {
		// Ceiling
		RandomGet(lunadata);
		if(lunadata->RandomNum >= lunadata->map.DirRandom) {
			RandomGet(lunadata);
			if(lunadata->RandomNum >= 0) {
				if(lunadata->map.CeilDir != 1)
					lunadata->map.CeilDir++;
			} else {
				if(lunadata->map.CeilDir != -1)
					lunadata->map.CeilDir--;
			}
			y = 1;
			if(lunadata->map.CeilDir == 1) {
				if((23 - lunadata->map.CeilHeight - lunadata->map.FloorHeight) < lunadata->map.MinGap) {
					lunadata->map.CeilDir = 0;
					y = 0;
				}
			}
			if(y) {
				lunadata->map.CeilHeight += lunadata->map.CeilDir;
				if(lunadata->map.CeilHeight < lunadata->map.MinHeight) {
					lunadata->map.CeilDir = 0;
					lunadata->map.CeilHeight = lunadata->map.MinHeight;
				}
			}
		}

		// Floor
		RandomGet(lunadata);
		if(lunadata->RandomNum >= lunadata->map.DirRandom) {
			RandomGet(lunadata);
			if(lunadata->RandomNum >= 0) {
				if(lunadata->map.FloorDir != 1)
					lunadata->map.FloorDir++;
			} else {
				if(lunadata->map.FloorDir != -1)
					lunadata->map.FloorDir--;
			}
		}
		y = 1;
		if(lunadata->map.FloorDir == 1) {
			if((23 - lunadata->map.CeilHeight - lunadata->map.FloorHeight) < lunadata->map.MinGap) {
				lunadata->map.FloorDir = 0;
				y = 0;
			}
		}
		if(y) {
			lunadata->map.FloorHeight += lunadata->map.FloorDir;
			if(lunadata->map.FloorHeight < lunadata->map.MinHeight) {
				lunadata->map.FloorDir = 0;
				lunadata->map.FloorHeight = lunadata->map.MinHeight;
			}
		}
	}
	
	//Clear column
	for(int i = 0; i < 24; i++)
		lunadata->map.ColData[i] = 0;
	
	// Apply ceil
	x = lunadata->map.CeilHeight - 1;
	while(x >= 0) {
		lunadata->map.ColData[x] = 135;
		x--;
	}
	lunadata->map.ColData[lunadata->map.CeilHeight - 1] = 132;

	// Apply floor
	mod = 24 - lunadata->map.FloorHeight;
	if(lunadata->map.FloorDir == -1)
		mod--;
	// Reset char
	ch = 135;

	// Draw Floor
	x = lunadata->map.FloorHeight;
	if(lunadata->map.FloorDir == -1) {
		x++;
	}
	do {
		if(x == 1) {
			if(lunadata->map.FloorDir == 1) {
				ch = lunadata->map.GrassTop[2];
			} else {
				if(lunadata->map.FloorDir == -1) {
					ch = lunadata->map.GrassTop[0];
				} else {
					ch = 129;
				}
			}
		}
		x--;
		if(x >= 0) {
			lunadata->map.ColData[mod + x] = ch;
		}
	} while(x >= 0);

	// Decorate floor
	x = 0;
	if(lunadata->map.DecorateType != 3) {
		RandomGet(lunadata);
		if(lunadata->RandomNum < 0)
			x = 1;
	} else
		x = 1;
	if(x) {
		RandomGet(lunadata);
		lunadata->map.DecorateType = lunadata->RandomNum & 3;
	}
	if((lunadata->map.FloorHeight != 0) && (lunadata->map.FloorDir == 0)) {
		lunadata->map.ColData[mod - 1] = lunadata->map.DecorateTypeChars[lunadata->map.DecorateType];
	}
	
	// Fix ceiling
	if(lunadata->map.CeilHeight != lunadata->map.PrevCeilHeight) {
		if(lunadata->map.PrevCeilHeight >= lunadata->map.CeilHeight)
			lunadata->map.ColData[lunadata->map.CeilHeight] = lunadata->map.CeilTop[2];
		else
			lunadata->map.ColData[lunadata->map.CeilHeight - 1] = lunadata->map.CeilTop[0];
	}
	lunadata->map.PrevCeilHeight = lunadata->map.CeilHeight;

	// Add random holes and fix the floor slopes
	for(int i = 0; i < 24; i++) {
		if((lunadata->map.ColData[i] == 128) || (lunadata->map.ColData[i] == 130))
			lunadata->map.ColData[i + 1] = lunadata->map.ColData[i] + 6;
		if(lunadata->map.ColData[i] == 135) {
			RandomGet(lunadata);
			if(lunadata->RandomNum >= 0) {
				lunadata->map.ColData[i] = lunadata->map.HoleData[lunadata->RandomNum & 0x1F];
			}
		}
	}
}

void MapAdvanceMap(maindata *lunadata) {
	lunadata->map.NeedToShift = 0;
	lunadata->map.Hscroll -= lunadata->map.HscrollSpeed;
	if(lunadata->map.Hscroll < 0) {
		lunadata->map.Hscroll = lunadata->map.Hscroll & 7;
		MapGenerateColumn(lunadata);
		lunadata->map.NeedToShift = 1;
	}
}

void MapScreenShift(maindata *lunadata) {
	if(lunadata->map.NeedToShift) {
		if(lunadata->gamestate == GAMESTATE_MENU) {
			for(int y = 2; y < 6; y++) {
				for(int x = 1; x < 40; x++)
					lunadata->SCREEN[y * 40 + x - 1] = lunadata->SCREEN[y * 40 + x];
				lunadata->SCREEN[y * 40 + 39] = lunadata->map.ColData[y];
			}
			for(int y = 17; y < 23; y++) {
				for(int x = 1; x < 40; x++)
					lunadata->SCREEN[y * 40 + x - 1] = lunadata->SCREEN[y * 40 + x];
				lunadata->SCREEN[y * 40 + 39] = lunadata->map.ColData[y];
			}
			for(int x = 1; x < 40; x++)
				lunadata->SCREEN[24 * 40 + x - 1] = lunadata->SCREEN[24 * 40 + x];
			
			if(lunadata->MessageIndex >= lunadata->MessageLength)
				lunadata->MessageIndex = 0;
			lunadata->SCREEN[24 * 40 + 39] = lunadata->MessageText[lunadata->MessageIndex];
			lunadata->MessageIndex++;
		} else {
			for(int y = 0; y < 24; y++) {
				for(int x = 1; x < 40; x++)
					lunadata->SCREEN[y * 40 + x - 1] = lunadata->SCREEN[y * 40 + x];
				lunadata->SCREEN[y * 40 + 39] = lunadata->map.ColData[y];
			}
		}
	}
}

void MapChangeMapColors(maindata *lunadata) {
}
