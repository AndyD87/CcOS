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
$Global:PortableCygwinDownload  = "http://coolcow.de/projects/ThirdParty/CygwinPackage/binaries/1.0.0.0/cygwin.32bit.7z"
$Global:PortableCygwinName      = "Cygwin"
$Global:PortableCygwinBinDir    = "bin"

Function Cygwin-GetEnv
{
    PARAM(
        [switch]$Mandatory
    )
    
    # Test default location from orignal Cygwin-Setup x64
    if((Test-Path "C:\cygwin64\bin"))
    {
        $env:PATH += ";C:\cygwin64\bin"
        Write-Output "Cygwin found at C:\cygwin64\bin"
    }
    # Test default location from orignal Cygwin-Setup x86
    if((Test-Path "C:\cygwin\bin"))
    {
        $env:PATH += ";C:\cygwin\bin"
        Write-Output "Cygwin found at C:\cygwin\bin"
    }
    # Test default location on BuildSystem VM (x64)
    elseif((Test-Path "C:\Tools\cygwin64\bin"))
    {
        $env:PATH += ";C:\Tools\cygwin64\bin"
        Write-Output "Cygwin found at C:\Tools\cygwin64\bin"
    }
    # Test default location on BuildSystem VM (x86)
    elseif((Test-Path "C:\Tools\cygwin\bin"))
    {
        $env:PATH += ";C:\Tools\cygwin\bin"
        Write-Output "Cygwin found at C:\Tools\cygwin\bin"
    }
    elseif($Mandatory)
    {
        Write-Output "Mandatory Cygwin not found, try to download portable Version"
        Import-Module "$PSScriptRoot\PortableApps.ps1" -Force
        PortableApp-DownloadAndPath $Global:PortableCygwinDownload $Global:PortableCygwinName $Global:PortableCygwinBinDir
    }
    else
    {
        Write-Output "No Cygwin found"
    }
}
