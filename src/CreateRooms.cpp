/**
*
*   Autor: Gabriel Di Domenico
*
*   gddomenico@inf.ufsm.br
*
**/

#include "CreateRooms.h"

using namespace std;

CreateRooms::CreateRooms(int maxR, int maxC, int numRooms, int numOfLevels) {
    maxLevels = numOfLevels;
    maxRooms = numRooms;
    maxRow = maxR;
    maxCol = maxC;
    grid = (int***)malloc(numOfLevels * sizeof(int**));

    for (int i = 0; i < numOfLevels; i++) {
        grid[i] = (int**)malloc(maxRow * sizeof(int*));
        grid[i][0] = (int*)malloc(maxRow * maxCol * sizeof(int));
        for (int j = 0; j < maxRow; j++)
            grid[i][j] = grid[i][0] + maxCol * j;
    }
    aStar = new AstarSearch(maxR, maxC, grid);
    srand(time(NULL));
    ClearGrid();
    InitCreation();
}

void CreateRooms::InitCreation() {
    SpawnStartAndStairs();
    for (int i = 0; i < maxLevels; i++)
    {
        SpawnRooms(i, maxRooms);
        Graphs.push_back(graph);
        findAllCorridors(Graphs[i], Graphs[i][0].first.first, i);
        CleanNotConectedNodes(Graphs[i], i);
        graph.clear();
    }
}

void CreateRooms::ClearGrid() {
    for (int k = 0; k < maxLevels; k++)
        for (int i = 0; i < maxRow; i++)
            for (int j = 0; j < maxCol; j++)
                grid[k][i][j] = 0;
    Graphs.clear();
}

int*** CreateRooms::GetGrid() {
    return grid;
}

void CreateRooms::SpawnRooms(int nivel, int numOfRooms) {
    int numberOfTries = 0;

    for (int i = 0; i < numOfRooms; i++) {
        if (numberOfTries > 100000) {
            numberOfTries = 0;
            continue;
        }
        int roomsMultiplier = 1;
        // Geração aleatória de posição e tamanho
        if (numOfRooms < 100) {
            roomsMultiplier = 4;
        }
        else if (numOfRooms < 300) {
            roomsMultiplier = 2;
        }
        else {
            roomsMultiplier = 1;
        }
        int r1 = rand() % (numOfRooms* roomsMultiplier) + 20;
        int r2 = rand() % (numOfRooms* roomsMultiplier) + 20;
        int roomWidth = (rand() % 10 + 8) | 1;  // Garante tamanho par
        int roomHeight = (rand() % 10 + 8) | 1;

        if (!CanPlaceRoom(nivel, r1, r2, roomWidth, roomHeight)) {
            i--;
            numberOfTries++;
            continue;
        }

        // Criação da sala e adição ao grafo
        if (i == 0) {
            graph.emplace_back(std::make_pair(std::make_pair(50, 10), std::make_pair(5, 5)), 4);
        }

        graph.emplace_back(std::make_pair(std::make_pair(r1, r2), std::make_pair(roomWidth, roomHeight)), 4);
        PlaceRoom(nivel, r1, r2, roomWidth, roomHeight);
    }
}

// Verifica se há espaço disponível para a sala
bool CreateRooms::CanPlaceRoom(int nivel, int r1, int r2, int width, int height) {
    for (int step = 0; step < maxSteps; step++) {
        for (int d = 0; d < 9; d++) {
            int newRow = r1 + directions[d][0] * (step + 1);
            int newCol = r2 + directions[d][1] * (step + 1);

            if (newRow < 0 || newRow >= maxRow || newCol < 0 || newCol >= maxCol)
                return false;

            if (grid[nivel][newRow][newCol] == TileType::Floor ||
                grid[nivel][newRow][newCol] == TileType::Door ||
                grid[nivel][newRow][newCol] == TileType::Wall) {
                return false;
            }
        }
    }
    return true;
}

// Posiciona a sala na grade e define paredes/portas
void CreateRooms::PlaceRoom(int nivel, int r1, int r2, int width, int height) {
    // Preenche a sala com chão
    for (int j = -width / 2; j < width / 2; j++) {
        for (int k = -height / 2; k < height / 2; k++) {
            grid[nivel][r1 + j][r2 + k] = TileType::Floor;
        }
    }

    // Cria paredes e portas
    for (int j = -width / 2; j <= width / 2; j++) {
        grid[nivel][r1 + j][r2 - height / 2] = (j == 0) ? TileType::Door : TileType::Wall;
        grid[nivel][r1 + j][r2 + height / 2] = (j == 0) ? TileType::Door : TileType::Wall;
    }
    for (int k = -height / 2; k <= height / 2; k++) {
        grid[nivel][r1 - width / 2][r2 + k] = (k == 0) ? TileType::Door : TileType::Wall;
        grid[nivel][r1 + width / 2][r2 + k] = (k == 0) ? TileType::Door : TileType::Wall;
    }
}

void CreateRooms::SpawnStartAndStairs()
{
    for (int i = 0; i < maxLevels; i++) {

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