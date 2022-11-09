@echo off
echo ################################################################################
echo # This script can be used to setup all CcOS resources and configs.
echo # It will build and execute CcOS tools
echo ################################################################################
set CWD=%cd%
set CWD=%CWD:\=/%
set START_DIR=%CWD%
set TOOLSOUTPUT=%CWD%/CcOSTools
set SOLUTION_DIR=%CWD%/Solution
set CMAKE_DIR=%CWD%/../Sources
cd ..

echo ################################################################################
echo # Start building tools
mkdir "%SOLUTION_DIR:/=\%"
cd "%SOLUTION_DIR%"
cmake %CMAKE_DIR% -DCC_OUTPUT_DIR=%TOOLSOUTPUT% -DCMAKE_BUILD_TYPE=Release
cmake --build . --target CcOSBuildConfigApp --config Release

cd %TOOLSOUTPUT%
cd msv*
cd bin
set TOOLS_DIR=%cd%
set PATH=%PATH%;%TOOLS_DIR%
echo del /S /Q "%SOLUTION_DIR%"

echo ################################################################################
echo # Start configuration in %TOOLS_DIR%

CcOSBuildConfigApp.exe config2cmake "%CMAKE_DIR%/CMakeConfig/CcOSBuildConfig.xml" "%CMAKE_DIR%/CMakeConfig/CcOSBuildConfig.cmake"
mkdir "%SOLUTION_DIR:/=\%"
cd "%SOLUTION_DIR%"
cmake %CMAKE_DIR% -DCC_OUTPUT_DIR=%TOOLSOUTPUT%
cmake --build . --target CcOSBuildConfigApp
cd "%TOOLS_DIR%"
del /S /Q "%SOLUTION_DIR%"

echo # run build again to build changed ressources

echo ################################################################################
echo # Cleanup, but keep tools
del /S /Q "%SOLUTION_DIR%"
cd "%START_DIR%"
