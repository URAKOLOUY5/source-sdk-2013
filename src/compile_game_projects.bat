:restart

@echo off
cls

set game=
set features=
set vsversion=

echo =========================
echo Games
echo =========================
echo.

rem set /p DBR=Use DBR? (y): 
rem If not "%DBR%"=="n" (
    rem set game=%game% /dbr
rem )

set /p DBR=Use Broken Hope? (y): 
If not "%DBR%"=="n" (
    set game=%game% /brokenhope
)

cls

rem echo.
rem echo =========================
rem echo Optional Features
rem echo =========================
rem echo.

rem set /p Deffered=Use CSM-lighting? (y): 
rem If not "%Deffered%"=="n" (
    rem set features=%features% /define:ENABLE_CSM
rem )

cls

echo =========================
echo Visual Studio Version
echo =========================
echo.

set vsversion=%vsversion% /2013 /define:2019

cls

title Generating %game% %features% %vsversion% Solution File

devtools\bin\vpc.exe +game %game% %features% /mksln sdk_game_%game%.sln %vsversion% /f

if errorlevel 1 (
    pause
	goto restart
)

title Starting compilation  of sdk_game_%game%.sln

set /p VS2019Path=Set path of VS2019:



pause