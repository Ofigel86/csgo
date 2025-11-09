@echo off
setlocal ENABLEDELAYEDEXPANSION

rem ------------------------------------------------------------
rem  Configuration
rem ------------------------------------------------------------

set "ROOT=%~dp0"
set "SOLUTION=%ROOT%supremacy_csgo.sln"

rem allow overriding via arguments: build_release.bat [Configuration] [Platform]
set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=Release"

set "PLATFORM=%~2"
if "%PLATFORM%"=="" set "PLATFORM=Win32"

rem ------------------------------------------------------------
rem  Locate MSBuild
rem ------------------------------------------------------------

call :find_msbuild
if not defined MSBUILD goto :fail

echo [INFO] Using MSBuild: %MSBUILD%
echo [INFO] Solution : %SOLUTION%
echo [INFO] Config   : %CONFIG%
echo [INFO] Platform : %PLATFORM%
echo.

"%MSBUILD%" "%SOLUTION%" ^
    /p:Configuration=%CONFIG% ^
    /p:Platform=%PLATFORM% ^
    /m ^
    /nologo

if errorlevel 1 goto :fail

echo.
echo [SUCCESS] Build finished. Output should be in "%ROOT%%PLATFORM%\%CONFIG%" or "%ROOT%%CONFIG%" depending on project settings.
goto :eof

:find_msbuild
    rem try vswhere first (VS 2017+)
    set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
    if exist "%VSWHERE%" (
        for /f "usebackq delims=" %%p in (`"%VSWHERE%" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe"`) do (
            set "MSBUILD=%%p"
            goto :eof
        )
    )

    rem fallback to well-known MSBuild location (VS2019/2022 Community)
    if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD=%ProgramFiles%\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
        goto :eof
    )

    if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
        goto :eof
    )

    rem fallback to .NET Framework MSBuild (может не иметь нужных тулчей)
    for %%v in (17.0 16.0 15.0 14.0) do (
        if exist "%SystemRoot%\Microsoft.NET\Framework\v%%v\MSBuild.exe" (
            set "MSBUILD=%SystemRoot%\Microsoft.NET\Framework\v%%v\MSBuild.exe"
            goto :eof
        )
    )

    echo [ERROR] Не удалось найти MSBuild. Установи Visual Studio или Build Tools.
    exit /b 1

:fail
echo.
echo [ERROR] Сборка завершилась с ошибкой.
exit /b 1


