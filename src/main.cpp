
#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>  
#include <vector>
#include "gl_canvas2d.h"

//Para incluir dar quickfix e adicionar isso C:\MinGW\lib\gcc\mingw32\6.3.0\include\c++\mingw32
#include <stack>
#include <set>
#include<list>

using namespace std;

#define ROW 1000
#define COL 1000
#define NUM_ROOMS 30
//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
int screenWidth = 1000, screenHeight = 1000; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
int ***grid;
int directions[9][2] = {{0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, 1}, {1, -1}, {-1, 1}};
int maxSteps = 10;
int levelToShow=0;
list<int> indexToRemove;

enum TileType {
    Wall = 1,
    Door,
    Floor,
    Corridor,
    ClosedDoor
};

// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;
 
Pair a;
Pair b;
typedef vector<Pair> rooms;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;
 
// A structure to hold the necessary parameters
struct cell {

    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
};

vector<vector<pair<pair<pair<int, int>, pair<int, int>>, int>>> Graphs;
vector<pair<pair<pair<int, int>, pair<int, int>>, int>> graph;
// pair(pair(positionXY,roomSizeHW),numberOfOpenedDoors)
pair<pair<pair<int,int>,pair<int,int>>,int> node;
 
// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < ROW) && (col >= 0)
           && (col < COL);
}
 
// A Utility Function to check whether the given cell is
// blocked or not
bool isUnBlocked(int** grid, int row, int col)
{
    // Returns true if the cell is not blocked else false
    if (grid[row][col] == TileType::Wall || grid[row][col] == TileType::ClosedDoor)
        return (false);
    else
        return (true);
}
 
// A Utility Function to check whether destination cell has
// been reached or not
bool isDestination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(int row, int col, Pair dest)
{

    return ((double)sqrt(
        (row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second)));
    // Return using the distance formula
    //return ((double)std::abs(row - dest.first) + std::abs(col - dest.second));
}

void tracePath(cell** cellDetails, Pair dest, int level)
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
        
        if(grid[level][p.first][p.second] != TileType::Door && grid[level][p.first][p.second] != TileType::Floor)
            grid[level][p.first][p.second] = TileType::Corridor;
        if (grid[level][p.first][p.second] == TileType::Door) {
            grid[level][p.first][p.second] = TileType::ClosedDoor;
        }
    }
 
    return;
}


void aStarSearch(int** grid, Pair src, Pair dest, int level)
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
    

    bool** closedList = (bool **)malloc(ROW * sizeof(bool*));
    closedList[0] = (bool *)malloc(ROW * COL * sizeof(bool));
    for(int j = 0; j < ROW; j++) 
        closedList[j] = closedList[0] + COL * j;

    for(int i=0;i<ROW;i++)
        for(int j=0;j<COL;j++)
            closedList[i][j] = false;
    

    cell** cellDetails = (struct cell**)malloc(ROW * sizeof(struct cell*));
    cellDetails[0] = (struct cell*)malloc(ROW * COL * sizeof(struct cell));
        
    for(int j = 1; j < ROW; j++)
        cellDetails[j] = cellDetails[0] + COL * j;
    
     
 
    int i, j;

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
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
                //printf("The destination cell is found\n");
                tracePath(cellDetails, dest,level);
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
                //printf("The destination cell is found\n");
                tracePath(cellDetails, dest,level);
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
                //printf("The destination cell is found\n");
                tracePath(cellDetails, dest,level);
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
                //printf("The destination cell is found\n");
                tracePath(cellDetails, dest,level);
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

