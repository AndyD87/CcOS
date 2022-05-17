SOLUTION_POSTFIX="Testing.Solution.ECL.Testing"
TEST_DIR=$(pwd)/../../../Sources
TEST_DIR_FIXED=$(pwd)/../../../$SOLUTION_POSTFIX

runDebug()
{
  cd "$TEST_DIR"
  rm -rf "$TEST_DIR_FIXED"
  mkdir "$TEST_DIR_FIXED"
  cd "$TEST_DIR_FIXED"
  cmake -G "Eclipse CDT4 - Unix Makefiles" "$TEST_DIR" -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=$Board -DCMAKE_BUILD_TYPE=Debug
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
  cd "$TEST_DIR"
  rm -rf "$TEST_DIR_FIXED"
  mkdir "$TEST_DIR_FIXED"
  cd "$TEST_DIR_FIXED"

  cmake -G "Eclipse CDT4 - Unix Makefiles" "$TEST_DIR" -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=$Board -DCMAKE_BUILD_TYPE=Release
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

cd "$TEST_DIR"
rm -rf "$TEST_DIR_FIXED"
