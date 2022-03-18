PARAM(
    [String]
    [ValidateSet("2015", "2017", "2019", "2022")]
    $Version = $null,
    [String]
    [ValidateSet("x64", "x86")]
    $Architecture = $null,
    [bool]
    [ValidateSet($true, $false)]
    $Static = $false,
    [bool]
    [ValidateSet($true, $false)]
    $DoBuild = $false,
    [bool]
    [ValidateSet($true, $false)]
    $TestEnv = $false,
    [bool]
    [ValidateSet($true, $false)]
    $DoTest = $false,
    [bool]
    [ValidateSet($true, $false)]
    $DoPackage = $false,
    [bool]
    [ValidateSet($true, $false)]
    $Run = $false
)

if($PSScriptRoot)
{
    cd $PSScriptRoot
}

Import-Module ..\..\ThirdParty\Powershell-Common\VisualStudio.ps1

if($TestEnv)
{
    $ENV:TEST_CCOS="TRUE"
}

##
# Search for available versions
##
$VisualStudios = @()
if((Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe") -and
    (Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\2022"))
{
    $VisualStudio        = @{}
    $VisualStudio.Year   = "2022"
    $VisualStudio.Full   = "Visual Studio 17 2022"
    $VisualStudio.CmakeX86  = @("-G", "`"Visual Studio 17 2022`"")
    $VisualStudio.CmakeX64  = @("-G", "`"Visual Studio 17 2022`"", "-A", "x64")
    $VisualStudios      += $VisualStudio
}
if((Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe") -and
    (Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\2019"))
{
    $VisualStudio           = @{}
    $VisualStudio.Year      = "2019"
    $VisualStudio.Full      = "Visual Studio 16 2019"
    $VisualStudio.CmakeX86  = @("-G", "`"Visual Studio 16 2019`"")
    $VisualStudio.CmakeX64  = @("-G", "`"Visual Studio 16 2019`"", "-A", "x64")
    $VisualStudios          += $VisualStudio
}
if((Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe") -and
    (Test-Path "C:\Program Files (x86)\Microsoft Visual Studio\2017"))
{
    $VisualStudio        = @{}
    $VisualStudio.Year   = "2017"
    $VisualStudio.Full   = "Visual Studio 15 2017"
    $VisualStudio.CmakeX86  = @("-G", "`"Visual Studio 15 2017`"")
    $VisualStudio.CmakeX64  = @("-G", "`"Visual Studio 15 2017 Win64`"")
    $VisualStudios      += $VisualStudio
}
if( (Test-Path "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat") -and
    (Test-Path "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe") -and # IDE
    (Test-Path "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\cl.exe"))              # Compiler
{
    $VisualStudio        = @{}
    $VisualStudio.Year   = "2015"
    $VisualStudio.Full   = "Visual Studio 14 2015"
    $VisualStudio.CmakeX86  = @("-G", "`"Visual Studio 14 2015`"")
    $VisualStudio.CmakeX64  = @("-G", "`"Visual Studio 14 2015 Win64`"")
    $VisualStudios      += $VisualStudio
}

if($VisualStudios.Count -eq 0)
{
    throw "No Visual Studio found"
}
elseif($Run -eq $true)
{
    # Do not forward to recursive call
    $Run = $false;
    #if test or package is aktive, build is required.
    if($DoTest -or $DoPackage)
    {
        $DoBuild = $true;
    }
    try
    {
        foreach($VS in $VisualStudios)
        {
            cd $PSScriptRoot
            .\CmakeVS.ps1 -Version $VS.Year -Architecture x64 -Static $True  -DoBuild $DoBuild -DoTest $DoTest -DoPackage $DoPackage -Run $False
            cd $PSScriptRoot
            .\CmakeVS.ps1 -Version $VS.Year -Architecture x64 -Static $False -DoBuild $DoBuild -DoTest $DoTest -DoPackage $DoPackage -Run $False
            cd $PSScriptRoot
            .\CmakeVS.ps1 -Version $VS.Year -Architecture x86 -Static $True  -DoBuild $DoBuild -DoTest $DoTest -DoPackage $DoPackage -Run $False
            cd $PSScriptRoot
            .\CmakeVS.ps1 -Version $VS.Year -Architecture x86 -Static $False -DoBuild $DoBuild -DoTest $DoTest -DoPackage $DoPackage -Run $False
        }
        exit 0
    }
    catch
    {
        cd $PSScriptRoot
        Write-Host $_.Exception.Message
        exit -1
    }
}

if([string]::IsNullOrEmpty($Version))
{
    Write-Host "No Visual Studio version set"
    Write-Host "Press enter for newest or type one of the available:"
    foreach($VisualStudio in $VisualStudios)
    {
        Write-Host ("    " + $VisualStudio.Year)
    }
    # Do query and validate
    do
    {
        $Input = Read-Host
        if($Input -eq "")
        {
            $VisualStudio = $VisualStudios[0]
            $Version = $VisualStudio.Year
        }
        else
        {
            foreach($VisualStudio in $VisualStudios)
            {
                if($VisualStudio.Year -eq $Input)
                {
                    $Version = $VisualStudio.Year
                    break;
                }
            }
            if([string]::IsNullOrEmpty($Version))
            {
                Write-Host "Version not found, please try again:"
            }
        }
    } while([string]::IsNullOrEmpty($Version))
}
else
{
    $VisualStudio = $null
    foreach($VisualStudio in $VisualStudios)
    {
        if($VisualStudio.Year -eq $Version)
        {
            break
        }
    }
    if($VisualStudio.Year -ne $Version)
    {
        throw "Visual Studio Version not available or invalid"
    }
}

if([string]::IsNullOrEmpty($Architecture))
{
    Write-Host "No Architecture set"
    Write-Host "Press enter for auto or type one of the available:"
    Write-Host ("    x64")
    Write-Host ("    x86")
    # Do query and validate
    do
    {
        $Input = Read-Host
        if($Input -eq "")
        {
            $Architecture = "x64"
        }
        else
        {
            switch($Input)
            {
                "x64"
                {
                    $Architecture = "x64"
                }
                "x86"
                {
                    $Architecture = "x86"
                }
                default
                {
                    $Architecture = ""
                }
            }
            if([string]::IsNullOrEmpty($Architecture))
            {
                Write-Host "Architecture not found, please try again:"
            }
        }
    } while([string]::IsNullOrEmpty($Architecture))
}

if($Static -eq $true)
{
    $StaticString = "Static"
}
else
{
    $StaticString = "Shared"
}

$BuildTypes = @("Debug","Release")

foreach($BuildType in $BuildTypes)
{
    $SolutionDir = "..\..\Solution." + $VisualStudio.Year + ".$Architecture.$StaticString"

    if((Test-Path $SolutionDir) -ne $true)
    {
        New-Item -ItemType Directory $SolutionDir
    }

    cd $SolutionDir

    $Paramters = @()
    switch($Architecture)
    {
        "x64"
        {
            $Paramters += $VisualStudio.CmakeX64
        }
        "x86"
        {
            $Paramters += $VisualStudio.CmakeX86
        }
    }

    $Paramters += "../"

    if($Static)
    {
        $Paramters += "-DCC_LINK_TYPE=STATIC"
    }

    Function RunCommand
    {
        PARAM( $Exe, $Arguments)
    
        $Params =  $Arguments -join " "
        $Params = "$Exe " + $Params

        Write-Host "********************************************************************************"
        Write-Host "* Next execution:"
        Write-Host "*     $Params"
        Write-Host "********************************************************************************"
        Invoke-Expression $Params

        if( $LASTEXITCODE -ne 0 )
        {
            throw "$Exe failed"
        }

    }

    RunCommand cmake $Paramters

    if($LASTEXITCODE -ne 0)
    {
        throw "CMake failed"
    }

    if($DoBuild)
    {
        $BuildParam = @("--build", "./", "--config", $BuildType)
        RunCommand cmake $BuildParam
    }

    if($DoTest)
    {
        $BuildParam = @("./", "-C", $BuildType, "--output-on-failure")
        RunCommand ctest $BuildParam
    }

    if($DoPackage)
    {
        $BuildParam = @("-C", $BuildType)
        RunCommand cpack $BuildParam
    }

    cd $PSScriptRoot
}
