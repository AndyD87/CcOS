for arg in "$@"
do
  if  [ "$arg" = "Release" ]; then
    echo "Select release"
    BUILD_TYPE=Release
  elif  [ "$arg" = "Debug" ]; then
    echo "Select debug"
    BUILD_TYPE=Debug
  elif  [ "$arg" = "clang" ]; then
    export CC=clang
    export CXX=clang++
  elif  [ "$arg" = "all" ]; then
    RUN_ALL="TRUE"
  fi
done

if  [ "$BUILD_TYPE" = "" ]; then
  echo "Default select release"
  BUILD_TYPE=Release
fi

if  [ "$COMPILER" = "" ]; then
  echo "Default select gcc"
  COMPILER=gcc
fi

run()
{
  cd ..
  mkdir Solution.$COMPILER.$BUILD_TYPE
  cd Solution.$COMPILER.$BUILD_TYPE
  cpack
  if [ $? -ne 0 ]
  then
      exit -1
  fi

  make -j $(nproc)
  if [ $? -ne 0 ]
  then
      exit -1
  fi

  cd ..
  cd Tools
}


if  [ "$RUN_ALL" = "TRUE" ]; then
  run
fi

run
