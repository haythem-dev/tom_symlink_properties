@echo off

set ENV=%MARATHON_CSC_BATCHES%
set PATH_DOCU=..\codedocu
set PATH_DOCU_HTML=%PATH_DOCU_LIB%\html
set PATH_DOXY=.

if not exist %PATH_DOCU%      mkdir %PATH_DOCU%
if not exist %PATH_DOCU_HTML% mkdir %PATH_DOCU_HTML%

echo rmdir /s /q %PATH_DOCU_HTML% ...
rmdir /s /q %PATH_DOCU_HTML%

CALL :createVersionInfo

echo doxygen %PATH_DOXY%\doxyfile ...
type version.tmp | %ENV%\ext\doxygen\doxygen.exe - >%PATH_DOXY%\doxygen_log.txt
del version.tmp
goto :eof

:createVersionInfo
  FOR /F "usebackq tokens=3" %%a in (`FIND /I "#define VERSION_MAJORNO" ..\version.h`) DO SET MAJPOR=%%~a
  FOR /F "usebackq tokens=3" %%a in (`FIND /I "#define VERSION_MINORNO" ..\version.h`) DO SET MINOR=%%~a
  FOR /F "usebackq tokens=3" %%a in (`FIND /I "#define VERSION_EXTENSION" ..\version.h`) DO SET EXTENSION=%%~a
  FOR /F "usebackq tokens=3" %%a in (`FIND /I "#define VERSION_BUILDNO" ..\version.h`) DO SET BUILD=%%~a
  type doxyfile > version.tmp
  echo. >> version.tmp
  echo.PROJECT_NUMBER=%MAJPOR%.%MINOR%.%EXTENSION%.%BUILD% >> version.tmp
  goto :eof