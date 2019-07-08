PARAM(
    [bool]$StopOnError = $true,
    [bool]$DoVisualStudio = $true,
    [bool]$DoMinGw = $true,
    [bool]$KeepOutput = $false
)

$Global:KeepOutput = $KeepOutput

Function RemoveDirs
{
    $SolutionDir = $PSScriptRoot+"\Solution"
    $OutputDir   = $PSScriptRoot+"\Output"
    # Fist Clean Solution if Existing
    if((Test-Path $SolutionDir))
    {
        Remove-Item $SolutionDir -Recurse -Force
    }
    # Fist Clean Solution if Existing
    if( (Test-Path $OutputDir) -eq $true -and 
        $Global:KeepOutput -eq $false)
    {
        Remove-Item $OutputDir -Recurse -Force
    }
}

Function ResetDirs
{
    $SolutionDir = $PSScriptRoot+"\Solution"
    $OutputDir   = $PSScriptRoot+"\Output"
    RemoveDirs
    New-Item $SolutionDir -ItemType Directory
    if((Test-Path $OutputDir) -eq $false)
    {
        New-Item $OutputDir -ItemType Directory
    }
}

function StartBuildProcess
{
    PARAM(
        $VisualStudio,
        $Architecture,
        $Configuration,
        $Static
    )

    # Enable parallel build
    $ENV:CL="/MP"

    $CurrentDir  = (Get-Item .\).FullName
    $TestLog     = $CurrentDir+"\Test.log" 
    $CcOSRootDir = $PSScriptRoot+"\.."
    $SolutionDir = $PSScriptRoot+"\Solution"
    $OutputDir   = $PSScriptRoot+"\Output"
    ResetDirs
    
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
        & "cmake.exe" "$CcOSRootDir" "-G" $VisualStudioString "-DCC_OUTPUT_DIR=`"$OutputDir`"" "$AppendCmake" "$AppendCmake2"
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
    cd $CurrentDir
    RemoveDirs
}

Function Test-VisualStudio()
{
    $VisualStudios = @()
    if((Test-Path "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat") -and
        (Test-Path "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\cl.exe")) # Compiler
    {
        $VisualStudios += "Visual Studio 14";
    }
    if((Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe") -and
       (Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\2017"))
    {
        $VisualStudios += "Visual Studio 15 2017";
    }
    if((Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe") -and
       (Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\2019"))
    {
        $VisualStudios += "Visual Studio 16 2019";
    }

    $Architectures  = @("win32", "x64")
    $Configurations = @("Release", "Debug") # Not required but possible to test : "RelWithDebInfo", "MinSizeRel")
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
                    # ExampleCall StartBuildProcess "Visual Studio 12" "win32" "Release" "Shared"
                    # ExampleCall StartBuildProcess "Visual Studio 12" "x64" "Debug" "Static"
                    StartBuildProcess $VisualStudio $Architecture $Configuration $Static
                }
            }
        }
    }
}

Function Test-MinGW()
{
    $oProcInfos = Get-WmiObject -class Win32_processor
    $iCores = 1
    foreach($oProcInfo in $oProcInfos)
    {
        $iCores = $oProcInfo.NumberOfCores
    }
    
    $CurrentDir  = (Get-Item .\).FullName
    $TestLog     = $CurrentDir+"\Test.log" 
    $CcOSRootDir = $PSScriptRoot+"\.."
    $SolutionDir = $PSScriptRoot+"\Solution"
    $OutputDir   = $PSScriptRoot+"\Output"
    ResetDirs
    
    $Architectures  = @("x86", "x64")
    $Configurations = @("Debug", "Release")
    foreach($Architecture in $Architectures)
    {
        foreach($Configuration in $Configurations)
        {
            cd $CurrentDir
            ResetDirs
            cd $SolutionDir

            cmake.exe "$CcOSRootDir" "-G" "`"MinGW Makefiles`"" "-DCCOS_BOARD=CMakeConfig/Boards/MinGW" "-DCMAKE_BUILD_TYPE=$Configuration" "-DCC_OUTPUT_DIR=`"$OutputDir`"" "-DCMAKE_SYSTEM_PROCESSOR=`"$Architecture`""
            if($LASTEXITCODE -ne 0)
            {
                cd $CurrentDir
                $Msg = "Failed: cmake generation of MinGW $Configuration failed"
                Add-Content $TestLog $Msg
                throw $Msg
            }
            cmake.exe --build . --config $Configuration -- "-j$iCores"
            if($LASTEXITCODE -ne 0)
            {
                cd $CurrentDir
                $Msg = "Failed: cmake build wit MinGW $Configuration failed"
                Add-Content $TestLog $Msg
                throw $Msg
            }
            ctest --output-on-failure -C $Configuration
            if($LASTEXITCODE -ne 0)
            {
                cd $CurrentDir
                $Msg = "Failed: ctest with MinGW $Configuration failed"
                Add-Content $TestLog $Msg
                throw $Msg
            }
            Add-Content $TestLog "Success: MinGW $Configuration"
        }
    }
    cd $CurrentDir
    RemoveDirs
}

if($DoVisualStudio)
{
    Test-VisualStudio
}

if($DoMinGw)
{
    Test-MinGW
}
