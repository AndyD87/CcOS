cd ..
cd ..
mkdir Solution.LinuxKERNEL
cd Solution.LinuxKERNEL
cmake ../ -DCMAKE_SYSTEM_NAME=LINUX
make -j $(nproc)
