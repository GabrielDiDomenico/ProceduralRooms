g++ -c -Iinclude src\main.cpp
g++ -c -Iinclude src\gl_canvas2d.cpp
g++ -c -Iinclude src\CreateRooms.cpp
g++ -c -Iinclude src\AstarSearch.cpp
g++ -o app.exe main.o gl_canvas2d.o AstarSearch.o CreateRooms.o -m32 -Iinclude -Llib -lopengl32 -lfreeglut32 -lglu32 -lglfw3
.\app.exe