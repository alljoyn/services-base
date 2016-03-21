rem  Copyright AllSeen Alliance. All rights reserved.
rem
rem     Permission to use, copy, modify, and/or distribute this software for any
rem     purpose with or without fee is hereby granted, provided that the above
rem     copyright notice and this permission notice appear in all copies.
rem
rem     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
rem     WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
rem     MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
rem     ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
rem     WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
rem     ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
rem     OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
pushd %WORKSPACE%\services\base\%SERVICE%
set CPU=%CPU%
set ALLJOYN_DISTDIR=%ALLJOYN_DIST_DIR%
set MSVC_VERSION=%MSVC_VER%
set V=1
set OS=%OS%
set BINDINGS=%BINDINGS%
set BUILD_SERVICES_SAMPLES=on
set BR=on
set DOCS=html
set WS=%WS%
set VARIANT=%VARIANT%
cmd /c %SCONS% "-c" "--jobs=%JOBS%" "OS=%OS%" "BINDINGS=%BINDINGS%" "VARIANT=%VARIANT%" "CPU=%CPU%" "MSVC_VERSION=%MSVC_VER%"
cmd /c %SCONS% "--jobs=%JOBS%" "OS=%OS%" "BINDINGS=%BINDINGS%" "VARIANT=%VARIANT%" "CPU=%CPU%" "MSVC_VERSION=%MSVC_VER%"
IF %ERRORLEVEL% GTR 0 exit %ERRORLEVEL% 
popd


