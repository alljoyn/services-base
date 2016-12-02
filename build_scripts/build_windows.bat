rem  rem  
rem     Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
rem     Source Project Contributors and others.
rem     
rem     All rights reserved. This program and the accompanying materials are
rem     made available under the terms of the Apache License, Version 2.0
rem     which accompanies this distribution, and is available at
rem     http://www.apache.org/licenses/LICENSE-2.0

rem
set GTEST_DIR=
set SCRIPT_FOLDER=%~dp0

rem Defaults
rem -----------------
set ALLJOYN_SRC_DIST_DIR=%ALLJOYN_CORE_DIR%\build\%OS%\%CPU%\%VARIANT%\dist
if "%ALLJOYN_DIST_DIR%"=="" set ALLJOYN_DIST_DIR=%ALLJOYN_SRC_DIST_DIR%
if "%JOBS%"=="" set JOBS=1

echo Current directory: %CD%
echo OS: %OS%
echo CPU: %CPU%
echo VARIANT: %VARIANT%
echo BINDINGS: %BINDINGS%
echo SERVICE: %SERVICE%
echo ALLJOYN_DIST_DIR: %ALLJOYN_DIST_DIR%

rem For build server to cache core
rem --------------------------------------
rem TODO Move this to an upstream build process
IF "%SERVICE%"=="core" %SCRIPT_FOLDER%\build_windows_core.bat

rem Build service
rem ----------------
pushd %BASE_ROOT%\%SERVICE%
set ALLJOYN_DISTDIR=%ALLJOYN_DIST_DIR%
set MSVC_VERSION=%MSVC_VER%
if "%VERBOSE%"=="" set VERBOSE=1
set BUILD_SERVICES_SAMPLES=on
set BR=on
set DOCS=html
cmd /c %SCONS% "-c" "--jobs=%JOBS%" "OS=%OS%" "BINDINGS=%BINDINGS%" "VARIANT=%VARIANT%" "CPU=%CPU%" "MSVC_VERSION=%MSVC_VER%"
cmd /c %SCONS% "--jobs=%JOBS%" "OS=%OS%" "BINDINGS=%BINDINGS%" "VARIANT=%VARIANT%" "CPU=%CPU%" "MSVC_VERSION=%MSVC_VER%" "V=%VERBOSE%"
IF %ERRORLEVEL% GTR 0 exit %ERRORLEVEL% 
popd

