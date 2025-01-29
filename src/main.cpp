
/*IMPORTANT NOTE*/


/*

This class use the Canvas2D for visualization, this is an outdated library
that uses old openGL, it is bad, but it is simple.

To test this code run the make file so all depencies are called correctly

*/


#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include "gl_canvas2d.h"

#include "CreateRooms.h"
#include <string>
#include <iostream>
#include <DrawRooms.h>

//variavel global para selecao do que sera exibido na canvas.
int opcao = 50;
int screenWidth = 1000, screenHeight = 1000; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
int maxGridHeight = 1000;
int maxGridWidth = 1000;
int maxLevels = 3;
int maxRooms = 10;
CreateRooms* rooms = NULL;
DrawRooms* drawRooms = NULL;


void printGraph(int levelToShow)
{
    CV::color(0, 1, 0);
    if (rooms->Graphs.size() > 1) {
        for (int j = 0; j < rooms->Graphs[levelToShow].size(); j++)
        {
            CV::circleFill(rooms->Graphs[levelToShow][j].first.first.first * 5, rooms->Graphs[levelToShow][j].first.first.second * 5, 4, 20);
        }

        std::string level = "Level: ";

        std::string levelAndNumber = level + std::to_string(levelToShow);
        const char* cString = levelAndNumber.c_str();
        CV::color(0, 0, 0);
        CV::text(10, 10, cString);
    }
    
}
void render()
{
    drawRooms->DrawRoomsOnCanvas();
    printGraph(drawRooms->levelToShow);
}


//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   switch(key)
   {
      //Backspace
      case 8:
          rooms->ClearGrid();
          rooms->InitCreation();
      break;
      //Space
      case 32:
          drawRooms->DrawRoomsOnFile();
          break;
      //Esc
      case 27:
          exit(0);
          break;
      //LeftArrow
      case 200:
          drawRooms->levelToShow++;
         if(drawRooms->levelToShow == maxLevels){
             drawRooms->levelToShow = 0;
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
}

int main(void)
{
    rooms = new CreateRooms(maxGridWidth, maxGridHeight, maxRooms, maxLevels);
    drawRooms = new DrawRooms(rooms->grid, maxGridWidth, maxGridHeight, maxLevels);
    CV::init(&screenWidth, &screenHeight, "Teste da matriz");
    CV::run();
}
