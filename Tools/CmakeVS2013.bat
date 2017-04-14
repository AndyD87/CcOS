cd ..
mkdir Solution
cd Solution
cmake ../ -G "Visual Studio 12" -DTARGET_SYSTEM="Windows" -DCMAKE_GENERATOR_PLATFORM=win32
cd ..
cd Tools