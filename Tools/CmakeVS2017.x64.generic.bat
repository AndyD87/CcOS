cd ..
mkdir Solution.VC15.x64.generic
cd Solution.VC15.x64.generic
cmake ../ -G "Visual Studio 15 Win64" -DCCGUI_SUBSYSTEM=Generic -DCCOS_DRIVER_NETWORK=TRUE
cd ..
cd Tools