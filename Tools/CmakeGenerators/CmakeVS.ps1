PARAM(
    [String]
    [ValidateSet("2015", "2017", "2019")]
    $Version = $null,
    [String]
    [ValidateSet("x64", "x86")]
    $Architecture = $null,
    [String]
    [ValidateSet($null, $true, $false)]
    $Static = $null,
    [String]
    [ValidateSet("Release", "Debug")]
    $Config = $null
)

if($PSScriptRoot)
{
    cd $PSScriptRoot
}

Import-Module ..\..\ThirdParty\Powershell-Common\VisualStudio.ps1

##
# Search for available versions
##
$VisualStudios = @()
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

if([string]::IsNullOrEmpty($Version))
{
    Write-Host "No Visual Studio version set"
    Write-Host "Press enter for newest or type one of the available:"
    foreach($VisualStudio in $VisualStudios)
    {
        Write-Host ("    " + $VisualStudio.Year)
    }
    # Do query and validate
    $VisualStudio = $VisualStudios[0]
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
    $Architecture = "x64"
}

if($Static -eq $false -or $Static -eq $true)
{}
else
{
    Write-Host "No Static set"
    Write-Host "Press enter for auto or type one of the available:"
    Write-Host ("    true")
    Write-Host ("    false")
    # Do query and validate
    $Static = $false
}

if([string]::IsNullOrEmpty($Config))
{
    Write-Host "No Static set"
    Write-Host "Press enter for auto or type one of the available:"
    Write-Host ("    Release")
    Write-Host ("    Debug")
    # Do query and validate
    $Config = "Debug"
}

if($Static)
{
    $StaticString = "Static"
}
else
{
    $StaticString = "Shared"
}

$sPostfix = VisualStudio-GetPostFix $VisualStudio.Year $Architecture $Static ($Config -eq "Debug") $true

$SolutionDir = "..\..\Solution." + $VisualStudio.Year + ".$Architecture.$StaticString.$Config"

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
        $Paramters = $VisualStudio.CmakeX64
    }
    "x86"
    {
        $Paramters = $VisualStudio.CmakeX86
    }
}

$Paramters += "../"

if($Static)
{
    $Paramters += "-DCC_LINK_TYPE=STATIC"
}

Start-Process cmake -ArgumentList $Paramters -Wait -WorkingDirectory .\
