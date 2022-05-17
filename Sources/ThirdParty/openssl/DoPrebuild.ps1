##
# This file is part of Powershell-Common, a collection of powershell-scrips
# 
# Copyright (c) 2017 Andreas Dirmeier
# License   MIT
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
##

# Import Portable Apps to receive, if required, all tools to powershell-common
Import-Module "$PSScriptRoot\..\Powershell-Common\PortableApps.ps1" -Force
Import-Module "$PSScriptRoot\..\Powershell-Common\Git.ps1" -Force
Import-Module "$PSScriptRoot\..\Powershell-Common\Process.ps1" -Force
Import-Module "$PSScriptRoot\..\Powershell-Common\VisualStudio.ps1" -Force

Git-GetEnv
git submodule init
git submodule update

$OpensslVersion = "1.1.0.5"

Function CreateMakeSession
{
    PARAM(
        [Parameter(Mandatory=$true, Position=1)]
        [string]$VisualStudio,
        [Parameter(Mandatory=$true, Position=2)]
        [string]$Architecture,
        [Parameter(Mandatory=$true, Position=3)]
        [string]$Version,
        [Parameter(Mandatory=$false, Position=4)]
        [bool]$DebugBuild = $false,
        [bool]$Static = $false,
        [bool]$StaticRuntime = $false,
        [bool]$NoClean = $false,
        [string]$OverrideDir=""
    )
    $VisualStudioPostifx = VisualStudio-GetPostFix -VisualStudio $VisualStudio -Architecture $Architecture -Static $Static -DebugBuild $DebugBuild -StaticRuntime $StaticRuntime
    $OutputDir  = $OverrideDir + "\openssl-" + $Version + "-" + $VisualStudioPostifx
    $RenameFrom = $OverrideDir + "\openssl-" + $Version + "-" + $VisualStudioPostifx + ".7z"
    $RenameTo   = $OverrideDir + "\openssl-" + $VisualStudioPostifx + ".7z"
    $sStatic = "0"
    if($Static)
    {
        $sStatic = "1";
    }
    $sStaticRuntime = "0"
    if($StaticRuntime)
    {
        $sStaticRuntime = "1";
    }
    $sDebugBuild = "0"
    if($DebugBuild)
    {
        $sDebugBuild = "1";
    }
    $sNoClean = "0"
    if($NoClean)
    {
        $sNoClean = "1";
    }
    $Cmd = "-command .\Make.ps1 -Version `"$Version`" -VisualStudio `"$VisualStudio`" -Architecture `"$Architecture`" -Static $sStatic -StaticRuntime $sStaticRuntime -DebugBuild $sDebugBuild -OverrideOutput `"$OutputDir`" -NoClean $sNoClean -DoPackage 1"

    if((Process-StartInline "powershell.exe" $Cmd) -ne 0)
    {
        $Msg = "Failed: make with openssl-" + $VisualStudioPostifx
        Add-Content $TestLog $Msg
        Move-Item $RenameFrom $RenameTo
        throw "Make command failed: powershell.exe $Cmd"
    }
    Add-Content $TestLog "Success: openssl-" + $VisualStudioPostifx
}

$CurrentDir     = (Get-Item -Path ".\" -Verbose).FullName
$Global:TestLog = "$CurrentDir\BuildLog.txt"

cd $PSScriptRoot
cd Build-Openssl

$VisualStudios  = @("2013", "2015", "2017")
$Architectures  = @("x64", "x86")
$Configurations = @($false, $true) # Debug enabled
$Statics        = @($true, $false)

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
                    CreateMakeSession -VisualStudio $VisualStudio -Architecture $Architecture -DebugBuild $Configuration -Static $Static -Version $OpensslVersion -StaticRuntime $true -OverrideDir "$CurrentDir\..\..\Cache"
                }
                catch
                {
                }
            }
        }
    }
}

cd $CurrentDir
