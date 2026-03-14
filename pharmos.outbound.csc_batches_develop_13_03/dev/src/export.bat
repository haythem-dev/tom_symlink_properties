@echo off

set PWD=%CD%

SET PROJECT=csc_batches

set BASEDIR=%PWD%\..\..

set SRCDIR=%BASEDIR%\dev\src
set EXTDIR=%BASEDIR%\dev\ext
set DESTDIR=%BASEDIR%\export\%PROJECT%

echo -----------------------------------------------------
echo  copy %PROJECT% files to export directory 
echo       from %SRCDIR%
echo       from %EXTDIR%
echo       to   %DESTDIR%
echo  compress export files to %DESTDIR%\built.zip
echo -----------------------------------------------------


echo %SRCDIR%\csc\debug\
xcopy %SRCDIR%\csc\debug\csc_batches.exe           %DESTDIR%\debug\              /d /y /s /e /q
xcopy %SRCDIR%\csc\debug\multilang                 %DESTDIR%\debug\multilang\    /d /y /s /e /q
xcopy %SRCDIR%\csc\print\*.ps                      %DESTDIR%\debug\print\        /d /y /s /e /q
echo %SRCDIR%\csc\release\
xcopy %SRCDIR%\csc\release\csc_batches.exe         %DESTDIR%\release\            /d /y /s /e /q
xcopy %SRCDIR%\csc\release\multilang               %DESTDIR%\release\multilang\  /d /y /s /e /q
xcopy %SRCDIR%\csc\print\*.ps                      %DESTDIR%\release\print\      /d /y /s /e /q

cd %DESTDIR%
echo zip %DESTDIR%\%PROJECT%.zip ...
7z a -sdel .\%PROJECT%.zip .\

copy /y %SRCDIR%\build.xml %DESTDIR%
copy /y %SRCDIR%\ivy.xml %DESTDIR%
copy /y %SRCDIR%\deploy_zdev.ps1 %DESTDIR%


cd /D %PWD%

