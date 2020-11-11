################################################################################
# This script can be used to setup all CcOS resources and configs.
# It will build and execute CcOS tools
################################################################################
CWD=$(pwd)
START_DIR=$CWD
TOOLSOUTPUT=$CWD/CcOSTools
SOLUTION_DIR=$CWD/Solution
CMAKE_DIR=$CWD/..
cd ..

################################################################################
# Start building tools
mkdir $SOLUTION_DIR
cd $SOLUTION_DIR
cmake $CMAKE_DIR -DCC_OUTPUT_DIR=$TOOLSOUTPUT
make -j $(nproc)

cd $TOOLSOUTPUT
# Enter the only build directory
cd *
cd bin
TOOLS_DIR=$(pwd)
export PATH=$PATH:$TOOLS_DIR
rm -rf $SOLUTION_DIR

################################################################################
# Start configuration
./CcOSBuildConfigApp config2cmake $CMAKE_DIR/CMakeConfig/CcOSBuildConfig.xml $CMAKE_DIR/CMakeConfig/CcOSBuildConfig.cmake

mkdir $SOLUTION_DIR
cd $SOLUTION_DIR
cmake $CMAKE_DIR -DCC_OUTPUT_DIR=$TOOLSOUTPUT
make -j $(nproc)
cd $TOOLS_DIR
rm -rf $SOLUTION_DIR

# run build again to build changed ressources

################################################################################
# Cleanup, but keep tools
rm -rf $SOLUTION_DIR
cd $START_DIR
