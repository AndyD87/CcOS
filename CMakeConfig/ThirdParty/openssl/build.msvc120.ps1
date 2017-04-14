if( !(Test-Path .\Strawberry.zip))
{
    Invoke-WebRequest -Uri "http://mirror.adirmeier.de/binaries/strawberry-perl-5.24.1.1-32bit-portable.zip" -OutFile "Strawberry.zip"
}

if( !(Test-Path .\Nasm.zip))
{
    Invoke-WebRequest -Uri "http://mirror.adirmeier.de/binaries/nasm-2.12.02.zip" -OutFile "Nasm.zip"
}

if( !(Test-Path .\StrawberryPerl))
{
    mkdir StrawberryPerl
    cd    StrawberryPerl
    cmake -E tar xf "../Strawberry.zip"
    cd ..
}

if( !(Test-Path .\Nasm))
{
    mkdir Nasm
    cd    Nasm
    cmake -E tar xf "../Nasm.zip"
    cd ..
}

Start-Process "git" -ArgumentList "clone --branch OpenSSL_1_1_0e  https://github.com/openssl/openssl.git" -Wait

New-Item -ItemType Directory "$PSScriptRoot\openssl_x64" -Force
New-Item -ItemType Directory "$PSScriptRoot\openssl_x86" -Force
Copy-Item "$PSScriptRoot\openssl\*" "$PSScriptRoot\openssl_x64" -Recurse -Force
Copy-Item "$PSScriptRoot\openssl\*" "$PSScriptRoot\openssl_x86" -Recurse -Force

###############################################################################
# x64 build
###############################################################################
cd "$PSScriptRoot\openssl_x64"

Set-Content "Build.bat" "SET PATH=%PATH%;$PSScriptRoot\StrawberryPerl\perl\bin\;$PSScriptRoot\Nasm\"
Add-Content "Build.bat" 'call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64'
Add-Content "Build.bat" "mkdir `"$PSScriptRoot\openssl.msvc120.x64.debug.dll`""
Add-Content "Build.bat" "perl.exe Configure debug-VC-WIN64A --prefix=`"$PSScriptRoot\openssl.msvc120.x64.debug.dll`""
Add-Content "Build.bat" "nmake"
Add-Content "Build.bat" "nmake install"
Add-Content "Build.bat" "nmake clean"
Add-Content "Build.bat" "exit"

Start-Process "cmd" -ArgumentList "/C build.bat" -Wait

Remove-Item "Build.bat"


Set-Content "Build.bat" "SET PATH=%PATH%;$PSScriptRoot\StrawberryPerl\perl\bin\;$PSScriptRoot\Nasm\"
Add-Content "Build.bat" 'call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64'
Add-Content "Build.bat" "mkdir `"$PSScriptRoot\openssl.msvc120.x64.debug.static`""
Add-Content "Build.bat" "perl.exe Configure debug-VC-WIN64A no-shared --prefix=`"$PSScriptRoot\openssl.msvc120.x64.debug.static`""
Add-Content "Build.bat" "nmake"
Add-Content "Build.bat" "nmake install"
Add-Content "Build.bat" "nmake clean"
Add-Content "Build.bat" "exit"

Start-Process "cmd" -ArgumentList "/C build.bat" -Wait

Remove-Item "Build.bat"

Set-Content "Build.bat" "SET PATH=%PATH%;$PSScriptRoot\StrawberryPerl\perl\bin\;$PSScriptRoot\Nasm\"
Add-Content "Build.bat" 'call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64'
Add-Content "Build.bat" "mkdir `"$PSScriptRoot\openssl.msvc120.x64.release.dll`""
Add-Content "Build.bat" "perl.exe Configure VC-WIN64A --prefix=`"$PSScriptRoot\openssl.msvc120.x64.release.dll`""
Add-Content "Build.bat" "nmake"
Add-Content "Build.bat" "nmake install"
Add-Content "Build.bat" "nmake clean"
Add-Content "Build.bat" "exit"

Start-Process "cmd" -ArgumentList "/C build.bat" -Wait

Remove-Item "Build.bat"


Set-Content "Build.bat" "SET PATH=%PATH%;$PSScriptRoot\StrawberryPerl\perl\bin\;$PSScriptRoot\Nasm\"
Add-Content "Build.bat" 'call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64'
Add-Content "Build.bat" "mkdir `"$PSScriptRoot\openssl.msvc120.x64.release.static`""
Add-Content "Build.bat" "perl.exe Configure VC-WIN64A no-shared --prefix=`"$PSScriptRoot\openssl.msvc120.x64.release.static`""
Add-Content "Build.bat" "nmake"
Add-Content "Build.bat" "nmake install"
Add-Content "Build.bat" "nmake clean"
Add-Content "Build.bat" "exit"

