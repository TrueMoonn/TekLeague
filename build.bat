@echo off
setlocal enabledelayedexpansion

set SCRIPT_DIR=%~dp0
cd /d "%SCRIPT_DIR%"

:display_help
if "%1"=="" (
    echo Usage: build.bat [FLAG]
    echo.
    echo Flags:
    echo   --build, -b           Build the program with CMake
    echo   --re-build, -rb       Clean build (delete build/ and artifacts^)
    echo   --debug-build, -d     Build in Debug mode with verbose output
    echo   --clear, -c           Clear build artifacts
    echo   --help, -h            Show this help message
    goto :end
)

if "%1"=="--help" goto :display_help
if "%1"=="-h" goto :display_help

:clear_project
if "%1"=="--clear" goto :do_clear
if "%1"=="-c" goto :do_clear
if "%1"=="--re-build" goto :do_clear
if "%1"=="-rb" goto :do_clear
goto :check_build

:do_clear
echo Clearing project artifacts...
if exist build\ rmdir /s /q build
if exist Release\ del /q Release\*.exe Release\*.dll 2>nul
if exist TrueEngine\ (
    for /r TrueEngine %%F in (*.a *.lib *.dll) do del /q "%%F" 2>nul
)
if "%1"=="--clear" goto :end
if "%1"=="-c" goto :end

:check_build
if "%1"=="--build" goto :do_build
if "%1"=="-b" goto :do_build
if "%1"=="--re-build" goto :do_build
if "%1"=="-rb" goto :do_build
if "%1"=="--debug-build" goto :do_build
if "%1"=="-d" goto :do_build
goto :end

:do_build
if "%1"=="--debug-build" goto :debug_build
if "%1"=="-d" goto :debug_build

echo.
echo ============ BUILD ============
if not exist build\ (
    mkdir build
    cd build
    cmake ..
    cd ..
) else (
    echo Build directory already exists, skipping cmake configuration
)
cmake --build build --config Release --parallel
goto :copy_deps

:debug_build
echo.
echo ============ DEBUG BUILD ============
if exist build\ rmdir /s /q build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug --parallel --verbose
cd ..
goto :copy_deps

:copy_deps
echo.
echo ============ COPYING DEPENDENCIES ============

REM Copy SFML DLLs from build output to Release
if exist "build\_deps\sfml-build\bin\Release" (
    echo Copying SFML DLLs...
    copy /Y "build\_deps\sfml-build\bin\Release\*.dll" Release\ >nul 2>&1
)

REM Copy all DLLs to client/plugins for plugin dependency resolution
if exist "Release\*.dll" (
    echo Copying DLLs to client/plugins...
    if not exist "client\plugins" mkdir "client\plugins"
    copy /Y "Release\*.dll" "client\plugins\" >nul 2>&1
)

REM Copy plugin DLLs from build to Release if they don't exist there
if exist "build\Release" (
    for %%F in (build\Release\*.dll) do (
        if not exist "Release\%%~nxF" (
            copy /Y "%%F" Release\ >nul 2>&1
        )
    )
)

echo.
echo ============ BUILD COMPLETE ============
echo Binaries: Release\tek_league_client.exe, Release\tek_league_server.exe
goto :end

:end
endlocal
