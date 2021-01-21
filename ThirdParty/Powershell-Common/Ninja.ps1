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
$Global:PortableNinjaDownload  = "http://coolcow.de/projects/ThirdParty/Ninja/binaries/1.7.2.0/Ninja.zip"
$Global:PortableNinjaName      = "Ninja"
$Global:PortableNinjaBinDir    = "\"

Function Ninja-GetEnv
{
    PARAM(
        [switch]$Mandatory
    )

    if(Get-Command ninja.exe -ErrorAction SilentlyContinue)
    {
        Write-Output "Ninja already in PATH"
    }
    elseif((Test-Path "C:\Tools\ninja-win-1.7.2"))
    {
        $env:PATH += ";C:\Tools\ninja-win-1.7.2"
        Write-Output "Ninja found at C:\Tools\ninja-win-1.7.2"
    }
    elseif($Mandatory)
    {
        Write-Output "Mandatory Ninja not found, try to download portable Version"
        Import-Module "$PSScriptRoot\PortableApps.ps1" -Force
        PortableApp-DownloadAndPath $Global:PortableNinjaDownload $Global:PortableNinjaName $Global:PortableNinjaBinDir
    }
    else
    {
        Write-Output "No Ninja found"
    }
}