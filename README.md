cmake -S . -B build -G Ninja
cmake --build build
build\tm.exe -config data\MT-2.txt