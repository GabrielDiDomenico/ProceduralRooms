#ifndef __DRAWROOMS__H__
#define __DRAWROOMS__H__

#include "gl_canvas2d.h"

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

class DrawRooms {

public:
	int levelToShow;
	int zoom;
	int shiftUpDown;
	int shiftRightLeft;

	DrawRooms(int***& g, int w, int h, int l, int z);

	//Draw the dungeon on the Canvas2D
	void DrawRoomsOnCanvas();
	//Save each level on a different text file
	void DrawRoomsOnFile();

private:
	int*** grid;
	int gridHeight;
	int gridWidth;
	int maxLevels;
};

#endif