set PWD=%CD%

set PROJECT=tender

set BASEDIR=%PWD%\..\..
set SRCDIR=%BASEDIR%\dev\src\tender
set EXTDIR=%BASEDIR%\dev\ext
set DESTDIR=%BASEDIR%\export\%PROJECT%

rmdir %DESTDIR% /s /q 2>&1 > NUL

mkdir %DESTDIR%\multilang
copy %SRCDIR%\multilang\*.qm  %DESTDIR%\multilang

mkdir %DESTDIR%\release
copy %SRCDIR%\release\tender.exe      %DESTDIR%\release

mkdir %DESTDIR%\debug
copy %SRCDIR%\debug\tender.exe      %DESTDIR%\debug

cd %DESTDIR%\
7z.exe a -sdel .\%PROJECT%.zip .\

copy %SRCDIR%\..\build.xml .\
copy %SRCDIR%\..\ivy.xml .\

cd /D %PWD%