PARAM(
    [bool]$StopOnError = $true
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
    $CcOSRootDir = $PSScriptRoot+"\.."

    # Fist Clean Solution if Existing
    if((Test-Path $SolutionDir))
    {
        Remove-Item $SolutionDir -Recurse -Force
    }
    New-Item $SolutionDir -ItemType Directory
    cd $SolutionDir

    $VisualStudioString = $VisualStudio
    if($Architecture -eq "x64")
    {
        $VisualStudioString += " Win64"
    }
    try
    {
        $AppendCmake = ""
        $AppendCmake2 = ""
        if($Static -eq "Static")
        {
            $AppendCmake = "-DCC_LINK_TYPE=STATIC"
        }
        if($Configuration -eq "Release")
        {
            $AppendCmake2 = "-DCC_WARNING_AS_ERROR=TRUE"
        }
        & "cmake.exe" "$CcOSRootDir" "-G" $VisualStudioString "$AppendCmake" "$AppendCmake2"
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
    catch
    {
        cd $CurrentDir
        if($StopOnError)
        { 
            throw $Msg 
        }

    }
    finally
    {
        cd $CurrentDir
        if((Test-Path $SolutionDir))
        {
            Remove-Item $SolutionDir -Recurse -Force
        }
    }
}

# Test all available VS Versions
$VisualStudios = @()
if(Test-Path "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat")
{
    $VisualStudios += "Visual Studio 12";
}
if(Test-Path "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat")
{
    $VisualStudios += "Visual Studio 14";
}
if(Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe")
{
    $VisualStudios += "Visual Studio 15";
}

$Architectures  = @("win32", "x64")
$Configurations = @("Release", "Debug") # Not required but possible to use: , "RelWithDebInfo", "MinSizeRel"
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
                StartBuildProcess $VisualStudio $Architecture $Configuration $Static
            }
        }
    }
}