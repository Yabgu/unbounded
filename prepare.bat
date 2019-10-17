@echo off
:: Maybe I should have used powershell but anyway this looks familiar for now

call :SANITY_CHECKS || (
  >&2 echo ERROR: Sanity checks are failed
  exit /B 1
)

vcpkg install libxml2 gtest --triplet x64-windows || (
  >&2 echo ERROR: Cannot install dependencies
  exit /B 1
)

if not exist "%~dp0build" mkdir "%~dp0build"
pushd %~dp0build
cmake .. -DCMAKE_GENERATOR_PLATFORM=x64 "-DCMAKE_TOOLCHAIN_FILE=%~dp0third-party\vcpkg\scripts\buildsystems\vcpkg.cmake"
popd
if %ERRORLEVEL% neq 0 (
  >&2 echo ERROR: CMake failed with exit code %ERRORLEVEL%
  exit /B %ERRORLEVEL%
)

echo INFO: Build finished
exit /B 0


:SANITY_CHECKS
:: Sanity checks are here

where git >nul 2>nul
if %ERRORLEVEL% neq 0 (
  >&2 echo ERROR: Git command line client not found, please install it and add it into the path
  exit /B 1
)

where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
  >&2 echo ERROR: CMake not found, please install it and add it into the path
  exit /B 1
)

where vcpkg >nul 2>nul || call :GET_VCPKG || (
  >&2 echo ERROR: VCPkg needed for this build script. If you use MSys2 then you already know what to do
  exit /B 1
)

exit /B 0


:GET_VCPKG
:: Get vcpkg working
call :CLONE_REPOSITORY vcpkg https://github.com/Microsoft/vcpkg.git || exit /B 1

pushd %~dp0third-party\vcpkg
if not exist "%CD%\vcpkg.exe" cmd /C call bootstrap-vcpkg.bat
popd
if %ERRORLEVEL% neq 0 (
  >&2 echo ERROR: Bootstrap vcpkg failed with %ERRORLEVEL%
  exit /B 1 
)

set PATH=%PATH%;%~dp0third-party\vcpkg

exit /B 0

:CLONE_REPOSITORY
:: Clone repositories into third-party folder
if not exist "%~dp0third-party\%1" git clone "%2" --recursive "%~dp0third-party\%1" || (
  rd /S /Q "%~dp0third-party\%1"
  exit /B 1
) else if not exist "%~dp0third-party\%1\.git" (
  >&2 echo ERROR: Git folder does not exist for %1 but its folder exist "%~dp0third-party\%1"
  exit /B 1
)

exit /B 0
