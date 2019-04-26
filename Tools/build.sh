cd ..
mkdir Solution
cd Solution
cmake ../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
make -j $(nproc)
cd ..
cd Tools
