SET TOOLS_DIR=%cd%
cd ..
cd ..

mkdir Solution.VC15.x64.Qt
cd Solution.VC15.x64.Qt
cmake ../ -G "Visual Studio 15 Win64" -DQT_QMAKE_EXECUTABLE=C:/Qt/5.12.1/msvc2017_64/bin/qmake.exe -DQt5_DIR=C:/Qt/5.12.1/msvc2017_64/lib/cmake/Qt5 -DQT5Core_DIR=C:/Qt/5.12.1/msvc2017_64/lib/cmake/Qt5Core -DQT5_DIR=C:/Qt/5.12.1/msvc2017_64/lib/cmake/Qt5

cd "%TOOLS_DIR%"
