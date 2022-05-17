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

# Import Portable Apps to receive, if required, all tools to powershell-common
Import-Module "$PSScriptRoot\..\Powershell-Common\PortableApps.ps1" -Force
Import-Module "$PSScriptRoot\..\Powershell-Common\Git.ps1" -Force
Import-Module "$PSScriptRoot\..\Powershell-Common\Process.ps1" -Force
Import-Module "$PSScriptRoot\..\Powershell-Common\VisualStudio.ps1" -Force

Git-GetEnv
git submodule init
git submodule update

$CurrentDir     = (Get-Item -Path ".\" -Verbose).FullName
cd $PSScriptRoot
cd Build-Openssl

$VisualStudioPostifx = VisualStudio-GetPostFix -VisualStudio $VisualStudio -Architecture $Architecture -Static $Static -DebugBuild $DebugBuild -StaticRuntime $StaticRuntime
$OutputDir = "$CurrentDir\..\..\Cache\openssl-$Version-$VisualStudioPostifx"

.\Make.ps1 -VisualStudio $VisualStudio -Architecture $Architecture -Version $Version -Static $Static -DebugBuild $DebugBuild -StaticRuntime $StaticRuntime -OverrideOutput $OutputDir

cd $CurrentDir
