#include "CreateRooms.h"


using namespace std;

int*** CreateRooms::GetGrid() {
    return grid;
}

void CreateRooms::SpawnRooms(int nivel, int numOfRooms)
{
    int numberOfTries = 0;
    for (int i = 0; i < numOfRooms; i++)
    {

        int r1 = rand() % numOfRooms * 3 + 20;
        int r2 = rand() % numOfRooms * 3 + 20;

        bool canMake = true;

        int randRoomSizeH = rand() % 10 + 8;
        int randRoomSizeW = rand() % 10 + 8;
        if (randRoomSizeH % 2 != 0)
            randRoomSizeH++;
        if (randRoomSizeW % 2 != 0)
            randRoomSizeW++;
        if (numberOfTries > 100000) {
            numberOfTries = 0;
            continue;
        }

        for (int step = 0; step < maxSteps; step++) {
            for (int d = 0; d < 9; d++) {
                int newRow = r1 + directions[d][0] * (step + 1);
                int newCol = r2 + directions[d][1] * (step + 1);

                if (newRow >= maxRow && newRow < maxRow &&
                    newCol >= maxCol && newCol < maxCol &&
                    i != 0 &&
                    grid[nivel][newRow][newCol] == 1 ||
                    grid[nivel][newRow][newCol] == 2 ||
                    grid[nivel][newRow][newCol] == 3)
                {
                    canMake = false;

                    i--;
                    numberOfTries++;
                    break;
                }
            }
            if (!canMake)
                break;
        }


        if (canMake)
        {
            if (i == 0) {
                node = make_pair(make_pair(make_pair(50, 10), make_pair(5, 5)), 4);

                graph.push_back(node);
            }
            node = make_pair(make_pair(make_pair(r1, r2), make_pair(randRoomSizeW, randRoomSizeH)), 4);

            graph.push_back(node);
            for (int j = -(int)(randRoomSizeW / 2); j < (int)(randRoomSizeW / 2); j++)
            {
                for (int k = -(int)(randRoomSizeH / 2); k < (int)(randRoomSizeH / 2); k++)
                {
                    grid[nivel][j + r1][k + r2] = TileType::Floor;
                }
            }

            for (int j = -(int)(randRoomSizeW / 2); j <= (int)(randRoomSizeW / 2); j++)
            {
                if (j == 0)
                {
                    grid[nivel][j + r1][r2 - (int)(randRoomSizeH / 2)] = TileType::Door;
                    grid[nivel][j + r1][r2 + (int)(randRoomSizeH / 2)] = TileType::Door;
                }
                else
                {
                    grid[nivel][j + r1][r2 - (int)(randRoomSizeH / 2)] = TileType::Wall;
                    grid[nivel][j + r1][r2 + (int)(randRoomSizeH / 2)] = TileType::Wall;
                }

            }

            for (int k = -(int)(randRoomSizeH / 2); k <= (int)(randRoomSizeH / 2); k++)
            {
                if (k == 0)
                {
                    grid[nivel][r1 - (int)(randRoomSizeW / 2)][r2 + k] = TileType::Door;
                    grid[nivel][r1 + (int)(randRoomSizeW / 2)][r2 + k] = TileType::Door;
                }
                else
                {
                    grid[nivel][r1 - (int)(randRoomSizeW / 2)][r2 + k] = TileType::Wall;
                    grid[nivel][r1 + (int)(randRoomSizeW / 2)][r2 + k] = TileType::Wall;
                }

            }
        }
    }
}

