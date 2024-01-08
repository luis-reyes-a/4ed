
WHERE cl >nul 2>nul
IF %ERRORLEVEL% NEQ 0 (
  call vc.bat
)

bin\build_optimized.bat
rem bin\build.bat 
