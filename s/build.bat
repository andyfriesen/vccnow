@echo off

mkdir build
pushd build

cl -GS -WX -W4 -wd4100 -wd4189 -wd4576 -FC -Zi ..\code\vccnow.cpp

popd