void CreateRooms::SpawnStartAndStairs()
{

    for (int i = 0; i < 3; i++) {

        for (int j = -(int)(5 / 2); j < (int)(5 / 2); j++)
        {
            for (int k = -(int)(5 / 2); k < (int)(5 / 2); k++)
            {
                grid[i][j + 50][k + 10] = TileType::Floor;
            }
        }
        for (int j = -(int)(5 / 2); j <= (int)(5 / 2); j++)
        {
            if (j == 0)
            {
                grid[i][j + 50][10 - (int)(5 / 2)] = TileType::Door;
                grid[i][j + 50][10 + (int)(5 / 2)] = TileType::Door;
            }
            else
            {
                grid[i][j + 50][10 - (int)(5 / 2)] = TileType::Wall;
                grid[i][j + 50][10 + (int)(5 / 2)] = TileType::Wall;
            }

        }

        for (int k = -(int)(5 / 2); k <= (int)(5 / 2); k++)
        {
            if (k == 0)
            {
                grid[i][50 - (int)(5 / 2)][10 + k] = TileType::Door;
                grid[i][50 + (int)(5 / 2)][10 + k] = TileType::Door;
            }
            else
            {
                grid[i][50 - (int)(5 / 2)][10 + k] = TileType::Wall;
                grid[i][50 + (int)(5 / 2)][10 + k] = TileType::Wall;
            }
        }
    }



}


void CreateRooms::CleanNotConectedNodes(vector<pair<pair<pair<int, int>, pair<int, int>>, int>>& pGraph, int level)
{

    for (int i = 0; i < pGraph.size(); i++)
    {
        int r1 = pGraph[i].first.first.first;
        int r2 = pGraph[i].first.first.second;
        int roomSizeW = pGraph[i].first.second.first;
        int roomSizeH = pGraph[i].first.second.second;
        if (pGraph[i].second < 4) {
            return;
        }
        for (int j = -(int)(pGraph[i].first.second.first / 2); j <= (int)(pGraph[i].first.second.first / 2); j++)
        {
            for (int k = -(int)(pGraph[i].first.second.second / 2); k <= (int)(pGraph[i].first.second.second / 2); k++)
            {
                grid[level][j + r1][k + r2] = TileType::Empty;
            }
        }

        for (int j = -(int)(roomSizeH / 2); j <= (int)(roomSizeH / 2); j++)
        {


            grid[level][j + r1][r2 + (int)(roomSizeH / 2)] = TileType::Empty;

        }

        for (int k = -(int)(roomSizeW / 2); k <= (int)(roomSizeW / 2); k++)
        {

            grid[level][r1 + (int)(roomSizeW / 2)][r2 + k] = TileType::Empty;

        }

    }
}

void CreateRooms::UpdateDoorsClosedNumber(pair<pair<pair<int, int>, pair<int, int>>, int>& Room, int level)
{
    int numOfOpenedDoors = 4;

    Pair pos = Room.first.first;
    Pair size = Room.first.second;
    if (grid[level][(int)pos.first + (size.first / 2)][pos.second] == TileType::ClosedDoor) {
        numOfOpenedDoors--;
    }
    if (grid[level][pos.first - (size.first / 2)][pos.second] == TileType::ClosedDoor) {
        numOfOpenedDoors--;
    }
    if (grid[level][pos.first][pos.second - (size.second / 2)] == TileType::ClosedDoor) {
        numOfOpenedDoors--;
    }
    if (grid[level][pos.first][pos.second + (size.second / 2)] == TileType::ClosedDoor) {
        numOfOpenedDoors--;
    }

    Room.second = numOfOpenedDoors;
}

void CreateRooms::findAllCorridors(vector<pair<pair<pair<int, int>, pair<int, int>>, int>>& pGraph, Pair nodeToStart, int level)
{

    if (pGraph.size() < 0) {
        return;
    }

    Pair closestNode;

    double lastDistance = 100000000000.0;
    float max_dist = 40.0;

    for (int k = 0; k < pGraph.size(); k++)
    {
        float currDistance = aStar->calculateHValue(nodeToStart.first, nodeToStart.second, pGraph[k].first.first);
        //cout << currDistance << endl;
        if (currDistance > max_dist) {
            continue;
        }
        UpdateDoorsClosedNumber(pGraph[k], level);
        if (pGraph[k].second == 0) {
            pGraph.erase(pGraph.begin() + k);
            continue;
        }
        if (currDistance < lastDistance) {
            closestNode = pGraph[k].first.first;

            lastDistance = currDistance;

            aStar->aStarSearch(grid[level], nodeToStart, closestNode, level);

            pGraph.erase(pGraph.begin() + k);

            findAllCorridors(pGraph, closestNode, level);
        }
    }
}