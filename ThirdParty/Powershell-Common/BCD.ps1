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

$Global:_BcdDefaultStore = ""

<#
.SYNOPSIS
    Execute a bcdedit command on commandline and get it`s output.
.DESCRIPTION
    Throw Exception if Exitcode is != 0
.PARAMETER Arguments
    Arguments to pass to bcd, 
.PARAMETER Store
    Use a different Store if necessary, default: $Global:_BcdDefaultStore
.PARAMETER WorkingDir
    Use a different Working Directorz than current.
.EXAMPLE
    [string]$Output = Bcd-Exec "/enum all"
#>
Function Bcd-Exec()
{
    PARAM(
        $Arguments,
        [string]$Store = $Global:_BcdDefaultStore
    )

    $BcdArg = ""
    if([string]::IsNullOrEmpty($Store) -eq $false)
    {
        $BcdArg = "/store `"" + $Store + "`" "
    }
    
    $BcdArg = $BcdArg + $Arguments
    $ExitCode = -1
    return Process-StartAndGetOutputAndExit bcdedit -Arguments $BcdArg -ExitCode ([ref]$ExitCode)
    
    if($ExitCode -ne 0)
    {
        throw "BCD Execution failed"
    }
}

<#
.SYNOPSIS
    Set path to BCD store to working with by default
.DESCRIPTION
    Change value of $Global:_BcdDefaultStore with a commandlet
.PARAMETER Store
    Path to BCD Store as string
.EXAMPLE
    Bcd-SetGlobalStore "C:\boot\BCD"
#>
Function Bcd-SetGlobalStore
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [string]$Store
    )

    $Global:_BcdDefaultStore = $Store
}

<#
.SYNOPSIS
    Setup Kernel Debugging in BCD
.DESCRIPTION
    Return value is the installed network key
.PARAMETER HostIp
    Ip Address of Host Computer with running Debugger.
.PARAMETER HostPor
    Port on wich Host Computer will listen for Debug informations
.PARAMETER NicBus
    Address on System the network device is listed.
    Can be found in Device Manger.
    It`s format is: b.d.f
.EXAMPLE
    $Global:_BcdDefaultStore = "C:\boot\BCD"
    $NetworkKey = Bcd-SetupKernelDebugNetwork "192.168.0.2" "59001" "0.25.0"
#>
Function Bcd-SetupKernelDebugNetwork
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [string]$HostIp,
        [Parameter(Mandatory=$True, Position=2)]
        [string]$HostPort,
        [Parameter(Mandatory=$False, Position=3)]
        [string]$NicBus = ""
    )
    
    [string]$FakeOutput = Bcd-Exec "/debug on"
    [string]$Output = Bcd-Exec "/dbgsettings net hostip:$HostIp port:$HostPort"
    if([string]::IsNullOrEmpty($NicBus) -eq $false)
    {
        $FakeOutput = Bcd-Exec "/set `"{dbgsettings}`" busparams $NicBus"
    }

    if($Output.StartsWith("Key="))
    {
        $Output = $Output.Substring(4)
    }

    return $Output
}

<#
.SYNOPSIS
    Enable Testsigned Driver on target windows
.DESCRIPTION
    Return value is the installed network key
.PARAMETER Enable
    if $true, turn Testsigning on.
    if $false, turn Testsigning off.
.PARAMETER Target
    Target entry in BCD, default not set
.EXAMPLE
    Bcd-SetTestsigning $true "{current}"
    Bcd-SetTestsigning -Enable $false
#>
Function Bcd-SetTestsigning
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [bool]$Enable,
        [Parameter(Mandatory=$False, Position=2)]
        [string]$Target = ""
    )
    
    $sSet = "/set"
    if([string]::IsNullOrEmpty($Target) -eq $False)
    {
        $sSet += " $Target"
    }
    if($Enable)
    {
        [string]$sSet += " TESTSIGNING ON"
    }
    else
    {
        [string]$sSet += " TESTSIGNING OFF"
    }
    
    [string]$Output = Bcd-Exec $sSet
}