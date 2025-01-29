/**
*
*   Autor: Gabriel Di Domenico
*
*   gddomenico@inf.ufsm.br
*
**/

#include "DrawRooms.h"
#include <TileTypeEnum.h>

using namespace std;

DrawRooms::DrawRooms(int***& g, int w, int h, int l, int z) {
	grid = g;
    gridHeight = h;
    gridWidth = w;
    maxLevels = l;
    levelToShow = 0;
    zoom = z;
    shiftRightLeft = 0;
    shiftUpDown = 0;
}

void DrawRooms::DrawRoomsOnCanvas() {
    for (int i = 0; i < gridHeight - 1; i++)
    {
        for (int j = 0; j < gridWidth - 1; j++)
        {
            if (grid[levelToShow][i][j] == TileType::Wall) {
                CV::color(0, 0, 0);
                CV::rectFill((i + shiftUpDown) * zoom, (j + shiftRightLeft) * zoom, (i + shiftUpDown + 1) * zoom, (j + shiftRightLeft + 1) * zoom);
            }
            if (grid[levelToShow][i][j] == TileType::Floor) {
                CV::color(0.8, 0.5, 0.2);
                CV::rectFill((i + shiftUpDown) * zoom, (j + shiftRightLeft) * zoom, (i + shiftUpDown + 1) * zoom, (j + shiftRightLeft + 1) * zoom);
            }
            if (grid[levelToShow][i][j] == TileType::Door || grid[levelToShow][i][j] == TileType::ClosedDoor) {
                CV::color(1, 0, 0);
                CV::rectFill((i + shiftUpDown) * zoom, (j + shiftRightLeft) * zoom, (i + shiftUpDown + 1) * zoom, (j + shiftRightLeft + 1) * zoom);
            }
            if (grid[levelToShow][i][j] == TileType::Corridor) {
                CV::color(1, 0, 1);
                CV::rectFill((i + shiftUpDown) * zoom, (j + shiftRightLeft) * zoom, (i + shiftUpDown + 1) * zoom, (j + shiftRightLeft + 1) * zoom);
            }
        }
    }
}

void DrawRooms::DrawRoomsOnFile() {
    //if your code ins't working on this part you need to use C++17
    filesystem::path projectRoot = filesystem::current_path();
    filesystem::path targetDir = projectRoot / "AllDungeonLevels";

    if (filesystem::create_directory(targetDir)) {
        cout << "Pasta criada em: " << targetDir << endl;
    }
    
    std::string fileName;

    for (int lvl = 0; lvl < maxLevels; lvl++) {
        fileName = "Dungeon" + to_string(lvl) + ".txt";
        ofstream file(targetDir / fileName);
        for (int i = 0; i < gridHeight - 1; i++)
        {
            for (int j = 0; j < gridWidth - 1; j++)
            {
                if (grid[lvl][i][j] == TileType::Wall) {
                    file << "W";
                }
                else if (grid[lvl][i][j] == TileType::Floor) {
                    file << "F";
                }
                else if (grid[lvl][i][j] == TileType::Door || grid[lvl][i][j] == TileType::ClosedDoor) {
                    file << "D";
                }
                else if (grid[lvl][i][j] == TileType::Corridor) {
                    file << "C";
                }
                else {
                    file << ".";
                }
            }
            file << "\n";
        }
        file.close();
    }
    
}