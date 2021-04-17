# Notes for WinSock programming WinSock by Visual Studio Code

## Difference between Visual Studio Code and Visual Studio 2015

-   remember add `#include <stdafx.h>`

## Environment variables

In `_mingw.h`, `_WIN32_WINNT` has been changed to `0x0A00` to use `inet_pton()`, etc.

## How to create a executable file contains WinSock

Change directory to parent folder.

Because `#pragma comment(lib, "Ws2_32.lib")` will be **ignored** by GCC,
use command `g++ -g {app}.cpp -o {app} -l ws2_32` to create executable file.

## Problems and solutions

-   undefined reference to `\_imp\_\_gets_s'

Replace `gets_s(buff, BUFF_SIZE)` with `fgets(buff, BUFF_SIZE, stdin)` and `buff[strlen(buff) - 1] = '\0'`.
