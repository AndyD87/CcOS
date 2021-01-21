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
$Global:PortableNasmDownload  = "http://coolcow.de/projects/ThirdParty/NASM/binaries/2.12.02/NASM.portable.zip"
$Global:PortableNasmName      = "NASM"
$Global:PortableNasmBinDir    = "\"

Function Nasm-GetEnv
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [switch]$Mandatory
    )

    if(Get-Command nasm.exe -ErrorAction SilentlyContinue)
    {
        Write-Output "NASM already in PATH"
    }
    elseif((Test-Path "C:\Program Files\NASM"))
    {
        $env:PATH += ";C:\Program Files\NASM"
        Write-Output "NASM found at C:\Program Files\NASM"
    }
    elseif($Mandatory)
    {
        Write-Output "Mandatory Nasm not found, try to download portable Version"
        Import-Module "$PSScriptRoot\PortableApps.ps1" -Force
        PortableApp-DownloadAndPath $Global:PortableNasmDownload $Global:PortableNasmName $Global:PortableNasmBinDir
    }
    else
    {
        Write-Output "No NASM found"
    }
}

