rem  rem  
rem     Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
rem     Source Project Contributors and others.
rem     
rem     All rights reserved. This program and the accompanying materials are
rem     made available under the terms of the Apache License, Version 2.0
rem     which accompanies this distribution, and is available at
rem     http://www.apache.org/licenses/LICENSE-2.0

rem
pushd %ALLJOYN_CORE_DIR%
set MSVC_VERSION=%MSVC_VER%
set OS=%OS%
set BINDINGS=%BINDINGS%
set BR=off
set WS=off
set CPU=%CPU%
set VARIANT=%VARIANT%

cmd /c %SCONS% "--jobs=%JOBS%" "V=1" "OS=%OS%" "BINDINGS=%BINDINGS%" "VARIANT=%VARIANT%" "CPU=%CPU%" "MSVC_VERSION=%MSVC_VER%"
echo Build completed
IF %ERRORLEVEL% GTR 0 exit %ERRORLEVEL%
popd
setlocal enableextensions
rem TODO rename ALLJOYN_DIST_DIR (batch script argument) such that it isn't confused with ALLJOYN_DISTDIR (SCons argument)
rd /s /q %ALLJOYN_DIST_DIR%
md %ALLJOYN_DIST_DIR%
xcopy /e %ALLJOYN_SRC_DIST_DIR% %ALLJOYN_DIST_DIR%\
echo Core cached!!!
endlocal 
exit 0