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
#include <DrawRooms.h>
#include <regex>

int screenWidth = 1000, screenHeight = 1000; 
int mouseX, mouseY;
int maxGridHeight = 100;
int maxGridWidth = 100;
int maxLevels = 1;
int maxRooms = 10;
bool changeLang = false;
bool currLang = 0;
string userInput;
string roomDensity;
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
    cout << key;
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
        cout << "Current Dungeon Configuration: " << maxGridWidth << "x" << maxGridWidth << " "
            << "Number of levels: " << maxLevels << " "
            << "Room Density: " << roomDensity << endl;
    }
    else {
        cout << "Configuracao da Dungeon atual: " << maxGridWidth << "x" << maxGridWidth << " "
            << "Numero de niveis: " << maxLevels << " "
            << "Quantidade de salas: " << roomDensity << endl;
    }
}


void ChangeConsoleLanguage() {
    if (changeLang && currLang == 0) {
        system("cls");
        PrintConsoleInformationPortuguese();
        ConfigurationInfo();
        changeLang = false;
        currLang = 1;
    }
    else {
        system("cls");
        PrintConsoleInformationEnglish();
        ConfigurationInfo();
        changeLang = false;
        currLang = 0;
    }
}

bool validateAndExtract(const string& input, int& val1, int& val2, int& val3) {
    regex pattern(R"((\d+)x(\d+)x(\d+))");
    smatch match;

    if (regex_match(input, match, pattern)) {
        val1 = stoi(match[1].str());
        val2 = stoi(match[2].str());
        val3 = stoi(match[3].str());


        if ((val1 >= 1 && val1 <= 1500) && (val2 >= 1 && val2 <= 50) && (val3 >= 1 && val3 <= 3)) {
            return true;
        }
    }
    return false;
}

int main(void)
{
    roomDensity = "low";
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
                rooms = new CreateRooms(maxGridWidth, maxGridHeight, maxRooms, maxLevels);
                drawRooms = new DrawRooms(rooms->grid, maxGridWidth, maxGridHeight, maxLevels, 1);
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
            int promptGridSize, promptLevels, promptRooms;

           
            currLang == 0 ?
                cout << "Type grid size, level number and density value (1 - low, 2 - medium, 3 - max)" << endl :
                cout << "Digite tamanho do grid, numro de niveis e o valor de densidade (1 - pouca, 2 - media, 3 - maxima)" << endl;
            currLang == 0 ?
                cout << "Type the string using the format(1-1500)x(1x1500)x(1-50)x(1-3): " :
                cout << "Digite a string no formato (1-1500)x(1x1500)x(1-50)x(1-3): ";
            
            while (true) {
                
                getline(cin, input);

                if (validateAndExtract(input, promptGridSize, promptLevels, promptRooms)) {
                    maxGridWidth = promptGridSize;
                    maxGridHeight = promptGridSize;
                    maxLevels = promptLevels;
                    
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
