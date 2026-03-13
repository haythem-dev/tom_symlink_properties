set PWD=%CD%

set PROJECT=libtender

set BASEDIR=%PWD%\..\..
set SRCDIR=%BASEDIR%\dev\src\libtender
set EXTDIR=%BASEDIR%\dev\ext
set DESTDIR=%BASEDIR%\export\%PROJECT%

rmdir %DESTDIR% /s /q 2>&1 > NUL

mkdir %DESTDIR%\lib
mkdir %DESTDIR%\lib\release
copy %SRCDIR%\release\libtender.lib      %DESTDIR%\lib\release

mkdir %DESTDIR%\lib\debug
copy %SRCDIR%\debug\libtender.lib      %DESTDIR%\lib\debug
copy %SRCDIR%\debug\libtender.pdb      %DESTDIR%\lib\debug

mkdir %DESTDIR%\include
robocopy %SRCDIR%\ %DESTDIR%\include *.h /s /NFL /NDL /NJH /NJS /nc /ns /np
robocopy %SRCDIR%\ %DESTDIR%\include *.hpp /s /NFL /NDL /NJH /NJS /nc /ns /np
robocopy %SRCDIR%\ %DESTDIR%\include *.inl /s /NFL /NDL /NJH /NJS /nc /ns /np

cd %DESTDIR%\
7z.exe a -sdel .\%PROJECT%.zip .\

move .\%PROJECT%.zip ..\tender\

cd ..

rmdir %PROJECT% /s /q 2>&1 > NUL

cd /D %PWD%