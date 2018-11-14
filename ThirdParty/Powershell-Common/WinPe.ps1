##
# This file is part of Powershell-Common, a collection of powershell-scrips
# 
# Copyright (c) 2018 Andreas Dirmeier
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
Import-Module "$PSScriptRoot\Process.ps1"

function New-TemporaryDirectory 
{
    $parent = [System.IO.Path]::GetTempPath()
    [string] $name = [System.Guid]::NewGuid()
    New-Item -ItemType Directory -Path (Join-Path $parent $name)
}

<#
.SYNOPSIS
    Get the environment vor Windows AIK loaded form Set env batch.
.DESCRIPTION
    Only Windows 10 AIK is supportet at the moment.
#>
Function WinPe-GetEnv
{
    function Invoke-CmdScript {
      param(
        [String] $scriptName
      )
      $cmdLine = """$scriptName"" $args & set"
      & cmd.exe /c $cmdLine |
      select-string '^([^=]*)=(.*)$' | foreach-object {
        $varName = $_.Matches[0].Groups[1].Value
        $varValue = $_.Matches[0].Groups[2].Value
        set-item Env:$varName $varValue
      }
    }
    if($Global:WinPe_GetEnvDone -ne $true)
    {
        $WinAdk = "C:\Program Files (x86)\Windows Kits\10\Assessment and Deployment Kit\Deployment Tools\DandISetEnv.bat"
        if((Test-Path $WinAdk) -ne $true)
        {
            # 10 ADK Path not found, select 8.1 ADK Path
            $WinAdk = "C:\Program Files (x86)\Windows Kits\8.1\Assessment and Deployment Kit\Deployment Tools\DandISetEnv.bat"
        }
        if((Test-Path $WinAdk) -eq $true)
        {
            Invoke-CmdScript $WinAdk
            $Global:WinPe_GetEnvDone = $true;
        }
        else
        {
            throw "Windows ADK not found";
        }
    }
}


Function WinPe-CreateIso
{
    PARAM(
        [string]$AdditionalFiles = "",
        [string]$Architecture = "amd64",
        [string]$TargetIso = "WinPE.iso"
    )
    $sTempDir = New-TemporaryDirectory
    Remove-Item -Recurse -Force $sTempDir -ErrorAction Stop
    
    try
    {
        Write-Host $sTempDir

        WinPe-GetEnv

        copype $Architecture $sTempDir

        if([string]::IsNullOrEmpty($AdditionalFiles) -eq $false)
        {
            Copy-Item "$AdditionalFiles\*" "$sTempDir\media\" -Recurse -Force -ErrorAction Stop
        }

        if((Test-Path $TargetIso) -eq $true)
        {
            Remove-Item -Force $TargetIso -ErrorAction Stop
        }

        MakeWinPEMedia /ISO $sTempDir $TargetIso
    }
    catch
    {
        Remove-Item -Recurse -Force $sTempDir -ErrorAction Stop
        throw $_
    }
    Remove-Item -Recurse -Force $sTempDir -ErrorAction Stop
}
