cd ..
mkdir Solution
cd Solution
cmake ../ -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_INSTALL_PREFIX=/usr
if [ $? -ne 0 ]
then
    exit -1
fi

make -j $(nproc)
if [ $? -ne 0 ]
then
    exit -1
fi

cd ..
cd Tools
