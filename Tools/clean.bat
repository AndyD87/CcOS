@echo off
cd ..
for /f %%G in ('dir /a:d /b Solution*') do rd /S /Q %%G
rd /S /Q Output
rd /S /Q Doku\Doxygen\html
del /q CMakeLists.txt.user
cd Tools