#cd ..
#mkdir Solution.VC12
#cd Solution.VC12
#cmake ../ -G "Visual Studio 12" -DCMAKE_GENERATOR_PLATFORM=x64
#cmake.exe --build . --config Release
#ctest -C Release
#cd ..
#cd Tools
PARAM(
    [bool]$StopOnError = $false
)

function StartBuildProcess
{
    PARAM(
        $VisualStudio,
        $Architecture,
        $Configuration,
        $Static
    )
    
    $CurrentDir  = (Get-Item .\).FullName
    $TestLog     = $CurrentDir+"\Test.log" 
    $SolutionDir = $PSScriptRoot+"\Solution"
    $OutputDir   = $PSScriptRoot+"\Output"
    $CcOSRootDir = $PSScriptRoot+"\.."

    # Fist Clean Solution if Existing
    if((Test-Path $SolutionDir))
    {
        Remove-Item $SolutionDir -Recurse -Force
    }
    # Fist Clean Solution if Existing
    if((Test-Path $OutputDir))
    {
        Remove-Item $OutputDir -Recurse -Force
    }
    New-Item $SolutionDir -ItemType Directory
    New-Item $OutputDir -ItemType Directory
    cd $SolutionDir

    $VisualStudioString = $VisualStudio
    if($Architecture -eq "x64")
    {
        $VisualStudioString += " Win64"
    }
    try
    {
        $AppendCmake = ""
        if($Static -eq "Static")
        {
            $AppendCmake = "-DCCOS_LINK_TYPE=STATIC"
        }
        & "cmake.exe" "$CcOSRootDir" "-G" $VisualStudioString "-DCCOS_OUTPUT_DIR=`"$OutputDir`"" "$AppendCmake"
        if($LASTEXITCODE -ne 0)
        {
            cd $CurrentDir
            $Msg = "Failed: cmake generation with $VisualStudioString $Configuration $AppendCmake"
            Add-Content $TestLog $Msg
            throw $Msg
        }
        cmake.exe --build . --config $Configuration
        if($LASTEXITCODE -ne 0)
        {
            cd $CurrentDir
            $Msg = "Failed: cmake build with $VisualStudioString $Configuration $AppendCmake"
            Add-Content $TestLog $Msg
            throw $Msg
        }
        ctest --output-on-failure -C $Configuration
        if($LASTEXITCODE -ne 0)
        {
            cd $CurrentDir
            $Msg = "Failed: ctest with $VisualStudioString $Configuration $AppendCmake"
            Add-Content $TestLog $Msg
            throw $Msg
        }
        Add-Content $TestLog "Success: $VisualStudioString $Configuration $AppendCmake"
    }
    finally
    {
        if($StopOnError){ throw $Msg }
        cd $CurrentDir
    }
}

$VisualStudios = @("Visual Studio 12", "Visual Studio 14", "Visual Studio 15") #
$Architectures  = @("win32", "x64")
$Configurations = @("Debug", "Release") # Not required but possible to test : "RelWithDebInfo", "MinSizeRel")
$Statics = @("Static", "Shared")
    
$CurrentDir  = (Get-Item .\).FullName
$TestLog     = $CurrentDir+"\Test.log" 
if((Test-Path $TestLog))
{
    Remove-Item $TestLog
}

foreach($VisualStudio in $VisualStudios)
{
    foreach($Architecture in $Architectures)
    {
        foreach($Configuration in $Configurations)
        {
            foreach($Static in $Statics)
            {
                try
                {
                    StartBuildProcess $VisualStudio $Architecture $Configuration $Static
                }
                catch
                {}
            }
        }
    }
}
# ExampleCall StartBuildProcess "Visual Studio 12" "win32" "Release" "Shared"
# ExampleCall StartBuildProcess "Visual Studio 12" "x64" "Debug" "Static"