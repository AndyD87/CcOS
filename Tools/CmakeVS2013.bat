cd ..
mkdir Solution.VC12
cd Solution.VC12
cmake ../ -G "Visual Studio 12" -DTARGET_SYSTEM="Windows" -DCMAKE_GENERATOR_PLATFORM=win32
cd ..
cd Tools