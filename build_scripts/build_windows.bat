rem  Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
rem     Source Project (AJOSP) Contributors and others.
rem
rem     SPDX-License-Identifier: Apache-2.0
rem
rem     All rights reserved. This program and the accompanying materials are
rem     made available under the terms of the Apache License, Version 2.0
rem     which accompanies this distribution, and is available at
rem     http://www.apache.org/licenses/LICENSE-2.0
rem
rem     Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
rem     Alliance. All rights reserved.
rem
rem     Permission to use, copy, modify, and/or distribute this software for
rem     any purpose with or without fee is hereby granted, provided that the
rem     above copyright notice and this permission notice appear in all
rem     copies.
rem
rem      THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
rem      WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
rem      WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
rem      AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
rem      DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
rem      PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
rem      TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
rem      PERFORMANCE OF THIS SOFTWARE.
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

