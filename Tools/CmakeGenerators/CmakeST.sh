
printHelp()
{
  echo "Configure ST Microcontrollers with cmake"
  echo "Usage: Cmake.sh CPU|BOARD"
  echo ""
  echo "CPU-Boards:"
  echo "    STM32F103C8"
  echo "    STM3220GEVAL"
  echo "    STM32F3Discovery"
  echo "    STM32F4Discovery"
  echo "    STM32F407VET"
  echo ""
  echo "CPU-Types:"
}

runCmake()
{
  TOOLS_DIR=$(pwd)
  cd ..
  cd ..
  SOLUTION_POSTFIX=".Solution.ECL.$CMAKE_BOARD"
  TARGET_DIR=$(pwd)
  TARGET_DIR_FIXED="$TARGET_DIR$SOLUTION_POSTFIX"

  #rm -rf "$TARGET_DIR_FIXED"
  mkdir "$TARGET_DIR_FIXED"
  cd "$TARGET_DIR_FIXED"
  cmake -G "Eclipse CDT4 - Unix Makefiles" "$TOOLS_DIR/../.." -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=CMakeConfig/Boards/ST/$CMAKE_BOARD -DCMAKE_BUILD_TYPE=Debug
  if [ $? -ne 0 ]
  then
    exit -1
  elif [ "$1" = "BUILD"  ]
    then
    make -j $CPU
    if [ $? -ne 0 ]
    then
        exit -1
    fi
  else
    echo "Successfully generated $2"
  fi
  cd $(pwd)
}

if [ $# -eq 0 ]
  then
    printHelp
  else
    if [ $1 = "STM3220GEVAL" ]
      then
        CMAKE_BOARD=STM3220GEVAL
        runCmake $2
      elif [ $1 = "STM32F3Discovery" ]
        then
          CMAKE_BOARD=STM32F3Discovery
          runCmake $2
      elif [ $1 = "STM32F407VET" ]
        then
          CMAKE_BOARD=STM32F407VET
          runCmake $2
      elif [ $1 = "STM32F4Discovery" ]
        then
          CMAKE_BOARD=STM32F4Discovery
          runCmake $2
      elif [ $1 = "STM32F103C8" ]
        then
          CMAKE_BOARD=STM32F103C8
          runCmake $2
      else
        echo "Invalid argument: $1"
        printHelp
    fi
fi
