cd ..
cd Solution
cmake ../ -G "Visual Studio 12" -DCMAKE_GENERATOR_PLATFORM=x64 --config Release
cmake.exe --build . --config Release
ctest -C Release
cd ..
cd Tools