/**
*
*   Autor: Gabriel Di Domenico
*
*   gddomenico@inf.ufsm.br
*
**/

#include "CreateRooms.h"

using namespace std;

CreateRooms::CreateRooms(int maxR, int maxC, int numRooms, int numOfLevels, int lEnemy, int hEnemy, int lTreasure, int hTreasure) {
    gen = std::mt19937(std::random_device{}());
    maxLevels = numOfLevels;
    maxRooms = numRooms;
    maxRow = maxR;
    maxCol = maxC;
	lowEnemy = lEnemy;
	highEnemy = hEnemy;
	lowTreasure = lTreasure;
	highTreasure = hTreasure;
    grid = (int***)malloc(numOfLevels * sizeof(int**));

    for (int i = 0; i < numOfLevels; i++) {
        grid[i] = (int**)malloc(maxRow * sizeof(int*));
        grid[i][0] = (int*)malloc(maxRow * maxCol * sizeof(int));
        for (int j = 0; j < maxRow; j++)
            grid[i][j] = grid[i][0] + maxCol * j;
    }
    aStar = new AstarSearch(maxR, maxC, grid);
    tresurePosList.resize(maxLevels);
    enemyPosList.resize(maxLevels);
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
        SpawnEntities(Graphs[i], i);
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
            if (grid[nivel][r1 + j][r2 + k] == TileType::Treasure || grid[nivel][r1 + j][r2 + k] == TileType::Enemy)
            {
                continue;
            }
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


void CreateRooms::CleanNotConectedNodes(vector<pair<pair<pair<int, int>, pair<int, int>>, int>>& pGraph, int level) {

    for (const auto& room : pGraph) {
        int r1 = room.first.first.first;
        int r2 = room.first.first.second;
        int halfW = room.first.second.first / 2;
        int halfH = room.first.second.second / 2;

        bool isConnected = false;

        for (int j = -halfW; j <= halfW; j++) {
            if (grid[level][r1 + j][r2 - halfH] == TileType::ClosedDoor ||
                grid[level][r1 + j][r2 + halfH] == TileType::ClosedDoor) {
                isConnected = true;
                break;
            }
        }

        if (!isConnected) {
            for (int k = -halfH; k <= halfH; k++) {
                if (grid[level][r1 - halfW][r2 + k] == TileType::ClosedDoor ||
                    grid[level][r1 + halfW][r2 + k] == TileType::ClosedDoor) {
                    isConnected = true;
                    break;
                }
            }
        }

        if (!isConnected) {
            for (int j = -halfW; j <= halfW; j++) {
                for (int k = -halfH; k <= halfH; k++) {
                    grid[level][r1 + j][r2 + k] = TileType::Empty;
                }
            }
        }
    }
}

pair<int, int> CreateRooms::PickByWeight(vector<Candidate>& candidates, std::mt19937& gen)
{
    if (candidates.empty())
    {
        return { -1, -1 };
    }

    float totalWeight = 0.0f;

    for (auto& c : candidates)
        totalWeight += c.score;

    if (totalWeight <= 0.0f)
    {
        std::uniform_int_distribution<int> dist(0, candidates.size() - 1);
        return candidates[dist(gen)].pos;
    }

    std::uniform_real_distribution<float> dist(0.0f, totalWeight);
    float r = dist(gen);

    float accumulated = 0.0f;

    for (auto& c : candidates)
    {
        accumulated += c.score;

        if (r <= accumulated)
            return c.pos;
    }

    return candidates.back().pos;
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

void CreateRooms::SpawnTreasures( vector<pair<pair<pair<int, int>, pair<int, int>>, int>>& graph, int level)
{
    vector<Candidate> candidates;

	for (int t = 0; t < lowTreasure + rand() % highTreasure; t++)
    {
        pair<int, int> bestRoom;
        float bestScore = -FLT_MAX;

        for (auto& room : graph)
        {
            pair<int, int> pos = room.first.first;

            if (pos.first == 50 && pos.second == 10)
                continue;
            if (std::find(tresurePosList[level].begin(),
                tresurePosList[level].end(),
                pos) != tresurePosList[level].end())
                continue;
            float score = MathAux::CalculateTreasureScore(
                5.0f, // distWeight
                20.0f, // repulsionRadius
				8.0f, // repulsionWeight
                pos,
                make_pair(50, 10),
                tresurePosList[level]
            );

            if (score > bestScore)
            {
                bestScore = score;
                bestRoom = pos;
            }
            if (bestScore == -FLT_MAX) {
                continue;
            }
            candidates.push_back({ pos, max(score, 0.1f) });
        }

        pair<int, int> chosen = PickByWeight(candidates, gen);

        tresurePosList[level].push_back(chosen);
        grid[level][chosen.first][chosen.second] = TileType::Treasure;

    }
}

void CreateRooms::SpawnEnemies( vector<pair<pair<pair<int, int>, pair<int, int>>, int>>& graph, int level)
{
    vector<Candidate> candidates;
	for (int e = 0; e < lowEnemy + rand() % highEnemy; e++) 
    {
        pair<int, int> bestRoom;
        float bestScore = -FLT_MAX;

        for (auto& room : graph)
        {
            pair<int, int> pos = room.first.first;

            if (pos.first == 50 && pos.second == 10)
                continue;
            if (grid[level][pos.first][pos.second] == TileType::Treasure)
                continue;
            if (std::find(enemyPosList[level].begin(),
                enemyPosList[level].end(),
                pos) != enemyPosList[level].end())
                continue;
            float score = MathAux::CalculateEnemyScore(
                30.0f,  // attractionWeight (treasure)
                25.0f,  // attractionRadius
                40.0f,  // repulsionWeight (enemy)
                20.0f,  // repulsionRadius
                pos,
                tresurePosList[level],
                enemyPosList[level]
            );

            if (score > bestScore)
            {
                bestScore = score;
                bestRoom = pos;
            }
            if (bestScore == -FLT_MAX) {
                continue; 
            }
            candidates.push_back({ pos, max(score, 0.1f) });
        }

        pair<int, int> chosen = PickByWeight(candidates, gen);

        enemyPosList[level].push_back(chosen);
        if (grid[level][chosen.first][chosen.second] == TileType::Treasure) {
            grid[level][chosen.first + 1][chosen.second + 1] = TileType::Enemy;
        }
        else {
            grid[level][chosen.first][chosen.second] = TileType::Enemy;
        }
       

    }
}

void CreateRooms::SpawnEntities(vector<pair<pair<pair<int, int>, pair<int, int>>, int>>& graph, int level)
{
    CreateRooms::SpawnTreasures(graph, level);
	CreateRooms::SpawnEnemies(graph, level);
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