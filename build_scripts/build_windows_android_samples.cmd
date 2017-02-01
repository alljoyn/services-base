rem     Copyright (c) Open Connectivity Foundation (OCF), AllJoyn Open Source
rem     Project (AJOSP) Contributors and others.
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
rem     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
rem     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
rem     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
rem     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
rem     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
rem     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
rem     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
rem     PERFORMANCE OF THIS SOFTWARE.
rem
@echo off

:: This snippet is included in order to exit the batch file from the subroutine.
:: See http://superuser.com/questions/80485/exit-batch-file-from-subroutine for more details.
IF "%selfWrapped%"=="%~0" (
  REM this is necessary so that we can use "exit" to terminate the batch file,
  REM and all subroutines, but not the original cmd.exe
  SET selfWrapped=%~0
  %ComSpec% /s /c ""%~0" %*"
  GOTO :EOF
)

set OK=0
set ERROR=1

set ALLJOYN_BASE_BUILD_DIR=%BASE_ROOT%
set ALLJOYN_CORE_BUILD_DIR=%ALLJOYN_CORE_DIR%
set ALLJOYN_ANDROID_JAR_DIR=%ALLJOYN_CORE_BUILD_DIR%\arm\java\jar
set ALLJOYN_ANDROID_LIB_DIR=%ALLJOYN_CORE_BUILD_DIR%\arm\java\lib
set ANDROID_HOME=%ANDROID_SDK%
set CONTROL_PANEL_SAMPLES_ROOT=%ALLJOYN_BASE_BUILD_DIR%\controlpanel\java
set NOTIFICATION_SAMPLES_ROOT=%ALLJOYN_BASE_BUILD_DIR%\notification\java
set ONBOARDING_SAMPLES_ROOT=%ALLJOYN_BASE_BUILD_DIR%\onboarding\java
set CPU=arm

echo Building Common Libraries
call:buildCommonLibraries
echo Building Control Panel Sample Apps
call:buildControlPanelSamples
echo Building Notification Sample Apps
call:buildNotificationSamples
echo Building Onboarding Sample Apps
call:buildOnboardingSamples
exit %OK%

::::::::::::::::::::::::::::::::::::::
:: Generic build functions
::::::::::::::::::::::::::::::::::::::
:executeFuncInDir
    set func=%~1
    set dir=%~2
    pushd %dir%
    call:%func%
    popd
exit /B %OK%

:copyToDir
    set file=%~1
    set dir=%~2
    set errorMessage=%~3
    set errorCode=%~4
    xcopy /Y %file% %dir% || (
        echo %errorMesage%
        exit %errorCode%
    )
exit /B %OK%

:createLibsDir
    if not exist libs mkdir libs
exit /B %OK%

:createLibsARMDir
    if not exist libs\armeabi mkdir libs\armeabi
exit /B %OK%

:copyAlljoynJars
    for %%f in (%ALLJOYN_ANDROID_JAR_DIR%\alljoyn.jar, %ALLJOYN_ANDROID_JAR_DIR%\alljoyn_about.jar) do (
        call:copyToDir %%f .\libs\ "failed to get alljoyn jars" %ERROR%
    ) 
exit /B %OK%

:copyAndroidV4SupportJar
    call:copyToDir %ANDROID_HOME%\extras\android\support\v4\android-support-v4.jar .\libs\ "failed to copy android-support-v4.jar" %ERROR%
exit /B %OK%

:copyControlPanelServiceJar
    call:copyToDir %ALLJOYN_BASE_BUILD_DIR%\controlpanel\java\ControlPanelService\build\deploy\ControlPanelService.jar .\libs\ "failed to copy ControlPanelService.jar to libs directory" %ERROR%
exit /B %OK%

:copyAlljoynAndroidUtilsJar
    call:copyToDir %ALLJOYN_BASE_BUILD_DIR%\sample_apps\android\common_utils\build\deploy\alljoyn_apps_android_utils.jar .\libs\ "failed to copy alljoyn_apps_android_utils.jar" %ERROR%
exit /B %OK%

:copyAlljoynServicesCommonJar
    call:copyToDir %ALLJOYN_BASE_BUILD_DIR%\services_common\java\build\deploy\alljoyn_services_common.jar .\libs\ "failed to copy alljoyn_services_common.jar" %ERROR%
