
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


//variavel global para selecao do que sera exibido na canvas.
int opcao = 50;
int screenWidth = 1000, screenHeight = 1000; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
int levelToShow = 0;
CreateRooms* rooms = NULL;


void printGraph(int levelToShow)
{
    CV::color(0, 1, 0);

    for (int j = 0; j < rooms->Graphs[levelToShow].size(); j++)
    {
        CV::circleFill(rooms->Graphs[levelToShow][j].first.first.first * 5, rooms->Graphs[levelToShow][j].first.first.second * 5, 4, 20);
    }

}
void render()
{

    for (int i = 0; i < 999; i++)
    {
        for (int j = 0; j < 999; j++)
        {
            if (rooms->grid[levelToShow][i][j] == 1) {
                CV::color(0, 0, 0);
                CV::rectFill(i * 5, j * 5, (i + 1) * 5, (j + 1) * 5);
            }
            if (rooms->grid[levelToShow][i][j] == 3) {
                CV::color(0.8, 0.5, 0.2);
                CV::rectFill(i * 5, j * 5, (i + 1) * 5, (j + 1) * 5);
            }
            if (rooms->grid[levelToShow][i][j] == 2 || rooms->grid[levelToShow][i][j] == TileType::ClosedDoor) {
                CV::color(1, 0, 0);
                CV::rectFill(i * 5, j * 5, (i + 1) * 5, (j + 1) * 5);
            }
            if (rooms->grid[levelToShow][i][j] == 4) {
                CV::color(1, 0, 1);
                CV::rectFill(i * 5, j * 5, (i + 1) * 5, (j + 1) * 5);
            }
        }
    }

    printGraph(levelToShow);

}


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

int main(void)
{
    
    rooms = new CreateRooms(1000,1000,1,5);

    
    CV::init(&screenWidth, &screenHeight, "Teste da matriz");
    CV::run();
}
