#ifndef __ASTARSEARCH__H__
#define __ASTARSEARCH__H__


#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>  
#include <vector>
#include "float.h"

//Para incluir dar quickfix e adicionar isso C:\MinGW\lib\gcc\mingw32\6.3.0\include\c++\mingw32
#include <stack>
#include <set>
#include<list>
#include "TileTypeEnum.h"
#include "AstarSearch.h"
#include "TileTypeEnum.h"
using namespace std;

// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;

class AstarSearch
{
public:
	

	AstarSearch(int maxR, int maxC, int*** &g) {
		maxRow = maxR;
		maxCol = maxC;
		grid = g;
	}
	//I used goto in the function, if it is in the language it is meant to be used :)
	//All jokes aside I don't know the risks of using it, but it works
	void aStarSearch(int** grid, Pair src, Pair dest, int level);
	// A Utility Function to calculate the 'h' heuristics.
	float calculateHValue(int row, int col, Pair dest);
private:
	
	int maxRow;
	int maxCol;
	int*** grid;


	// A structure to hold the necessary parameters
	struct cell {

		int parent_i, parent_j;
		// f = g + h
		double f, g, h;
	};
	// A Utility Function to check whether given cell (row, col)
	// is a valid cell or not.
	bool isValid(int row, int col);
	// A Utility Function to check whether the given cell is
	// blocked or not
	bool isUnBlocked(int** grid, int row, int col);
	// A Utility Function to check whether destination cell has
	// been reached or not
	bool isDestination(int row, int col, Pair dest);
	//For each pair in the stack corresponding a tile on the grid that
	//is a corridor is added in the grid
	void tracePath(cell** cellDetails, Pair dest, int level);
	
};

#endif