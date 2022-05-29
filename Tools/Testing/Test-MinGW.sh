CWD=$(pwd)
CPU=$(nproc)

# Setup MinGW, Installation must be done before (sudo apt-get install mingw-w64)
# 64bit version
export CC=x86_64-w64-mingw32-gcc
export CXX=x86_64-w64-mingw32-g++

if [ -x "$(command -v $CC)" -a -x "$(command -v $CXX)" ]
then
    rm -rf Solution
    rm -rf Output
    mkdir Solution
    cd Solution

    cmake ../../../Sources -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCC_OUTPUT_DIR=$CWD/Output -DCC_WARNING_AS_ERROR=TRUE -DCCOS_BOARD=CMakeConfig/Boards/MinGW
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    make -j $CPU
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    # Testing not possible in linux
    #make test
    #if [ $? -ne 0 ]
    #then
    #    exit -1
    #fi

    cd $CWD
    rm -rf Solution
    rm -rf Output
    mkdir Solution
    cd Solution

    cmake ../../../Sources -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr -DCC_OUTPUT_DIR=$CWD/Output -DCC_WARNING_AS_ERROR=TRUE -DCCOS_BOARD=CMakeConfig/Boards/MinGW
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    make -j $CPU
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    # Testing not possible in linux
    #make test
    #if [ $? -ne 0 ]
    #then
    #    exit -1
    #fi

    cd $CWD
    rm -rf Solution
    rm -rf Output
else
    echo "MinGW 64bit not found"
fi

# Setup MinGW, Installation must be done before (sudo apt-get install mingw-w64)
# 32 bit version
export CC=i686-w64-mingw32-gcc
export CXX=i686-w64-mingw32-g++

if [ -x "$(command -v $CC)" -a -x "$(command -v $CXX)" ]
then
    rm -rf Solution
    rm -rf Output
    mkdir Solution
    cd Solution

    cmake ../../../Sources -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCC_OUTPUT_DIR=$CWD/Output -DCC_WARNING_AS_ERROR=TRUE -DCCOS_BOARD=CMakeConfig/Boards/MinGW
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    make -j $CPU
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    # Testing not possible in linux
    #make test
    #if [ $? -ne 0 ]
    #then
    #    exit -1
    #fi

    cd $CWD
    rm -rf Solution
    rm -rf Output
    mkdir Solution
    cd Solution

    cmake ../../../Sources -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr -DCC_OUTPUT_DIR=$CWD/Output -DCC_WARNING_AS_ERROR=TRUE -DCCOS_BOARD=CMakeConfig/Boards/MinGW
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    make -j $CPU
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    # Testing not possible in linux
    #make test
    #if [ $? -ne 0 ]
    #then
    #    exit -1
    #fi

    cd $CWD
    rm -rf Solution
    rm -rf Output
else
    echo "MinGW 32bit not found"
fi