void CreateRooms(int nivel, int numOfRooms)
{
   int numberOfTries = 0;
   for(int i=0;i<numOfRooms;i++)
   {
      
      int r1 = rand() % numOfRooms * 3 + 20;
      int r2 = rand() % numOfRooms * 3 + 20;
      
      bool canMake = true;
      
      int randRoomSizeH = rand() % 10 + 8;
      int randRoomSizeW = rand() % 10 + 8;
      if(randRoomSizeH%2!=0)
         randRoomSizeH++;
      if(randRoomSizeW%2!=0)
         randRoomSizeW++;
      if(numberOfTries > 100000){
         numberOfTries=0;
         continue;
      }
      
      for (int step = 0; step < maxSteps; step++) {
        for (int d = 0; d < 9; d++) {
            int newRow = r1 + directions[d][0] * (step + 1);
            int newCol = r2 + directions[d][1] * (step + 1);

            if (newRow >= ROW && newRow < ROW &&
                newCol >= COL && newCol < COL &&
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
         if(!canMake)
            break;
      }
      
      
      if(canMake)
      {
        if(i==0){
            node = make_pair(make_pair(make_pair(50,10),make_pair(5,5)),4);
  
            graph.push_back(node);
        }
        node = make_pair(make_pair(make_pair(r1,r2),make_pair(randRoomSizeW,randRoomSizeH)),4);
  
        graph.push_back(node);
        for(int j=-(int)(randRoomSizeW/2);j<(int)(randRoomSizeW/2);j++)
        {
            for(int k=-(int)(randRoomSizeH/2);k<(int)(randRoomSizeH/2);k++)
            {
                grid[nivel][j+r1][k+r2]= TileType::Floor;
            }
        }
        
        for(int j=-(int)(randRoomSizeW/2);j<=(int)(randRoomSizeW/2);j++)
        {
            if(j==0)
            {
                grid[nivel][j+r1][r2-(int)(randRoomSizeH/2)]= TileType::Door;
                grid[nivel][j+r1][r2+(int)(randRoomSizeH/2)]= TileType::Door;
            }else
            {
                grid[nivel][j+r1][r2-(int)(randRoomSizeH/2)]= TileType::Wall;
                grid[nivel][j+r1][r2+(int)(randRoomSizeH/2)]= TileType::Wall;
            }
        
        }

        for(int k=-(int)(randRoomSizeH/2);k<=(int)(randRoomSizeH/2);k++)
        {
        if(k==0)
        {
            grid[nivel][r1-(int)(randRoomSizeW/2)][r2+k]= TileType::Door;
            grid[nivel][r1+(int)(randRoomSizeW/2)][r2+k]= TileType::Door;
        }else
        {
            grid[nivel][r1-(int)(randRoomSizeW/2)][r2+k]= TileType::Wall;
            grid[nivel][r1+(int)(randRoomSizeW/2)][r2+k]= TileType::Wall;
        }
        
        }
      }
   }
}

void printGraph(int levelToShow)
{
    CV::color(0,1,0);

    for(int j=0;j<Graphs[levelToShow].size();j++)
    {
        CV::circleFill(Graphs[levelToShow][j].first.first.first*5,Graphs[levelToShow][j].first.first.second*5,4,20);
    }
    
}

void spawnStartAndStairs()
{
    
    for(int i=0;i<3;i++){
        
        for(int j=-(int)(5/2);j<(int)(5/2);j++)
        {
            for(int k=-(int)(5/2);k<(int)(5/2);k++)
            {
                grid[i][j+50][k+10]= TileType::Floor;
            }
        }
        for(int j=-(int)(5/2);j<=(int)(5/2);j++)
        {
            if(j==0)
            {
                grid[i][j+50][10-(int)(5/2)]= TileType::Door;
                grid[i][j+50][10+(int)(5/2)]= TileType::Door;
            }else
            {
                grid[i][j+50][10-(int)(5/2)]= TileType::Wall;
                grid[i][j+50][10+(int)(5/2)]= TileType::Wall;
            }
        
        }

        for(int k=-(int)(5/2);k<=(int)(5/2);k++)
        {
            if(k==0)
            {
                grid[i][50-(int)(5/2)][10+k]= TileType::Door;
                grid[i][50+(int)(5/2)][10+k]= TileType::Door;
            }else
            {
                grid[i][50-(int)(5/2)][10+k]= TileType::Wall;
                grid[i][50+(int)(5/2)][10+k]= TileType::Wall;
            }
        }
    }
   
        
        
}


//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
 
   for(int i=0;i<999;i++)
   {
      for(int j=0;j<999;j++)
      {
         if(grid[levelToShow][i][j] == 1){
            CV::color(0,0,0);
            CV::rectFill(i ,j,(i+1) ,(j+1) );
         }
         if(grid[levelToShow][i][j] == 3){
            CV::color(0.8,0.5,0.2);
            CV::rectFill(i ,j ,(i+1) ,(j+1) );
         }
         if(grid[levelToShow][i][j] == 2 || grid[levelToShow][i][j] == TileType::ClosedDoor){
            CV::color(1,0,0);
            CV::rectFill(i ,j ,(i+1) ,(j+1));
         }
         if(grid[levelToShow][i][j] == 4){
            CV::color(1,0,1);
            CV::rectFill(i ,j ,(i+1),(j+1));
         }
      }
   }
   
   printGraph(levelToShow);

}
/*
void render()
{

    for (int i = 0; i < 999; i++)
    {
        for (int j = 0; j < 999; j++)
        {
            if (grid[levelToShow][i][j] == 1) {
                CV::color(0, 0, 0);
                CV::rectFill(i * 5, j * 5, (i + 1) * 5, (j + 1) * 5);
            }
            if (grid[levelToShow][i][j] == 3) {
                CV::color(0.8, 0.5, 0.2);
                CV::rectFill(i * 5, j * 5, (i + 1) * 5, (j + 1) * 5);
            }
            if (grid[levelToShow][i][j] == 2 || grid[levelToShow][i][j] == TileType::ClosedDoor) {
                CV::color(1, 0, 0);
                CV::rectFill(i * 5, j * 5, (i + 1) * 5, (j + 1) * 5);
            }
            if (grid[levelToShow][i][j] == 4) {
                CV::color(1, 0, 1);
                CV::rectFill(i * 5, j * 5, (i + 1) * 5, (j + 1) * 5);
            }
        }
    }

    printGraph(levelToShow);

}*/


//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   switch(key)
   {
      case 27:
         exit(0);
      break;

      //seta para a esquerda
      case 200:
         levelToShow++;
         if(levelToShow==3){
            levelToShow=0;
         }
      break;

   
   }  
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

  
}
//TODO
/*
void cleanNotConectedNodes(vector<pair<pair<pair<int, int>, pair<int, int>>, int>>& pGraph)
{
    for (int i = 0; i < pGraph.size(); i++)
    {
        for (int j = -(int)(pGraph / 2); j < (int)(randRoomSizeW / 2); j++)
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
*/


