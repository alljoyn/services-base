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
pushd %ALLJOYN_CORE_DIR%
set MSVC_VERSION=%MSVC_VER%" 
set BR=off
set WS=off
cmd /c %SCONS% "-c" "--jobs=%JOBS%" "OS=%OS%" "BINDINGS=%BINDINGS%" "VARIANT=%VARIANT%" "CPU=%CPU%" "MSVC_VERSION=%MSVC_VER%" "ANDROID_SDK=%ANDROID_SDK%" "ANDROID_NDK=%ANDROID_NDK%"
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
