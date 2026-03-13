cmd /c export-tender.bat %*
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

cmd /c export-libtender.bat %*
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%