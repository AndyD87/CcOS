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

# Test Windows Cross Compilation
sh Test-MinGw.sh
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
