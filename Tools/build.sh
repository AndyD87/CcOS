cd ..
mkdir Solution
cd Solution
cmake ../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCCOS_DRIVER_NETWORK=TRUE
make -j $(nproc)
cd ..
cd Tools
