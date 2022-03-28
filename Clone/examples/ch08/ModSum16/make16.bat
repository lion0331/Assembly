REM  make16.bat -  batch file for assembling/linking 
REM  Special version for the ModSum program
REM  Requires 16-bit Linker, downloaded from Microsoft FTP site.

REM  Revised: 3/29/04

@echo off
cls

REM The following three lines can be customized for your system:
REM ********************************************BEGIN customize
PATH c:\Masm615
SET INCLUDE=c:\Masm615\include
SET LIB=c:\Masm615\lib
REM ********************************************END customize

ML -Zi -c -Fl Sum_main.asm _display.asm _arrysum.asm _prompt.asm
if errorlevel 1 goto terminate

REM add the /MAP option for a map file in the link command.

LINK /nologo /CODEVIEW Sum_main.obj+_display.obj+_arrysum.obj+_prompt.obj,,NUL,Irvine16;
if errorLevel 1 goto terminate

dir *.exe

:terminate
pause


