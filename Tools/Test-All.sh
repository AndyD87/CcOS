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

# Use STM32F4Discovery-Build for testing generic
sh Test-STM32F4Discovery.sh
if [ $? -ne 0 ]
then
    exit -1
fi
