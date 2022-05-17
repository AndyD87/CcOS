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

$Global:WinFlexBisonName      = "WinFlexBison"
$Global:WinFlexBisonDownload  = "http://coolcow.de/projects/ThirdParty/binaries/2.5.5.0/win_flex_bison.zip"

Function WinFlexBison-GetEnv
{
    PARAM(
        [Parameter(Mandatory=$False, Position=1)]
        [switch]$Mandatory
    )

    if(Get-Command win_flex.exe -ErrorAction SilentlyContinue)
    {
        Write-Output "WinFlexBison already in PATH"
    }
    elseif((Test-Path "C:\Tools\WinFlexBison"))
    {
        $env:PATH += ";C:\Tools\WinFlexBison\bin"
        Write-Output "WinFlexBison found at C:\Tools\WinFlexBison"
    }
    elseif($Mandatory)
    {
        Write-Output "Mandatory WinFlexBison not found, try to download portable Version"
        Import-Module "$PSScriptRoot\PortableApps.ps1" -Force
        $Path = PortableApp-DownloadAndPath $Global:WinFlexBisonDownload $Global:WinFlexBisonName -ReturnPath $true
        if([string]::IsNullOrEmpty($Path) -eq $false)
        {
            $sFlex = "$Path\win_flex.exe"
            if((Test-Path $sFlex) -eq $true)
            {
                if((Test-Path "$Path\flex.exe") -eq $false)
                {
                    Copy-Item "$Path\win_flex.exe" "$Path\flex.exe"
                }
            }
            if((Test-Path "$Path\win_bison.exe") -eq $true)
            {
                if((Test-Path "$Path\bison.exe") -eq $false)
                {
                    Copy-Item "$Path\win_bison.exe" "$Path\bison.exe"
                }
            }
        }
    }
    else
    {
        Write-Output "No WinFlexBison found";
    }
}
