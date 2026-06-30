@echo off
setlocal
cd /d "%~dp0"
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0SkyFire-Build.ps1" %*
set "EXITCODE=%ERRORLEVEL%"
if %EXITCODE% neq 0 (
    echo.
    echo skyfire-updater failed. See skyfire-build.log in this folder.
    if "%~1"=="" pause
)
endlocal & exit /b %EXITCODE%
