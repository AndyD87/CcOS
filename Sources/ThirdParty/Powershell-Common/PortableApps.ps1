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

Import-Module "$PSScriptRoot\Web.ps1" -Force
Import-Module "$PSScriptRoot\Compress.ps1" -Force

# Centralize all Tools to one directory to avoid multiple downloads
$Global:PortableAppsDir  = "$PSScriptRoot\Tools"
# If an Location was already defined from an other Script or by System, use this location
if(-not [string]::IsNullOrEmpty($env:CC_PORTABLE_APPS_DIR))
{
    $Global:PortableAppsDir = $env:CC_PORTABLE_APPS_DIR
}
# Check if an global option for caching files in specific folder exists
elseif(-not [string]::IsNullOrEmpty($env:CC_CACHE_DIR))
{
    $Global:PortableAppsDir = $env:CC_CACHE_DIR + "/Tools"
    $env:CC_PORTABLE_APPS_DIR = $Global:PortableAppsDir
}
else
{
    $env:CC_PORTABLE_APPS_DIR = $Global:PortableAppsDir
}

<#
.SYNOPSIS
    Download a Zipped Portable Version of an Application and add it's bin Directory to PATH
.DESCRIPTION
    Target Outputfolder will be $Global:PortableAppsDir.
    This can be overwritten by setteing the System Variable PORTABLE_APPS_DIR with a different location.
.PARAMETER Url
    Url to Target Application
.PARAMETER Name
    Name of Application, wich will be used for creating a directory in $Global:PortableAppsDir
.PARAMETER Md5
    Optional: HexString for Zip's MD5-Value of downloaded file
.PARAMETER Crc
    Optional: Uint32 number for Zip's Crc-Value of downloaded file
#>
Function PortableApp-DownloadAndPath
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [string]$Url,
        [Parameter(Mandatory=$True, Position=2)]
        [string]$Name,
        [Parameter(Mandatory=$False, Position=3)]
        [string]$BinDir = "",
        [Parameter(Mandatory=$False, Position=4)]
        [string]$ReturnPath = $false,
        [string]$Md5 = "",
        [Uint32]$Crc  = 0

    )
    Import-Module "$PSScriptRoot\Web.ps1" -Force
    Import-Module "$PSScriptRoot\Compress.ps1" -Force

    # Generate Paths from Globals
    $ZipEnding =  ".zip"
    if($Url.EndsWith(".7z"))
    {
        $ZipEnding =  ".7z"
    }
    $PortableAppCacheFile = $Global:PortableAppsDir+"\"+$Name+$ZipEnding
    $PortableAppTargetDir = $Global:PortableAppsDir+"\"+$Name
    if([string]::IsNullOrWhiteSpace($BinDir) -eq $false)
    {
        $PortableAppBinDir = $PortableAppTargetDir+"\"+$BinDir
    }
    else
    {
        $PortableAppBinDir = $PortableAppTargetDir
    }
    
    if(-not (Test-Path $Global:PortableAppsDir))
    {
        $Trash = New-Item -ItemType Directory -Path $Global:PortableAppsDir
    }

    # Test if BinDir already available
    if(-not (Test-Path $PortableAppBinDir))
    {
        Web-Download $Url $PortableAppCacheFile -Md5 $Md5 -Crc $Crc
        if(-not (Test-Path $PortableAppTargetDir))
        {
            $Trash = New-Item -ItemType Directory -Path $PortableAppTargetDir
        }
        Compress-Unzip $PortableAppCacheFile $PortableAppTargetDir
        Remove-Item $PortableAppCacheFile
        $env:PATH += ";$PortableAppBinDir"
        if($ReturnPath -eq $false)
        {
            Write-Output "$Name now available at $PortableAppBinDir"
        }
    }
    else
    {
        # Nothing to download, but PATH is possible not yet set.
        $env:PATH = "$PortableAppBinDir;" + $env:PATH
    }
    if($ReturnPath -eq $true)
    {
        return $PortableAppBinDir
    }
}

