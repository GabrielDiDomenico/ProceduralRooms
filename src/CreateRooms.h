#ifndef __CREATEROOMS__H__
#define __CREATEROOMS__H__

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>  
#include <vector>

//Para incluir dar quickfix e adicionar isso C:\MinGW\lib\gcc\mingw32\6.3.0\include\c++\mingw32
#include <stack>
#include <set>
#include<list>
#include "TileTypeEnum.h"
#include "AstarSearch.h"

using namespace std;

class CreateRooms {

public:
	int*** grid;
	vector<vector<pair<pair<pair<int, int>, pair<int, int>>, int>>> Graphs;

	/*
	Constructor for allocating the memory for the grid and to start the creation
	of the rooms and the corridors.
	
	It starts by creating the first room that will be in the same location for all
	levels.

	For each level it will create the numbers os rooms randomly, each room is saved
	in a vector, and added to another one to store all levels.

	After the creation of the room the corridors are made, using A* search.

	When it's finished any rooms that could not be conected is removed from the grid.
	*/
	CreateRooms(int maxR, int maxC, int numRooms, int numOfLevels);

	int*** GetGrid();
	void ClearGrid();
	void InitCreation();
private:

	
	vector<pair<pair<pair<int, int>, pair<int, int>>, int>> graph;
	// pair(pair(positionXY,roomSizeHW),numberOfOpenedDoors)
	pair<pair<pair<int, int>, pair<int, int>>, int> node;
	
	
	int directions[9][2] = { {0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, 1}, {1, -1}, {-1, 1} };
	int maxSteps = 10;

	list<int> indexToRemove;
	int maxRow;
	int maxCol;
	int maxLevels;
	int maxRooms;
	AstarSearch* aStar;

	//Create randomly rooms, saving in pairs with position information (X,Y) and size (W,H)
	void SpawnRooms(int nivel, int numOfRooms);
	//Create the first room for each level
	void SpawnStartAndStairs();
	//Remove all rooms that were not conected
	void CleanNotConectedNodes(vector<pair<pair<pair<int, int>, pair<int, int>>, int>>& pGraph, int level);
	//An auxiliary funtion to count the number of doors conected to a corridor, this prevent two corridor
	//in the same door
	void UpdateDoorsClosedNumber(pair<pair<pair<int, int>, pair<int, int>>, int>& Room, int level);
	//Function that calculate the distance between the rooms and call the A* search to trace the corridor
	void findAllCorridors(vector<pair<pair<pair<int, int>, pair<int, int>>, int>>& pGraph, Pair nodeToStart, int level);
	
};

#endif
