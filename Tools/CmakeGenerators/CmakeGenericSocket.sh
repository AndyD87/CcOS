cd ..
mkdir Solution.Generic
cd Solution.Generic
cmake ../Source -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCCOS_GENERIC_NETWORK_LINUX_CCRAWSOCKET=TRUE
make -j $(nproc)
