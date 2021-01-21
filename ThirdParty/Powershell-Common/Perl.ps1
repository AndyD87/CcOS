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

$Global:PortablePerlName      = "StrawberryPerl"
$Global:PortablePerlDownload  = "http://coolcow.de/projects/ThirdParty/StrawberryPerl/binaries/5.24.1.1/StrawberryPerl.32bit.portable.zip"
$Global:PortablePerlBin       = "perl\bin"

Function Perl-GetEnv
{
    PARAM(
        [Parameter(Mandatory=$False, Position=1)]
        [switch]$Mandatory
    )

    if(Get-Command perl.exe -ErrorAction SilentlyContinue)
    {
        Write-Output "Perl already in PATH"
    }
    elseif((Test-Path "C:\Tools\Perl\bin"))
    {
        $env:PATH += ";C:\Tools\Perl\bin"
        Write-Output "Perl found at C:\Tools\Perl\bin"
    }
    elseif((Test-Path "C:\Tools\Perl\perl\bin"))
    {
        $env:PATH += ";C:\Tools\Perl\perl\bin"
        Write-Output "Perl found at C:\Tools\Perl\perl\bin"
    }
    elseif($Mandatory)
    {
        Write-Output "Mandatory Perl not found, try to download portable Version"
        Import-Module "$PSScriptRoot\PortableApps.ps1" -Force
        PortableApp-DownloadAndPath $Global:PortablePerlDownload $Global:PortablePerlName $Global:PortablePerlBin
    }
    else
    {
        Write-Output "No Perl found";
    }
}
