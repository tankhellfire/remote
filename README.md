# update
sudo apt update
sudo apt install g++-mingw-w64 mingw-w64-{tools,x86-64-dev}  # For 64-bit Windows target
sudo apt install g++-mingw-w64 mingw-w64-{tools,i686-dev}  # For 32-bit Windows target
sudo apt-get install cmake

export CC=x86_64-w64-mingw32-gcc
export CXX=x86_64-w64-mingw32-g++
cmake .