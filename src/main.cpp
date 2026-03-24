/**
*
*   Autor: Gabriel Di Domenico
*
*   gddomenico@inf.ufsm.br
*
**/

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
#include "DrawRooms.h"
#include <regex>


int screenWidth = 1000, screenHeight = 1000; 
int mouseX, mouseY;
int maxGridHeight = 100;
int maxGridWidth = 100;
int maxLevels = 3;
int maxRooms = 100*0.8;
int lowEnemy = 3;
int highEnemy = 6;
int lowTreasure = 2;
int highTreasure = 4;
bool changeLang = false;
bool currLang = 0;
string userInput;
string roomDensity = "High";
CreateRooms* rooms = NULL;
DrawRooms* drawRooms = NULL;

void printGraph(int levelToShow)
{
    CV::color(0, 1, 0);
    if (rooms->Graphs.size() > 1) {
       
        std::string level = "Level: ";

        std::string levelAndNumber = level + std::to_string(levelToShow);
        const char* cString = levelAndNumber.c_str();
        CV::color(0, 0, 0);
        CV::text(10, screenHeight-10, cString);
    }
}

void render()
{
    drawRooms->DrawRoomsOnCanvas();
    printGraph(drawRooms->levelToShow);

}

void keyboard(int key)
{
   switch(key)
   {
      //Backspace - New Dungeon
      case 8:
          rooms->ClearGrid();
          rooms->InitCreation();
      break;
      //Space - Save Dungeon on file
      case 32:
          drawRooms->DrawRoomsOnFile();
          break;
      //Esc
      case 27:
          exit(0);
          break;
      //LeftArrow 200 - UpArrow 201 - RightArrow 202 - DownArrow 203
      case 203:
          drawRooms->shiftRightLeft--;
          break;
      case 202:
          drawRooms->shiftUpDown++;
          break;
      case 201:
          drawRooms->shiftRightLeft++;
          break;
      case 200:
          drawRooms->shiftUpDown--;
          break;
      //c
      case 99:
         drawRooms->levelToShow++;
         if(drawRooms->levelToShow == maxLevels){
             drawRooms->levelToShow = 0;
         }
         break;
      //z
      case 122:
          drawRooms->zoom++;
          if (drawRooms->zoom == 10) {
              drawRooms->zoom = 1;
          }
      break;
   }  
}

void keyboardUp(int key)
{
   
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x;
   mouseY = y;
}

void printWithAsterisks(const std::string& text) {

    std::cout << std::string(text.length() + 4, '*') << std::endl;
    std::cout << "* " << text << " *" << std::endl;
    std::cout << std::string(text.length() + 4, '*') << std::endl;
}

void PrintConsoleInformationEnglish() {
    cout << "\n";
    printWithAsterisks("Procedural Dungeon Generator");
    cout << "\n";
    cout << "Q - Change to PT-BR";
    cout << "\n";
    cout << "F - Setup Dungeon Grid and Rooms density";
    cout << "\n";
    cout << "W - Generate Dungeon";
    cout << "\n";
    cout << "S - Save to file";
    cout << "\n";
    cout << "C - Visualize in Canvas2D -> You can create new Dungeons on Canvas2D";
    cout << "\n";
}

void PrintConsoleInformationPortuguese() {
    cout << "\n";
    printWithAsterisks("Gerador Procedural de Dungeons");
    cout << "\n";
    cout << "Q - Mudar para EN-US";
    cout << "\n";
    cout << "F - Configurar grid da Dungeon e densidade de salas";
    cout << "\n";
    cout << "W - Gerar Dungeon";
    cout << "\n";
    cout << "S - Salvar para arquivo";
    cout << "\n";
    cout << "C - Visualizar em Canvas2D -> Voce pode criar novas Dungeons no Canvas2D";
    cout << "\n";
}

void ConfigurationInfo() {
    cout << "\n";
    if (currLang == 0) {
        cout << "Current Dungeon Configuration: \n Grid Size: " << maxGridWidth << "x" << maxGridWidth << " | "
            << "Number of levels: " << maxLevels << " | "
			<< "Room Density: " << roomDensity << " | "
			<< "Range of Enemies: " << lowEnemy << " - " << highEnemy << " | "
            << "Range of Treasures: " << lowTreasure << " - " << highTreasure
            << endl;
    }
    else {
        cout << "Configuracao da Dungeon atual: \n Tamanho do grid: " << maxGridWidth << "x" << maxGridWidth << " | "
            << "Numero de niveis: " << maxLevels << " | "
            << "Quantidade de salas: " << roomDensity << " | "
            << "Intervalo de inimigos: " << lowEnemy << " - " << highEnemy << " | "
            << "Intervalo de tesouros: " << lowTreasure << " - " << highTreasure
            << endl;
    }
}


void ChangeConsoleLanguage() {
    if (changeLang && currLang == 0) {
        system("cls");
        changeLang = false;
        currLang = 1;
        PrintConsoleInformationPortuguese();
        ConfigurationInfo();
        
    }
    else {
        system("cls");
        changeLang = false;
        currLang = 0;
        PrintConsoleInformationEnglish();
        ConfigurationInfo();
        
    }
}

