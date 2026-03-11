# Procedural Dungeon Room generator using Canvas2D for visualization.

Can create multiple rooms and the corridors are conected using A* algorithm

Generate text files with informations about the dungeon, categorizing each tile with a different symbol. You can create a Dungeon with multiple levels, so is created multiple files for each level generated

I'm using a customized version of the A* algorithm from GeeksforGeeks

To visualize just open the make.bat file, it is needed GCC with a version superior than 8.0 installed (I'm using <filesystem>). The way I install on windows is by using MSYS, it's not that complicated, just download from [here](https://www.msys2.org/).

I use an 32 bit version because of the Canvas2D (will remove in the future), so it's needed some extra steps, write this on the MSYS console:

* pacman -S mingw-w64-i686-gcc 
* pacman -S mingw-w64-i686-freeglut
* pacman -S mingw-w64-i686-glfw


when done add the path on the ambient variables -> (C:\msys64\mingw32\bin).

If you want to test you can copy the AstarSearch and CreateRooms files and run in any C++ program.

To edit the code using Canvas2D to visualize you need to use VS Code or VS Studio following the tutorial from my friend Alfredo.

Link to the VS Code tutorial: https://www.youtube.com/watch?v=pU2wzUdGEwY

Link to the VS Studio Tutorial: https://www.youtube.com/watch?v=ZF59t7HX24s

## Current/Future Features

- [x]  Can generate dungeons that contains up to 1000 rooms (maybe more, with luck :D)
- [x]  Easy visualization of dungeons using Canvas2D
- [x]  Generate new dungeon on the fly
- [x]  Import the generated dungeon to a text file
- [x]  Dungeon creation via terminal  
- [x]  Add dungeon configuration from terminal
- [ ]  Adding the possibility to spawn treasures, updates, merchants and enemies
- [ ]  Increase and decrease enemies spawn based on treasure/update position
