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
set MSVC_VERSION=%MSVC_VER%" 
set BR=off
set WS=off
cmd /c %SCONS% "--jobs=%JOBS%" "OS=%OS%" "BINDINGS=%BINDINGS%" "VARIANT=%VARIANT%" "CPU=%CPU%" "MSVC_VERSION=%MSVC_VER%" "ANDROID_SDK=%ANDROID_SDK%" "ANDROID_NDK=%ANDROID_NDK%"
echo Build completed
IF %ERRORLEVEL% GTR 0 exit %ERRORLEVEL%
popd
setlocal enableextensions
rd /s /q %ALLJOYN_DIST_DIR%
md %ALLJOYN_DIST_DIR%
xcopy /e /i %ALLJOYN_SRC_DIST_DIR%\java\jar %ALLJOYN_DIST_DIR%\java\jar
xcopy /e /i %ALLJOYN_SRC_DIST_DIR%\java\lib %ALLJOYN_DIST_DIR%\java\lib
echo Core cached!!!
endlocal 
exit 0