bool validateAndExtract(const std::string& input, int& val1, int& val2, int& val3, int& val4, int& val5, int& val6, int& val7) {

    std::regex pattern(R"(^(\d+)x(\d+)x(\d+)x(\d+)(?:,(\d+))?x(\d+)(?:,(\d+))?$)");
    std::smatch match;

    if (std::regex_match(input, match, pattern)) {
        val1 = std::stoi(match[1].str());
        val2 = std::stoi(match[2].str());
        val3 = std::stoi(match[3].str());
        val4 = std::stoi(match[4].str());
        if (match[5].matched) {
            val5 = std::stoi(match[5].str());
        }
        else {
            val5 = val4;
        }
        val6 = std::stoi(match[6].str());
        if (match[7].matched) {
            val7 = std::stoi(match[7].str());
        }
        else {
            val7 = val6;
        }

        bool v1_3 = (val1 >= 1 && val1 <= 1500) && (val2 >= 1 && val2 <= 50) && (val3 >= 1 && val3 <= 3);
        bool v4_5 = (val4 >= 1 && val4 <= 30) && (val5 >= 1 && val5 <= 30) && (val4 <= val5);
        bool v6_7 = (val6 >= 1 && val6 <= 30) && (val7 >= 1 && val7 <= 30) && (val6 <= val7);

        return v1_3 && v4_5 && v6_7;
    }

    return false;
}

int main(void)
{
    PrintConsoleInformationEnglish();
    ConfigurationInfo();
    while (true) {
        getline(cin, userInput);

        if (userInput == "q") {
            changeLang = true;
            ChangeConsoleLanguage();
        }
        if (userInput == "w") {
            if (rooms == NULL) {
                currLang == 0 ? cout << "Creating Dungeon...." : cout << "Criando Dungeon....";
                rooms = new CreateRooms(maxGridWidth, maxGridHeight, maxRooms, maxLevels, lowEnemy, highEnemy, lowTreasure, highTreasure);
                drawRooms = new DrawRooms(rooms->grid, maxGridWidth, maxGridHeight, maxLevels, 1);
                cout << "\n";
                currLang == 0 ? cout << "Dungeon Created" << endl : cout << "Dungeon Criada" << endl;
            }
            else {
                rooms->ClearGrid();
                rooms->InitCreation();
                currLang == 0 ? cout << "Dungeon Created" << endl : cout << "Dungeon Criada" << endl;
            }
        }
        if (rooms != NULL && userInput == "s") {
            drawRooms->DrawRoomsOnFile();
            currLang == 0 ? cout << "Dungeon copied to file" << endl : cout << "Dungeon copiada para arquivo" << endl;
        }
        if (rooms != NULL && userInput == "c") {
            CV::init(&screenWidth, &screenHeight, "Teste da matriz");
            CV::run();
        }

        if (userInput == "f") {
            cout << "\n";
            string input;
            int promptGridSize, promptLevels, promptRooms, promptLowEnemies, promptHighEnemies, promptLowTreasures, prompthighTreasures;

           
            currLang == 0 ?
                cout << "Type grid size, number of levels, density room value (1 - low, 2 - medium, 3 - max), range of enemies, range of trasures" << endl :
                cout << "Digite tamanho do grid, numero de niveis, valor de densidade de quartos (1 - pouca, 2 - media, 3 - maxima), intervalo de inimigos e intervalo de tesouros" << endl;
            currLang == 0 ?
                cout << "Use the format (1-1500)x(1-50)x(1-3)x(1-30,1-30)x(1-30,1-30), ex-> 100x3x3x3,6x2,8: " :
                cout << "Utilize o formato (1-1500)x(1-50)x(1-3)x(1-30,1-30)x(1-30,1-30), ex-> 100x3x3x3,6x2,8: ";
            
            while (true) {
                
                getline(cin, input);

                if (validateAndExtract(input, promptGridSize, promptLevels, promptRooms, promptLowEnemies, promptHighEnemies, promptLowTreasures, prompthighTreasures)) {
                    maxGridWidth = promptGridSize;
                    maxGridHeight = promptGridSize;
                    maxLevels = promptLevels;
                    lowEnemy = promptLowEnemies;
					highEnemy = promptHighEnemies;
					lowTreasure = promptLowTreasures;
					highTreasure = prompthighTreasures;
                    
                    switch (promptRooms) {
                        case 1:
                            maxRooms = promptGridSize*0.1;
                            currLang == 0 ?
                                roomDensity = "Low" :
                                roomDensity = "Baixa";
                            break;
                        case 2:
                            maxRooms = promptGridSize * 0.4;
                            currLang == 0 ?
                                roomDensity = "Medium" :
                                roomDensity = "Media";
                            break;
                        case 3:
                            maxRooms = promptGridSize * 0.8;
                            currLang == 0 ?
                                roomDensity = "High" :
                                roomDensity = "Alta";
                            break;
                    }
                    system("cls");
                    if (currLang == 0) {
                        PrintConsoleInformationEnglish();
                        ConfigurationInfo();
                    }
                    else {
                        PrintConsoleInformationPortuguese();
                        ConfigurationInfo();
                    }
                         
                    break;
                }
                else {
                    currLang == 0 ?
                        cout << "Invalid Prompt" << endl :
                        cout << "Prompt Invalido" << endl;
                }
            }
        }
    }
}
