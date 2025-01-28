#include "AstarSearch.h"


using namespace std;

AstarSearch::AstarSearch(int maxR, int maxC, int***& g) {
    maxRow = maxR;
    maxCol = maxC;
    grid = g;
}

bool AstarSearch::isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < maxRow) && (col >= 0)
        && (col < maxCol);
}


bool AstarSearch::isUnBlocked(int** grid, int row, int col)
{
    // Returns true if the cell is not blocked else false
    if (grid[row][col] == TileType::Wall || grid[row][col] == TileType::ClosedDoor)
        return (false);
    else
        return (true);
}


bool AstarSearch::isDestination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}


float AstarSearch::calculateHValue(int row, int col, Pair dest)
{
    return ((double)sqrt(
        (row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second)));
}


void AstarSearch::tracePath(cell** cellDetails, Pair dest, int level)
{

    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;

    while (!(cellDetails[row][col].parent_i == row
        && cellDetails[row][col].parent_j == col)) {
        Path.push(make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.push(make_pair(row, col));
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();

        if (grid[level][p.first][p.second] != TileType::Door && grid[level][p.first][p.second] != TileType::Floor)
            grid[level][p.first][p.second] = TileType::Corridor;
        if (grid[level][p.first][p.second] == TileType::Door) {
            grid[level][p.first][p.second] = TileType::ClosedDoor;
        }
    }

    return;
}

void AstarSearch::aStarSearch(int** grid, Pair src, Pair dest, int level)
{
    if (isValid(src.first, src.second) == false) {
        printf("Source is invalid\n");
        return;
    }
    if (isValid(dest.first, dest.second) == false) {
        printf("Destination is invalid\n");
        return;
    }

    if (isUnBlocked(grid, src.first, src.second) == false
        || isUnBlocked(grid, dest.first, dest.second)
        == false) {
        printf("Source or the destination is blocked\n");
        return;
    }

    if (isDestination(src.first, src.second, dest)
        == true) {
        printf("We are already at the destination\n");
        return;
    }


    bool** closedList = (bool**)malloc(maxRow * sizeof(bool*));
    closedList[0] = (bool*)malloc(maxRow * maxCol * sizeof(bool));
    for (int j = 0; j < maxRow; j++)
        closedList[j] = closedList[0] + maxCol * j;

    for (int i = 0; i < maxRow; i++)
        for (int j = 0; j < maxCol; j++)
            closedList[i][j] = false;


    cell** cellDetails = (struct cell**)malloc(maxRow * sizeof(struct cell*));
    cellDetails[0] = (struct cell*)malloc(maxRow * maxCol * sizeof(struct cell));

    for (int j = 1; j < maxRow; j++)
        cellDetails[j] = cellDetails[0] + maxCol * j;



    int i, j;

    for (i = 0; i < maxRow; i++) {
        for (j = 0; j < maxCol; j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

    set<pPair> openList;

    openList.insert(make_pair(0.0, make_pair(i, j)));

    bool foundDest = false;

    while (!openList.empty()) {
        pPair p = *openList.begin();

        openList.erase(openList.begin());

        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        double gNew, hNew, fNew;

        if (isValid(i - 1, j) == true) {
            if (isDestination(i - 1, j, dest) == true) {
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;

                tracePath(cellDetails, dest, level);
                foundDest = true;
                goto liberate;
            }
            else if (closedList[i - 1][j] == false
                && isUnBlocked(grid, i - 1, j)
                == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, dest);
                fNew = gNew + hNew;

                if (cellDetails[i - 1][j].f == FLT_MAX
                    || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j)));

                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }

        if (isValid(i + 1, j) == true) {
            if (isDestination(i + 1, j, dest) == true) {
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
     
                tracePath(cellDetails, dest, level);
                foundDest = true;
                goto liberate;
            }
            else if (closedList[i + 1][j] == false
                && isUnBlocked(grid, i + 1, j)
                == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, dest);
                fNew = gNew + hNew;

                if (cellDetails[i + 1][j].f == FLT_MAX
                    || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j)));
                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }

        if (isValid(i, j + 1) == true) {
            if (isDestination(i, j + 1, dest) == true) {
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;

                tracePath(cellDetails, dest, level);
                foundDest = true;
                goto liberate;
            }

            else if (closedList[i][j + 1] == false
                && isUnBlocked(grid, i, j + 1)
                == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, dest);
                fNew = gNew + hNew;

                if (cellDetails[i][j + 1].f == FLT_MAX
                    || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j + 1)));

                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

        //----------- 4th Successor (West) ------------
        if (isValid(i, j - 1) == true) {
            if (isDestination(i, j - 1, dest) == true) {
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;

                tracePath(cellDetails, dest, level);
                foundDest = true;
                goto liberate;
            }


            else if (closedList[i][j - 1] == false
                && isUnBlocked(grid, i, j - 1)
                == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, dest);
                fNew = gNew + hNew;


                if (cellDetails[i][j - 1].f == FLT_MAX
                    || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j - 1)));


                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }
    }


    if (foundDest == false)
        printf("Failed to find the Destination Cell\n");

liberate:

    free(cellDetails[0]);

    free(cellDetails);

    free(closedList[0]);

    free(closedList);


    return;
}
