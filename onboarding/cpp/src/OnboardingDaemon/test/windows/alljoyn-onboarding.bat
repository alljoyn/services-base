REM    Copyright (c) 2014, AllSeen Alliance. All rights reserved.
REM
REM       Permission to use, copy, modify, and/or distribute this software for any
REM       purpose with or without fee is hereby granted, provided that the above
REM       copyright notice and this permission notice appear in all copies.
REM
REM       THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
REM       WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
REM       MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
REM       ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
REM       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
REM       ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
REM       OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
@echo off

REM Todo: Error handling/Messages need to be compliant to Onboarding HLD

REM Todo: Should be params, not hard coded
set ssid=[mySsid]
set authentication=WPA2PSK
set encryption=AES
set key=[myPassword]

set localDir=[Current_Directory_Where_Scripts_Are_Stored]
REM Todo: need to find the powershell path
set PowerShell=[PowerShell_FullPath]



IF "%1"=="configure" GOTO configure
IF "%1"=="connect" GOTO connect
IF "%1"=="offboard" GOTO offboard

set emptyProfile=EmptyProfile.xml
set newProfile="%localDir%\Wireless Network Connection-%ssid%.xml"

del %newProfile%

echo "starting..."

:configure
echo "Step 1: Enable Profile"

set enableProfiles=netsh wlan set createalluserprofile enable=yes

call %enableProfiles% > quiet
echo errorlevel 1 = %errorlevel%


echo "Step 2: Create Empty Profile"

copy  %emptyProfile% %newProfile% > quiet
echo errorlevel 2 = %errorlevel%


echo "Step 3: Replace text in profile"
%PowerShell% Set-ExecutionPolicy unrestricted
%PowerShell%  -File %localDir%\replaceString.ps1 "%localDir%\Wireless Network Connection-%ssid%.xml" "%ssid%"


echo "Step 4: Add Profile"
netsh wlan add profile filename=%newProfile% > quiet
echo errorlevel 4 = %errorlevel%


echo "Step 5: Edit Profile"

netsh wlan set profileparameter name=%ssid% SSIDName=%ssid% authentication=%authentication% encryption=%encryption% keyMaterial=%key% keyType=passphrase
echo errorlevel 5 = %errorlevel%

:connect

echo "Step 6: Connect To Profile"

netsh wlan connect ssid=%ssid% name=%ssid% > quiet
echo errorlevel 6 = %errorlevel%
goto done

REM Todo complete offboard
REM I could not make my machine an Access Point but this may be due to firewall issues.
:offboard
echo "Step 1: Set hosted network"

REM Get mac address
REM is slow at times, may need to look for an alternative
for /f "tokens=*" %%V in (
    '%PowerShell%  -File %localDir%\getMac.ps1') do set MacAddress=%%V

set hostedSSID=AJ_Windows %MacAddress%
echo %hostedSSID%

netsh wlan set hostednetwork mode=allow ssid="%hostedSSID%"
netsh wlan start hostednetwork

echo "Step 2: Share network"
%PowerShell%  -File %localDir%\setAccessPoint.ps1

REM May need to wait for the hostednetwork. netsh wlan show hosted shows its started but the ControlPanel sees it as "Identifying"

:done
del quiet

