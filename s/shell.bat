@echo off

set path=C:\remedy;C:\dev\vccnow\s;C:\dev\vccnow\build

rem vcvarsall.bat won't configure things properly (?) without this.
rem If system32 is not included, stdio.h cannot be found when running build.bat!
rem C:\Windows doesn't seem necessary for building, but might as well.
set path=%path%;C:\WINDOWS\system32;C:\WINDOWS;C:\Users\Charles\AppData\Local\Programs\Microsoft VS Code\bin;C:\Program Files\LLVM\bin

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
start remedybg open-session c:\dev\vccnow\.rdbg\vccnow.rdbg
code .
