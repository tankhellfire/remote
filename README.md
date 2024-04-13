# update
sudo apt update
sudo apt-get install mingw-w64 cmake

-mwindows  <windows.h>// no cmd
-lshlwapi  <shlwapi.h>a


mingw-g++ -o main.exe a.cpp -mwindows -lshlwapi -s

mingw-g++ -o main.exe a.cpp -mwindows -lshlwapi -lwininet -static-libgcc -static-libstdc++ -s

mingw-g++ -o main.exe test.cpp -mwindows -lshlwapi -lwininet -static-libgcc -static-libstdc++ -s