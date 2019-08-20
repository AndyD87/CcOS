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

<#
.SYNOPSIS
    Download a File
.DESCRIPTION
    This Method will first try to download the file directly.
    If this fail, Proxysettings will be retreived from System and
    the download will be restarted with retreived proxy Settings
.PARAMETER Url
    Url to requested File
.PARAMETER Target
    Full path to location were file should be stored
.PARAMETER Md5
    Optional: HexString for target's MD5-Value of downloaded file
.PARAMETER Crc
    Optional: Uint32 number for target's Crc-Value of downloaded file
.EXAMPLE
    Web-Download "https://url.de/file.z" "C:\Temp\file.zip"
#>
Function Web-Download
{
    PARAM(
        [Parameter(Mandatory=$True, Position=1)]
        [string]$Url,
        [Parameter(Mandatory=$False, Position=2)]
        [string]$Target,
        [string]$Md5 = "",
        [Uint32]$Crc  = 0
    )
    try
    {
        Invoke-WebRequest -Uri $Url -OutFile $Target
    }
    catch
    {
        # Try with Proxy and Default Credentials
        $sProxy = Web-GetSystemProxy
        if(-not [string]::IsNullOrEmpty($sProxy))
        {
            Invoke-WebRequest -Uri $Url -OutFile $Target -Proxy $sProxy -ProxyUseDefaultCredentials
        }
        else
        {
            throw "Unable to download file, check internet connection"
        }
    }
    if(-not [string]::IsNullOrEmpty($Md5))
    {
        $oHash = Get-FileHash $Target -Algorithm MD5
        if($oHash.Hash -ne $Md5)
        {
            throw "File was downloaded, but does not math required MD5 Hash"
        }
    }
    if($Crc -ne 0)
    {
        $oHash = Web-FileGetCrc $Target
        if($oHash -ne $Crc)
        {
            throw "File was downloaded, but does not math required CRC does not match"
        }
    }
}

<#
.SYNOPSIS
    Download each file from Array until one file succeeded.
.DESCRIPTION
    This Method will first try to download the file directly.
    If this fail, Proxysettings will be retreived from System and
    the download will be restarted with retreived proxy Settings
.PARAMETER Url
    Array of Urls to request one File
.PARAMETER Target
    Full path to location were file should be stored
.PARAMETER Md5
    Optional: HexString for target's MD5-Value of downloaded file
.PARAMETER Crc
    Optional: Uint32 number for target's Crc-Value of downloaded file
.EXAMPLE
    Web-Download @("https://url.de/file.z","https://url.de/file2.z") "C:\Temp\file.zip"
#>
Function Web-DownloadMirrors
{
    PARAM(
        [Parameter(Mandatory=$False, Position=1)]
        [string[]]$Urls,
        [Parameter(Mandatory=$False, Position=2)]
        [string]$Target,
        [string]$Md5 = "",
        [Uint32]$Crc  = ""
    )
    
    $bSuccess;
    foreach($Url in $Urls)
    {
        try
        {
            Web-Download $Url $Target $Md5 $Crc
            return
        }
        catch
        {
            Write-Output $_.Exception.Message
        }
    }
    throw "Could not receive any file from mirrors, possible check internet connection"
}

<#
.SYNOPSIS
    Get Crc Value of a File
.PARAMETER File
    Path to File
#>
function Web-FileGetCrc 
{
  param(
    [Parameter(Mandatory = $True)]
    [String]$File
    )
    # Algorithm from https://github.com/FuzzySecurity/PowerShell-Suite/blob/master/Get-CRC32.ps1
  Add-Type -TypeDefinition @"
    using System;
    using System.Diagnostics;
    using System.Runtime.InteropServices;
    using System.Security.Principal;
  
    public static class CRC32
    {
      [DllImport("ntdll.dll")]
      public static extern UInt32 RtlComputeCrc32(
        UInt32 InitialCrc,
        Byte[] Buffer,
        Int32 Length);
    }
"@
  
  [Byte[]]$Buffer = New-Object Byte[](1024)
  [Uint32]$uiCrcValue = 0
    [System.IO.FileStream]$oFile = New-Object System.IO.FileStream($File, [System.IO.FileMode]::Open)
    while($oFile.Length -ne $oFile.Position)
    {
        [int]$iReadBytes = $oFile.Read($Buffer, 0, $Buffer.Length)
        $uiCrcValue = [CRC32]::RtlComputeCrc32($InitialCRC, $Buffer, $iReadBytes)
    }
    return $uiCrcValue;
}

<#
.SYNOPSIS
    Get Proxy Settings from Windows
#>
Function Web-GetSystemProxy
{
  Add-Type -TypeDefinition @"
        using System;
        using System.Diagnostics;
        using System.Runtime.InteropServices;
        using System.Security.Principal;
        using System.Net;
        using System.Collections.Generic;
        using System.Linq;
        using System.Text;
        using System.Threading.Tasks;

        public static class WinhttpExport
        {
          [StructLayout( LayoutKind.Sequential, CharSet=CharSet.Auto)]  
          public class WINHTTP_PROXY_INFO 
          {
              public UInt32  dwAccessType;
              public String lpszProxy;
              public String lpszProxyBypass;
          }

          [DllImport("Winhttp.dll")]
          private static extern bool WinHttpGetDefaultProxyConfiguration([In, Out]WINHTTP_PROXY_INFO pWinHttpProxyInfo );

          public static String GetSystemProxyInfo()
          {
            WINHTTP_PROXY_INFO oProxyInfo = new WINHTTP_PROXY_INFO();
            if(WinHttpGetDefaultProxyConfiguration(oProxyInfo))
            {
              if (oProxyInfo.lpszProxy == null)
              {
                IWebProxy iWebProxy = WebRequest.GetSystemWebProxy();
                Uri iProxyUri = iWebProxy.GetProxy(new Uri("http://google.de"));
                return iProxyUri.AbsoluteUri;
              }
              else
              {
                return oProxyInfo.lpszProxy;
              }
            }
            else
            {
              return "";
            }
          }
        }
"@
    return [WinhttpExport]::GetSystemProxyInfo()
}
