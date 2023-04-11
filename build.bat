@echo off

set CC=g++
set CFLAGS=-Wall -Wextra -Werror -ISDL\include -LSDL\lib -lmingw32 -lSDL2main -lSDL2 -O3
set SOURCE=src\*.cpp
set OUT=test.exe

%CC% %SOURCE% -o %OUT% %CFLAGS%