exit /B %OK%

:copyAlljoynSharedObject
    call:copyToDir %ALLJOYN_ANDROID_LIB_DIR%\liballjoyn_java.so .\libs\armeabi\ "failed to copy liballjoyn_java.so" %ERROR%
exit /B %OK%

:copyAlljoynOnboardingJar
    call:copyToDir %ALLJOYN_BASE_BUILD_DIR%\onboarding\java\OnboardingService\build\deploy\alljoyn_onboarding.jar .\libs\ "failed to copy alljoyn_onboarding.jar" %ERROR%
exit /B %OK%

:copyControlPanelAdapterJar
    call:copyToDir %ALLJOYN_BASE_BUILD_DIR%\controlpanel\java\ControlPanelAdapter\bin\ControlPanelAdapter.jar .\libs\ "failed to copy ControlPanelAdapter.jar" %ERROR%
exit /B %OK%

::::::::::::::::::::::::::::::::::::::::
:: Common Library build functions
::::::::::::::::::::::::::::::::::::::::
:buildCommonLibraries
    echo Building common java utils
    call:executeFuncInDir buildServicesCommonJar %ALLJOYN_BASE_BUILD_DIR%\services_common\java
    echo Building common android utils
    call:executeFuncInDir buildAndroidUtilsJar %ALLJOYN_BASE_BUILD_DIR%\sample_apps\android\common_utils
exit /B %OK%

:buildServicesCommonJar
    call:createLibsDir
    call:copyAlljoynJars
    call:copyToDir %ALLJOYN_ANDROID_JAR_DIR%\alljoyn_config.jar .\libs\ "Failed to copy android config jars" %ERROR%
    ant || (
        echo "failed to build services common jar"
        exit %ERROR% 
    )
exit /B %OK%

:buildAndroidUtilsJar
    call:createLibsDir
    call:copyAlljoynJars
    call:copyAlljoynServicesCommonJar
    call:copyToDir %ALLJOYN_ANDROID_JAR_DIR%\alljoyn_config.jar .\libs\ "Failed to copy android config jars" %ERROR%
    ant || (
        echo "failed to build android utils jar"
        exit %ERROR% 
    )
exit /B %OK%

::::::::::::::::::::::::::::::::::::::::
:: Control Panel build functions
::::::::::::::::::::::::::::::::::::::::
:buildControlPanelSamples
    echo Building ControlPanelService
    call:executeFuncInDir buildControlPanelService %CONTROL_PANEL_SAMPLES_ROOT%\ControlPanelService
    echo Building ControlPanelAdapter
    call:executeFuncInDir buildControlPanelAdapter %CONTROL_PANEL_SAMPLES_ROOT%\ControlPanelAdapter
    echo Building ControlPanelBrowser
    call:executeFuncInDir buildControlPanelBrowser %CONTROL_PANEL_SAMPLES_ROOT%\sample_applications\android\ControlPanelBrowser
exit /B %OK%

:buildControlPanelService
    call:createLibsDir
    call:copyAlljoynServicesCommonJar
    call:copyAlljoynJars
    ant || (
        echo "failed to build controlpanelservice"
        exit %ERROR%
    )
exit /B %OK%

:buildControlPanelAdapter
    call:createLibsDir
    call:copyAlljoynServicesCommonJar
    call:copyControlPanelServiceJar
    ant || (
        echo "failed to buld controlpaneladapter"
        exit %ERROR%
    )
exit /B %OK%

:buildControlPanelBrowser
    call:createLibsARMDir
    call:copyAlljoynJars
    call:copyControlPanelServiceJar
    call:copyAlljoynServicesCommonJar
    call:copyAlljoynAndroidUtilsJar
    call:copyAlljoynSharedObject
    call:copyControlPanelAdapterJar
    call:copyAndroidV4SupportJar
    ant || (
        echo "failed to build controlpanelbrowser"
        exit %ERROR%
    )
exit /B %OK%


