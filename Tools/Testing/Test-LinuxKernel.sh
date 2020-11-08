CWD=$(pwd)
CPU=$(nproc)

# Setup Gcc
export CC=gcc
export CXX=g++

if [ -x "$(command -v $CC)" ]
then
  LINUX_HEADERS_DIR=/usr/src/linux-headers-$(uname -r)  

  if [ -d $LINUX_HEADERS_DIR ]
  then
    echo "Linux headers found, start KernelModule build"
    rm -rf Solution
    rm -rf Output
    mkdir Solution
    cd Solution

    cmake ../../../ -DCMAKE_BUILD_TYPE=Release -DCC_OUTPUT_DIR=$CWD/Output -DCC_WARNING_AS_ERROR=TRUE -DLINUXKERNEL=TRUE
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    make -j $CPU
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    # No test available yet
    #make test
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    cd $CWD
    rm -rf Solution
    rm -rf Output
    mkdir Solution
    cd Solution

    cmake ../../../ -DCMAKE_BUILD_TYPE=Debug -DCC_OUTPUT_DIR=$CWD/Output -DCC_WARNING_AS_ERROR=TRUE -DLINUXKERNEL=TRUE
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    make -j $CPU
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    # No test available yet
    #make test
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    cd $CWD
    rm -rf Solution
    rm -rf Output
  else
    echo "No linux headers available, skip KernelModule build"
  fi
fi

