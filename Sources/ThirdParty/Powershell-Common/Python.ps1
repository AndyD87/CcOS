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
Import-Module "$PSScriptRoot\Process.ps1" -Force

$Global:PortablePython27Download  = "http://coolcow.de/projects/ThirdParty/WinPython/binaries/2.7.13.1/WinPython.32bit.zip"
$Global:PortablePython27Name      = "WinPython-2.7"
$Global:PortablePython27BinDir    = "python-2.7.13"

$Global:PortablePython36Download  = "http://coolcow.de/projects/ThirdParty/WinPython/binaries/3.6.2.0/WinPython.32bit.zip"
$Global:PortablePython36Name      = "WinPython-3.6"
$Global:PortablePython36BinDir    = "python-3.6.2"

Function Python-GetEnv
{
    PARAM(
        [Parameter(Mandatory=$False, Position=1)]
        [string]$Version = "3.6",
        [switch]$Mandatory
    )
    
    $bPythonFound = $false
    if(Get-Command python.exe -ErrorAction SilentlyContinue)
    {
        # Ignore microsoft store python exe!!!
        $sOutput = python --version
        if($LASTEXITCODE -eq 0)
        {
        Write-Output "Python already in PATH"
            $bPythonFound = $true
        }
    }
    if($bPythonFound -eq $False)
    {
        $VTarget = New-Object System.Version($Version)
        if($VTarget.Major -eq 2)
        {
            # Test default location from BuildSystem
            if((Test-Path "C:\Tools\Python27"))
            {
                $env:PATH += ";C:\Tools\Python27"
                Write-Output "Python found at C:\Tools\Python27"
            }
            # Test default location from orignal Python-Setup 2.7.x
            elseif((Test-Path "C:\Python27"))
            {
                $env:PATH += ";C:\Python27"
                Write-Output "Python found at C:\Python27"
            }
            elseif($Mandatory)
            {
                Write-Output "Mandatory Python not found, try to download portable Version"
                Import-Module "$PSScriptRoot\PortableApps.ps1" -Force
                PortableApp-DownloadAndPath $Global:PortablePython27Download $Global:PortablePython27Name $Global:PortablePython27BinDir
            }
            else
            {
                Write-Output "No Python found"
            }
        }

        if($VTarget.Major -eq 3)
        {
            # Test default location from BuildSystem
            if((Test-Path "C:\Tools\Python36"))
            {
                $env:PATH += ";C:\Tools\Python36"
                Write-Output "Python found at C:\Tools\Python36"
            }
            # Test default location from orignal Python-Setup 3.6.x
            elseif((Test-Path "C:\Program Files\Python36"))
            {
                $env:PATH += ";C:\Program Files\Python36"
                Write-Output "Python found at C:\Program Files\Python36"
            }
            # Test default location from orignal Python-Setup 3.4.x
            elseif((Test-Path "C:\Program Files\Python34"))
            {
                $env:PATH += ";C:\Program Files\Python34"
                Write-Output "Python found at C:\Program Files\Python34"
            }
            elseif($Mandatory)
            {
                Write-Output "Mandatory Python not found, try to download portable Version"
                Import-Module "$PSScriptRoot\PortableApps.ps1" -Force
                PortableApp-DownloadAndPath $Global:PortablePython36Download $Global:PortablePython36Name $Global:PortablePython36BinDir
            }
            else
            {
                Write-Output "No Python found"
            }
        }
    }
}
