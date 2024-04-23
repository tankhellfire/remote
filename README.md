# update
sudo apt update
sudo apt-get install mingw-w64 cmake

-mwindows  <windows.h>// no cmd
-lshlwapi  <shlwapi.h>


mingw-g++ -o main.exe a.cpp -mwindows -lshlwapi -s

mingw-g++ -o main.exe a.cpp -mwindows -lshlwapi -lwininet -static-libgcc -static-libstdc++ -s

mingw-g++ -o main.exe main.cpp -mwindows -lshlwapi -lwininet -static-libgcc -static-libstdc++ -s