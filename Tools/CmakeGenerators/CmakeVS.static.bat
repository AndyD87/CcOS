SET TOOLS_DIR=%cd%
cd ..
cd ..

mkdir Solution.VC.static
cd Solution.VC.static
cmake ../ -DCC_LINK_TYPE=STATIC

cd "%TOOLS_DIR%"
