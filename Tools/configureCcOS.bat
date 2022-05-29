REM ################################################################################
REM # This script can be used to setup all CcOS resources and configs.
REM # It will build and execute CcOS tools
REM ################################################################################
set CWD=%cd%
set CWD=%CWD:\=/%
set START_DIR=%CWD%
set TOOLSOUTPUT=%CWD%/CcOSTools
set SOLUTION_DIR=%CWD%/Solution
set CMAKE_DIR=%CWD%/../Sources
cd ..

REM ################################################################################
REM # Start building tools
mkdir "%SOLUTION_DIR:/=\%"
cd "%SOLUTION_DIR%"
cmake %CMAKE_DIR% -DCC_OUTPUT_DIR=%TOOLSOUTPUT%
cmake --build . --target CcOSBuildConfigApp

cd %TOOLSOUTPUT%
cd *
cd bin
set TOOLS_DIR=%cd%
set PATH=%PATH%;%TOOLS_DIR%
del /S /Q "%SOLUTION_DIR%"

REM ################################################################################
REM # Start configuration
CcOSBuildConfigApp.exe config2cmake "%CMAKE_DIR%/CMakeConfig/CcOSBuildConfig.xml" "%CMAKE_DIR%/CMakeConfig/CcOSBuildConfig.cmake"
mkdir "%SOLUTION_DIR:/=\%"
cd "%SOLUTION_DIR%"
cmake %CMAKE_DIR% -DCC_OUTPUT_DIR=%TOOLSOUTPUT%
cmake --build . --target CcOSBuildConfigApp
cd "%TOOLS_DIR%"
del /S /Q "%SOLUTION_DIR%"

REM # run build again to build changed ressources

REM ################################################################################
REM # Cleanup, but keep tools
del /S /Q "%SOLUTION_DIR%"
cd "%START_DIR%"
