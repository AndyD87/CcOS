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

$Global:Portable7zipDownload  = "http://coolcow.de/projects/ThirdParty/7zip/binaries/16.4.0.0/7zip.zip"
$Global:Portable7zipName      = "7zip"
$Global:Portable7zipBinDir    = "\"

Function 7Zip-GetEnv
{
    PARAM(
        [Parameter(Mandatory=$False, Position=1)]
        [switch]$Mandatory
    )

    if(Get-Command 7za.exe -ErrorAction SilentlyContinue)
    {
    }
    elseif(Get-Command 7z.exe -ErrorAction SilentlyContinue)
    {
    }
    elseif((Test-Path "C:\Program Files\7-Zip"))
    {
        $env:PATH += ";C:\Program Files\7-Zip"
        Write-Output "7zip found at C:\Program Files\7-Zip"
    }
    elseif($Mandatory)
    {
        Write-Output "Mandatory 7zip not found, try to download portable Version"
        Import-Module "$PSScriptRoot\PortableApps.ps1" -Force
        PortableApp-DownloadAndPath $Global:Portable7zipDownload $Global:Portable7zipName $Global:Portable7zipBinDir
    }
    else
    {
        Write-Output "No 7zip found"
    }
}

Function 7Zip-GetExeName
{
    PARAM(
        [Parameter(Mandatory=$False, Position=1)]
        [string]$AdditionalPath
    )

    $Trash = 7Zip-GetEnv
    $SavePath   = $env:PATH

    if(-not [string]::IsNullOrEmpty($AdditionalPath))
    {
        $env:PATH += ";$AdditionalPath"
    }

    if(Get-Command 7z.exe -ErrorAction SilentlyContinue)
    {
        return "7z.exe"
    }
    elseif(Get-Command 7za.exe -ErrorAction SilentlyContinue)
    {
        return "7za.exe"
    }
    else
    {
        throw "7Zip exe not found"
    }
    
    if(-not [string]::IsNullOrEmpty($AdditionalPath))
    {
        $env:PATH += ";$SavePath"
    }
}

Function 7Zip-Compress
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [string]$OutputFile,
        [Parameter(Mandatory=$False, Position=2)]
        [string]$Single = "",
        [Parameter(Mandatory=$False, Position=3)]
        [int]$Compression = 5

    )

    $Exe = 7Zip-GetExeName
    
    if(-not [string]::IsNullOrEmpty($Single))
    {
        $oFile = Get-Item $Single
        if($oFile.PSIsContainer)
        {
            #$CurrentDir = ((Get-Item -Path ".\" -Verbose).FullName)
            #cd $Single
            Process-StartInlineAndThrow $Exe "u -y -mx=$Compression `"$OutputFile`" `"$Single\*`"" -Hidden $true
            #cd $CurrentDir
        }
        else
        {
            Process-StartInlineAndThrow $Exe "u -y -mx=$Compression `"$OutputFile`" `"$Single`"" -Hidden $true
        }
    }
    else
    {
        throw "Empty Path"
    }
}

Function 7Zip-Uncompress
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [string]$ZipFile,
        [Parameter(Mandatory=$True, Position=2)]
        [string]$Target
    )

    $Exe = 7Zip-GetExeName
    
    Process-StartInlineAndThrow $Exe "x -y $ZipFile -o`"$Target`""
}
