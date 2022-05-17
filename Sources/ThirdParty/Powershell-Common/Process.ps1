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
Function Process-RunInIse 
{
    try {    
        return $psISE -ne $null;
    }
    catch {
        return $false;
    }
}

<#
.SYNOPSIS
    Execute a process and capture it`s output and additionaly store ExitCode to a referenced Argument
.DESCRIPTION
    Target Outputfolder will be $Global:PortableAppsDir.
    This can be overwritten by setteing the System Variable PORTABLE_APPS_DIR with a different location.
.PARAMETER Executable
    Application to Exectute, Full path is not required if it is available in PATH.
.PARAMETER Arguments
    Arguments to pass to Application like in commandline.
.PARAMETER WorkingDir
    Use a different Working Directory than current.
.PARAMETER ExitCode
    Value will be overwritten with ExitCode of Process
.EXAMPLE
    $ExitCode = -1
    Process-StartAndGetOutput bcdedit -Arguments "/enum all" -ExitCode ([ref]$ExitCode)
    
    Write-Output ("BcdExitCode: $ExitCode")
#>
Function Process-StartAndGetOutputAndExit
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [string]$Executable,
        [Parameter(Mandatory=$false, Position=2)]
        [string]$Arguments = "",
        [Parameter(Mandatory=$false, Position=3)]
        [string]$WorkingDir = "",
        [Parameter(Mandatory=$false, Position=4)]
        [ref]$ExitCode = $null
    )
	
    $pinfo = New-Object System.Diagnostics.ProcessStartInfo
    $pinfo.FileName = $Executable
    $pinfo.RedirectStandardOutput = $true
    $pinfo.UseShellExecute = $false
    $pinfo.Arguments = $Arguments
    $pinfo.WindowStyle =  [System.Diagnostics.ProcessWindowStyle]::Hidden
    $pinfo.CreateNoWindow = $true
    $pinfo.WorkingDirectory = $WorkingDir
    $p = New-Object System.Diagnostics.Process
    $p.StartInfo = $pinfo
    $trash = $p.Start()
    $string=""
    while($p.HasExited -eq $false)
    {
	    $string += $p.StandardOutput.ReadLine();
        $string += [Environment]::NewLine
    }
    while($p.StandardOutput.EndOfStream -eq $false)
    {
	    $string += $p.StandardOutput.ReadLine();
        $string += [Environment]::NewLine
    }

    $ExitCode.Value = $p.ExitCode

    return $string
}

<#
.SYNOPSIS
    Execute a process and capture it`s output
.DESCRIPTION
    Target Outputfolder will be $Global:PortableAppsDir.
    This can be overwritten by setteing the System Variable PORTABLE_APPS_DIR with a different location.
.PARAMETER Executable
    Application to Exectute, Full path is not required if it is available in PATH.
.PARAMETER Arguments
    Arguments to pass to Application like in commandline.
.PARAMETER WorkingDir
    Use a different Working Directory than current.
.EXAMPLE
    $Output Process-StartAndGetOutput bcdedit -Arguments "/enum all"
    Write-Output $Output
#>
Function Process-StartAndGetOutput
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [string]$Executable,
        [Parameter(Mandatory=$false, Position=2)]
        [string]$Arguments = "",
        [Parameter(Mandatory=$false, Position=3)]
        [string]$WorkingDir = ""
    )

    $ExitCode = -1

    return Process-StartAndGetOutputAndExit -Executable $Executable -Arguments $Arguments -WorkingDir $WorkingDir -ExitCode ([ref]$ExitCode)
}

Function Process-StartInline
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [string]$Executable,
        [Parameter(Mandatory=$false, Position=2)]
        [string]$Arguments = "",
        [Parameter(Mandatory=$false, Position=3)]
        [string]$WorkingDir = "",
        [Parameter(Mandatory=$false, Position=4)]
        [bool]$Hidden = $false
    )
    $pinfo = New-Object System.Diagnostics.ProcessStartInfo
    if([string]::IsNullOrEmpty($WorkingDir))
    {
        $WorkingDir = ((Get-Item -Path ".\" -Verbose).FullName)
    }
    $pinfo.FileName = $Executable
    $pinfo.UseShellExecute = $false
    $pinfo.Arguments = $Arguments
    $pinfo.WorkingDirectory = $WorkingDir
    if($Hidden)
    {
        $pinfo.WindowStyle =  [System.Diagnostics.ProcessWindowStyle]::Hidden
        if(Process-RunInIse)
        {
            $pinfo.CreateNoWindow = $true;
        }
    }
    $p = New-Object System.Diagnostics.Process
    if(Process-RunInIse)
    {
        $pinfo.RedirectStandardOutput = $true
        $p.StartInfo = $pinfo
        $trash = $p.Start()
        while($p.HasExited -eq $false)
        {
	        Write-Host $p.StandardOutput.ReadLine();
        }
        while($p.StandardOutput.EndOfStream -eq $false)
        {
	        Write-Host $p.StandardOutput.ReadLine();
        }
    }
    else
    {
        $p.StartInfo = $pinfo
        $trash = $p.Start()
        $p.WaitForExit();
    }

    return $p.ExitCode
}

Function Process-StartInlineAndThrow
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [string]$Executable,
        [Parameter(Mandatory=$false, Position=2)]
        [string]$Arguments = "",
        [Parameter(Mandatory=$false, Position=3)]
        [string]$WorkingDir = "",
        [Parameter(Mandatory=$false, Position=4)]
        [bool]$Hidden = $false
    ) 
    if((Process-StartInline $Executable $Arguments $WorkingDir $Hidden) -ne 0)
    {
        throw "Failed: $Executable $Arguments"
    }
}

Function Process-StopIfExists
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [string]$Name
    )

    $Processes = Get-Process $Name -ErrorAction SilentlyContinue

    foreach ($Process in $Processes)
    {
        Stop-Process $Process -Force
    }
}

