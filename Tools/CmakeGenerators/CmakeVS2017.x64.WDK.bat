SET TOOLS_DIR=%cd%
cd ..
cd ..

mkdir Solution.VC15.x64.wdk
cd Solution.VC15.x64.wdk
cmake ../ -G "Visual Studio 15 Win64" -DCC_LINK_TYPE=STATIC -DWINDOWS_KERNEL=TRUE -DTARGET_SYSTEM=Generic

cd "%TOOLS_DIR%"
