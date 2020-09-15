#ifndef map_h
#define map_h

struct map {
	int Hscroll = 7, HscrollSpeed = 1, MapCols = 0;
	int CeilHeight = 1, PrevCeilHeight = 1, CeilDir = 0;
	int DecorateType = 0, DecorateTypeChars[4] = {0, 0, 141, 142};
	int FloorHeight = 1, FloorDir = 0, DirRandom = -64;
	int MinGap = 16, MinHeight = 2, NeedToShift = 0;
	int ColData[28] = {0};
	int GrassTop[3] = {130, 129, 128}, CeilTop[3] = {131, 132, 133};
	int HoleData[32] = {137, 138, 139, 140, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135,
		135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135};
	int MapColors[16] = {11, 6, 2, 13, 4, 11, 11, 3,
		8, 14, 10, 9, 3, 7, 12, 2};
};

void MapInit(maindata *lunadata);
void MapGenerateColumn(maindata *lunadata);
void MapAdvanceMap(maindata *lunadata);
void MapScreenShift(maindata *lunadata);
void MapChangeMapColors(maindata *lunadata);

#endif
