cd ..
cd ..
mkdir Solution.Generic
cd Solution.Generic
cmake ../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCCOS_DRIVER_NETWORK_LINUX_CCRAWSOCKET=TRUE
make -j $(nproc)
