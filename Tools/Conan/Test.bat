SET PATH=C:\Tools\WinPython.64bit;C:\Tools\WinPython.64bit\Scripts;%PATH%
SET CONAN_USER_HOME=%~dp0\conan
SET CONAN_USER_HOME_SHORT=None

mkdir conan

conan install .