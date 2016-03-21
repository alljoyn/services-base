pushd %ALLJOYN_CORE_DIR%
set MSVC_VERSION=%MSVC_VER%" 
set OS=%OS%
set BINDINGS=%BINDINGS%
set BR=off
set WS=off
set CPU=%CPU%
set VARIANT=%VARIANT%
cmd /c %SCONS% "-c" "--jobs=%JOBS%" "V=1" "OS=%OS%" "BINDINGS=%BINDINGS%" "VARIANT=%VARIANT%" "CPU=%CPU%" "MSVC_VERSION=%MSVC_VER%"
cmd /c %SCONS% "--jobs=%JOBS%" "V=1" "OS=%OS%" "BINDINGS=%BINDINGS%" "VARIANT=%VARIANT%" "CPU=%CPU%" "MSVC_VERSION=%MSVC_VER%"
echo Build completed
IF %ERRORLEVEL% GTR 0 exit %ERRORLEVEL%
popd
setlocal enableextensions
md %ALLJOYN_DIST_DIR%
rd /s /q %ALLJOYN_DIST_DIR%
xcopy /E %ALLJOYN_SRC_DIST_DIR% %ALLJOYN_DIST_DIR%\
echo Core cached!!!
endlocal 
exit 0
