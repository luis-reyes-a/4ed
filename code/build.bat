
WHERE cl >nul 2>nul
IF %ERRORLEVEL% NEQ 0 (
  call vc.bat
)

rem bin\build_optimized.bat
bin\build.bat 