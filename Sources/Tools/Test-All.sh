###############################################################################
# Test All available build options for CcOS
###############################################################################
matches() {
    input="$1"
    pattern="$2"
    echo "$input" | grep -q "$pattern"
}

cd Testing

export PREBUILD_REQUIRED=1
export CTEST_OUTPUT_ON_FAILURE=1
export TEST_CCOS=TRUE

# Test gcc build
sh Test-Gcc.sh
if [ $? -ne 0 ]
then
    exit -1
    echo Test-Gcc.sh failed >> Test.log
fi
echo Test-Gcc.sh succeeded > Test.log

# Test clang build
sh Test-Clang.sh
if [ $? -ne 0 ]
then
    exit -1
    echo Test-Clang.sh failed >> Test.log
fi
echo Test-Clang.sh succeeded >> Test.log

ARCHITECTURE=$(uname -m)
if matches $ARCHITECTURE "x64" || matches $ARCHITECTURE "x86"
then
    echo "Run Tests on x64 and x86 available"

    # Test Windows Cross Compilation
    sh Test-MinGW.sh
    if [ $? -ne 0 ]
    then
        exit -1
        echo Test-MinGW.sh failed >> Test.log
    fi
    echo Test-MinGW.sh succeeded >> Test.log

    # Use STM32 Boards
    sh Test-ST.sh
    if [ $? -ne 0 ]
    then
        exit -1
        echo Test-ST.sh failed >> Test.log
    fi
    echo Test-ST.sh succeeded >> Test.log

    # Use OrangePi Boards
    sh Test-OrangePi.sh
    if [ $? -ne 0 ]
    then
        exit -1
        echo Test-OrangePi.sh failed >> Test.log
    fi
    echo Test-OrangePi.sh succeeded >> Test.log
elif matches $ARCHITECTURE "arm"
then
    echo "Run cross compile tests not available on arm"
else
    echo "Run cross compile tests not available on current architecture"
fi
