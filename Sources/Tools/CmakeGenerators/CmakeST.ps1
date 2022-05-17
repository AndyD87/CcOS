Param
(
  [string]$1 = "",
  [string]$2 = ""
)

cd $PSScriptRoot

$ParamsValid = $false
if($1 -ne "")
{
  $ParamsValid = $true;
}
if($2 -eq "")
{
    $2 = ""
}

Function printHelp()
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

Function runCmake()
{
  $TOOLS_DIR=(Get-Item .).FullName
  cd ..
  cd ..
  $SOURCES_DIR=(Get-Item .).FullName
  cd ..
  $SOLUTION_POSTFIX="Generic.Solution.ECL.$CMAKE_BOARD"
  $TARGET_DIR_FIXED=(Get-Item .).FullName + "/$SOLUTION_POSTFIX"

  #rm -rf "$TARGET_DIR_FIXED"
  if((Test-Path $TARGET_DIR_FIXED) -ne $true)
  {
    mkdir "$TARGET_DIR_FIXED"
  }
  cd "$TARGET_DIR_FIXED"
  cmake -G "Eclipse CDT4 - Unix Makefiles" "$SOURCES_DIR" "-DCMAKE_ECLIPSE_VERSION=4.9" "-DCCOS_BOARD=${CMAKE_BOARD}" "-DCMAKE_BUILD_TYPE=Debug"
  if($LASTEXITCODE -ne 0)
  {
    cd $TOOLS_DIR
    exit -1
  }
  elseif("$2" -eq "BUILD")
  {
    cmake --build .
    if($LASTEXITCODE -ne 0)
    {
      cd $TOOLS_DIR
      exit -1
    }
  }
  else
  {
    echo "Successfully generated $2"
  }
  cd $TOOLS_DIR
}

if($ParamsValid -eq $false)
{
  printHelp
}
elseif ( $1 -eq "STM3220GEVAL" )
{
    $CMAKE_BOARD="STM3220GEVAL"
    runCmake $2
}
elseif ( $1 -eq "STM32F3Discovery" )
{
    $CMAKE_BOARD="STM32F3Discovery"
    runCmake $2
}
elseif ( $1 -eq "STM32F407VET" )
{
    $CMAKE_BOARD="STM32F407VET"
    runCmake $2
}
elseif ( $1 -eq "STM32F4Discovery" )
{
    $CMAKE_BOARD="STM32F4Discovery"
    runCmake $2
}
elseif ( $1 -eq "STM32F103C8" )
{
    $CMAKE_BOARD="STM32F103C8"
    runCmake $2
}
else
{
    echo "Invalid argument: $1"
    printHelp
}
