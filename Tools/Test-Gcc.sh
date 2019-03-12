CWD=$(pwd)
CPU=$(nproc)

# Setup Gcc
export CC=gcc
export CXX=g++

rm -rf Solution
rm -rf Output
mkdir Solution
cd Solution

cmake ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCC_OUTPUT_DIR=$CWD/Output -DCC_WARNING_AS_ERROR=TRUE
if [ $? -ne 0 ]
then
    exit -1
fi

make -j $CPU
if [ $? -ne 0 ]
then
    exit -1
fi

make test
if [ $? -ne 0 ]
then
    exit -1
fi

cd $CWD
rm -rf Solution
rm -rf Output
mkdir Solution
cd Solution

cmake ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr -DCC_OUTPUT_DIR=$CWD/Output -DCC_WARNING_AS_ERROR=TRUE
if [ $? -ne 0 ]
then
    exit -1
fi

make -j $CPU
if [ $? -ne 0 ]
then
    exit -1
fi

make test
if [ $? -ne 0 ]
then
    exit -1
fi

cd $CWD
rm -rf Solution
rm -rf Output
