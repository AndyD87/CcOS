@echo off
cd ..
for /f %%G in ('dir /a:d /b Solution*') do rd /S /Q %%G
rd /S /Q bin
rd /S /Q lib
rd /S /Q Debug
rd /S /Q Release
rd /S /Q build
rd /S /Q Doku\Doxygen\html
del /q CMakeLists.txt.user
cd Tools