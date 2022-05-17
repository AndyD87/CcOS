
TOOLS_DIR=$(pwd)/..
cd ..
SOLUTION_POSTFIX=".Solution.ECL.Testing"
TARGET_DIR=$(pwd)
TARGET_DIR_FIXED="$TARGET_DIR$SOLUTION_POSTFIX"

runDebug()
{
  cd "$TOOLS_DIR"
  rm -rf "$TARGET_DIR_FIXED"
  mkdir "$TARGET_DIR_FIXED"
  cd "$TARGET_DIR_FIXED"
  cmake -G "Eclipse CDT4 - Unix Makefiles" "$TOOLS_DIR/.." -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=CMakeConfig/Boards/ST/$Board -DCMAKE_BUILD_TYPE=Debug
  if [ $? -ne 0 ]
  then
      exit -1
  fi

  make -j $CPU
  if [ $? -ne 0 ]
  then
      exit -1
  fi
}

runRelease()
{
  cd "$TOOLS_DIR"
  rm -rf "$TARGET_DIR_FIXED"
  mkdir "$TARGET_DIR_FIXED"
  cd "$TARGET_DIR_FIXED"

  cmake -G "Eclipse CDT4 - Unix Makefiles" "$TOOLS_DIR/.." -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=CMakeConfig/Boards/ST/$Board -DCMAKE_BUILD_TYPE=Release
  if [ $? -ne 0 ]
  then
      exit -1
  fi

  make -j $CPU
  if [ $? -ne 0 ]
  then
      exit -1
  fi
}

Board=STM32F103C8
runDebug
runRelease
Board=STM3220GEVAL
runDebug
runRelease
Board=STM32F3Discovery
runDebug
runRelease
Board=STM32F4Discovery
runDebug
runRelease
Board=STM32F407VET
runDebug
runRelease

cd "$TOOLS_DIR"
rm -rf "$TARGET_DIR_FIXED"