void updateDoorsClosedNumber(pair<pair<pair<int, int>, pair<int, int>>, int>& Room, int level) {
    int numOfOpenedDoors = 4;
    
    Pair pos = Room.first.first;
    Pair size = Room.first.second;
    if (grid[level][(int)pos.first + (size.first/2)][pos.second] == TileType::ClosedDoor) {
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

void findAllCorridors(vector<pair<pair<pair<int, int>, pair<int, int>>, int>>& pGraph, Pair nodeToStart, int level)
{
    
    if(pGraph.size() < 0){
        return;
    }
    
    Pair closestNode;
    
    double lastDistance=100000000000.0;
    float max_dist = 40.0;

    for(int k=0;k<pGraph.size();k++)
    {
        double currDistance = calculateHValue(nodeToStart.first,nodeToStart.second,pGraph[k].first.first);
        //cout << currDistance << endl;
        if (currDistance > max_dist) {
            continue;
        }
        updateDoorsClosedNumber(pGraph[k], level);
        if (pGraph[k].second == 0) {
            pGraph.erase(pGraph.begin() + k);
            continue;
        }
        if(currDistance < lastDistance){
            closestNode = pGraph[k].first.first;

            lastDistance = currDistance;
  
            aStarSearch(grid[level], nodeToStart, closestNode, level);
            
            pGraph.erase(pGraph.begin() + k);
     
            findAllCorridors(pGraph, closestNode, level);
        }
    }
    
   
    
}

int main(void)
{
    grid = (int ***)malloc(3 * sizeof(int**));
    for(int i=0;i<3;i++){
        grid[i] = (int **)malloc(ROW * sizeof(int*));
        grid[i][0] = (int *)malloc(ROW * COL * sizeof(int));
        for(int j = 0; j < ROW; j++) 
            grid[i][j] = grid[i][0] + COL * j;
    } 
   
    srand(time(NULL));
    for(int k=0;k<3;k++)
        for(int i=0;i<ROW;i++)
            for(int j=0;j<COL;j++)
                grid[k][i][j] = 0;
    spawnStartAndStairs();
    for(int i=0;i<3;i++)
    {
        CreateRooms(i, NUM_ROOMS);
        Graphs.push_back(graph);
        findAllCorridors(Graphs[i], Graphs[i][0].first.first,i);
        std::cout << "saiu" << std::endl;
        graph.clear();
            
    }
        
    
    
    CV::init(&screenWidth, &screenHeight, "Teste da matriz");
    CV::run();
}
