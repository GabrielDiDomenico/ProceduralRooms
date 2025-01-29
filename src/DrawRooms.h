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

	DrawRooms(int***& g, int w, int h, int l);

	void DrawRoomsOnCanvas();
	void DrawRoomsOnFile();

private:
	int*** grid;
	int gridHeight;
	int gridWidth;
	int maxLevels;
};

#endif