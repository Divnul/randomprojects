pushd %~dp0\..\Output
cl ..\main.c user32.lib gdi32.lib msimg32.lib
@echo off
del *.obj
popd