Start-Process "cmd" -ArgumentList "/C build.bat" -Wait

Remove-Item "Build.bat"

cd ..

###############################################################################
# x86 build
###############################################################################
cd "$PSScriptRoot\openssl_x64"

Set-Content "Build.bat" "SET PATH=%PATH%;$PSScriptRoot\StrawberryPerl\perl\bin\;$PSScriptRoot\Nasm\"
Add-Content "Build.bat" 'call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86'
Add-Content "Build.bat" "mkdir `"$PSScriptRoot\openssl.msvc120.x86.debug.dll`""
Add-Content "Build.bat" "perl.exe Configure debug-VC-WIN32 --prefix=`"$PSScriptRoot\openssl.msvc120.x86.debug.dll`""
Add-Content "Build.bat" "nmake"
Add-Content "Build.bat" "nmake install"
Add-Content "Build.bat" "nmake clean"
Add-Content "Build.bat" "exit"

Start-Process "cmd" -ArgumentList "/C build.bat" -Wait

Remove-Item "Build.bat"


Set-Content "Build.bat" "SET PATH=%PATH%;$PSScriptRoot\StrawberryPerl\perl\bin\;$PSScriptRoot\Nasm\"
Add-Content "Build.bat" 'call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86'
Add-Content "Build.bat" "mkdir `"$PSScriptRoot\openssl.msvc120.x86.debug.static`""
Add-Content "Build.bat" "perl.exe Configure debug-VC-WIN32 no-shared --prefix=`"$PSScriptRoot\openssl.msvc120.x86.debug.static`""
Add-Content "Build.bat" "nmake"
Add-Content "Build.bat" "nmake install"
Add-Content "Build.bat" "nmake clean"
Add-Content "Build.bat" "exit"

Start-Process "cmd" -ArgumentList "/C build.bat" -Wait

Remove-Item "Build.bat"

Set-Content "Build.bat" "SET PATH=%PATH%;$PSScriptRoot\StrawberryPerl\perl\bin\;$PSScriptRoot\Nasm\"
Add-Content "Build.bat" 'call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86'
Add-Content "Build.bat" "mkdir `"$PSScriptRoot\openssl.msvc120.x86.release.dll`""
Add-Content "Build.bat" "perl.exe Configure VC-WIN32 --prefix=`"$PSScriptRoot\openssl.msvc120.x86.release.dll`""
Add-Content "Build.bat" "nmake"
Add-Content "Build.bat" "nmake install"
Add-Content "Build.bat" "nmake clean"
Add-Content "Build.bat" "exit"

Start-Process "cmd" -ArgumentList "/C build.bat" -Wait

Remove-Item "Build.bat"


Set-Content "Build.bat" "SET PATH=%PATH%;$PSScriptRoot\StrawberryPerl\perl\bin\;$PSScriptRoot\Nasm\"
Add-Content "Build.bat" 'call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86'
Add-Content "Build.bat" "mkdir `"$PSScriptRoot\openssl.msvc120.x86.release.static`""
Add-Content "Build.bat" "perl.exe Configure VC-WIN32 no-shared --prefix=`"$PSScriptRoot\openssl.msvc120.x86.release.static`""
Add-Content "Build.bat" "nmake"
Add-Content "Build.bat" "nmake install"
Add-Content "Build.bat" "nmake clean"
Add-Content "Build.bat" "exit"

Start-Process "cmd" -ArgumentList "/C build.bat" -Wait

Remove-Item "Build.bat"

cd ..

###############################################################################
# Packing
###############################################################################

cmake -E tar cf openssl.1.1.0e.msvc120.x64.zip --format=zip openssl.msvc120.x64.debug.dll openssl.msvc120.x64.debug.static openssl.msvc120.x64.release.dll openssl.msvc120.x64.release.static 
cmake -E tar cf openssl.1.1.0e.msvc120.x86.zip --format=zip openssl.msvc120.x86.debug.dll openssl.msvc120.x86.debug.static openssl.msvc120.x86.release.dll openssl.msvc120.x86.release.static 

###############################################################################
# Cleanup and pack
###############################################################################

Remove-Item "openssl"        -Recurse -Force
Remove-Item "openssl_x64"    -Recurse -Force
Remove-Item "openssl_x86"    -Recurse -Force
Remove-Item "Nasm"           -Recurse -Force
Remove-Item "StrawberryPerl" -Recurse -Force
Remove-Item "Strawberry.zip" -Force
Remove-Item "Nasm.zip" -Force
