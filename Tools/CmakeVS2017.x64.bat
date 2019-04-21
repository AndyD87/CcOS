cd ..
mkdir Solution.VC15.x64
cd Solution.VC15.x64
cmake ../ -G "Visual Studio 15 Win64" -DCCGUI_SUBSYSTEM=Generic -DCCOS_DRIVER_NETWORK=TRUE 
cd ..
cd Tools