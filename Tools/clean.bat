@echo off
cd ..
rd /S /Q Solution
rd /S /Q Solution.VC12
rd /S /Q Solution.VC14
rd /S /Q bin
rd /S /Q lib
rd /S /Q Debug
rd /S /Q Release
rd /S /Q build
rd /S /Q Doku\Doxygen\html
del /q CMakeLists.txt.user
cd Tools