:::::::::::::::::::::::::::::::::::::::::
:: Notification build functions
:::::::::::::::::::::::::::::::::::::::::
:buildNotificationSamples
    echo Building native notification service
    call:executeFuncInDir buildNativeNotificationService %NOTIFICATION_SAMPLES_ROOT%\native_platform\NotificationServiceNativePlatformAndroid
    echo Building notification ui sample app
    call:executeFuncInDir buildNotificationServiceUISample %NOTIFICATION_SAMPLES_ROOT%\sample_applications\android\NotificationServiceUISample
exit /B %OK%

:copyNotificationJars
    call:copyToDir %ALLJOYN_BASE_BUILD_DIR%\notification\java\native_platform\NotificationServiceNativePlatformAndroid\build\deploy\NSCommons.jar .\libs\ "failed to copy NSCommons.jar" %ERROR%
    call:copyToDir %ALLJOYN_BASE_BUILD_DIR%\notification\java\native_platform\NotificationServiceNativePlatformAndroid\build\deploy\NSNativePlatformAndr.jar .\libs\ "failed to copy NSNativePlatformAndr.jar" %ERROR%
    call:copyToDir %ALLJOYN_BASE_BUILD_DIR%\notification\java\native_platform\NotificationServiceNativePlatformAndroid\build\deploy\NotificationService.jar .\libs\ "failed to copy NotificationService.jar" %ERROR%
exit /B %OK%

:buildNativeNotificationService
    call:createLibsARMDir
    call:copyAlljoynJars
    call:copyAlljoynAndroidUtilsJar
    call:copyAlljoynServicesCommonJar
    call:copyConytolPanelServiceJar
    call:copyConytolPanelAdapterJar
    call:copyAlljoynSharedObject
    ant || (
        echo "failed to build nativeNotificationService"
        exit %ERROR%
    )
exit /B %OK%

:buildNotificationServiceUISample
    call:createLibsARMDir
    call:copyAlljoynJars
    call:copyAlljoynAndroidUtilsJar
    call:copyAlljoynServicesCommonJar
    call:copyControlPanelServiceJar
    call:copyControlPanelAdapterJar
    call:copyAlljoynSharedObject
    call:copyNotificationJars
    ant %VARIANT% || (
        echo "failed to build notificationServiceUISample"
        exit %ERROR%
    )
exit /B %OK%


:::::::::::::::::::::::::::::::::::::::::
:: Onboarding build functions
:::::::::::::::::::::::::::::::::::::::::
:buildOnboardingSamples
    echo Building OnboardingService sample
    call:executeFuncInDir buildOnboardingService %ONBOARDING_SAMPLES_ROOT%\OnboardingService 
    echo Building Dependency Onboarding About Configuration Server
    call:executeFuncInDir buildOnboardingAboutConfServer %ALLJOYN_BASE_BUILD_DIR%\simulators\android\about_conf_onb_server
    echo Building OnboardingSampleClient
    call:executeFuncInDir buildOnboardingSampleClient %ONBOARDING_SAMPLES_ROOT%\sample_applications\android\OnboardingSampleClient
exit /B %OK%

:buildOnboardingService
    call:createLibsDir
    call:copyAlljoynJars
    call:copyAlljoynAndroidUtilsJar
    call:copyAlljoynServicesCommonJar
    ant || (
        echo "failed to build onboarding service"
        exit %ERROR%
    )
exit /B %OK%

:buildOnboardingAboutConfServer
    call:createLibsARMDir
    call:copyAlljoynJars
    call:copyAlljoynAndroidUtilsJar
    call:copyAlljoynServicesCommonJar
    call:copyAlljoynOnboardingJar
    call:copyAlljoynSharedObject
    call:copyToDir %ALLJOYN_ANDROID_JAR_DIR%\alljoyn_config.jar .\libs\ "Failed to copy android config jars" %ERROR%
    ant || (
        echo "failed to build onboarding about conf server"
        exit %ERROR%
    )
exit /B %OK%

:buildOnboardingSampleClient
    call:createLibsARMDir
    call:copyAlljoynJars
    call:copyAlljoynAndroidUtilsJar
    call:copyAlljoynServicesCommonJar
    call:copyAlljoynOnboardingJar
    call:copyAlljoynSharedObject
    ant || (
        echo "failed to build onboarding sample client"
        exit %ERROR%
    )
exit /B %OK%
