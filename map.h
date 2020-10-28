#ifndef map_h
#define map_h

struct map {
	int Hscroll, HscrollSpeed, MapCols;
	int CeilHeight, PrevCeilHeight, CeilDir;
	int DecorateType, DecorateTypeChars[4];
	int FloorHeight, FloorDir, DirRandom;
	int MinGap, MinHeight, NeedToShift;
	int ColData[28];
	int GrassTop[3], CeilTop[3];
	int HoleData[32];
	int MapColors[16];
};

void MapInit(maindata *lunadata);
void MapGenerateColumn(maindata *lunadata);
void MapAdvanceMap(maindata *lunadata);
void MapScreenShift(maindata *lunadata);
void MapChangeMapColors(maindata *lunadata);

#endif
