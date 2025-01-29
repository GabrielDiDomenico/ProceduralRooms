# Procedural Dungeon Room generator using Canvas2D for visualization.

Can create multiple rooms and the corridors are conected using A* algorithm

Generate text files with informations about the dungeon, categorizing each tile with a different symbol. You can create a Dungeon with multiple levels, so is created multiple files for each level generated

I'm using a customized version of the A* algorithm from GeeksforGeeks

To visualize just open the make.bat file (it is safe, I think) it is needed MinGW 32 c++ installed

If you want to test you can copy the AstarSearch and CreateRooms files and run in any C++ program.

To edit the code using Canvas2D to visualize you need to use VS Code or VS Studio following the tutorial from my friend Alfredo.

Link to the VS Code tutorial: https://www.youtube.com/watch?v=pU2wzUdGEwY

Link to the VS Studio Tutorial: https://www.youtube.com/watch?v=ZF59t7HX24s

## Current/Future Features

- [x]  Can generate dungeons that contains up to 1000 rooms (maybe more, with luck :D)
- [x]  Easy visualization of dungeons using Canvas2D
- [x]  Generate new dungeon on the fly
- [X]  Import the generated dungeon to a text file
- [ ]  Update room search approach, making possible bigger dungeons, the current one relies on rooms being close with each other for the pathfinding algorithm to actually find and connect the rooms
- [ ]  Update room spawn, the current one is do not consider the area occupied by the rooms, so it's up to the programmer to test how many rooms can fit on the grid, the new planned form take in consideration the available area for each new room
- [ ]  Adding treasures, Updates, Merchants and Enemies
- [ ]  Increase and decrease enemies spawn based on trasure/update position
