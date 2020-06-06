
matches() {
    input="$1"
    pattern="$2"
    echo "$input" | grep -q "$pattern"
}

cd Testing

export PREBUILD_REQUIRED=1
export CTEST_OUTPUT_ON_FAILURE=1
export TEST_CCOS=TRUE

sh Test-Gcc.sh
if [ $? -ne 0 ]
then
    exit -1
fi

sh Test-Clang.sh
if [ $? -ne 0 ]
then
    exit -1
fi

ARCHITECTURE=$(uname -m)
if matches $ARCHITECTURE "x64" || matches $ARCHITECTURE "x86"
then
    echo "Run Tests on x64 and x86 available"

    # Test Windows Cross Compilation
    sh Test-MinGW.sh
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    # Use STM32F4Discovery-Build for testing generic
    sh Test-STM32F4Discovery.sh
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    # Use STM32F3Discovery-Build for testing generic
    sh Test-STM32F3Discovery.sh
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    # Use STM32F220GEval-Build for testing generic
    sh Test-STM32F220GEval.sh
    if [ $? -ne 0 ]
    then
        exit -1
    fi

    echo Test-ESP8266
    # Use ESP8266 with Xtensa chip for testing
    sh Test-ESP8266.sh
    if [ $? -ne 0 ]
    then
        exit -1
    fi
elif matches $ARCHITECTURE "arm"
then
    echo "Run Tests available on arm"
else
    echo "arch found"
fi

