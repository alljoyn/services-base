/******************************************************************************
 * Copyright (c) 2013 - 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

/**
 * Per-module definition of the current module for debug logging.  Must be defined
 * prior to first inclusion of aj_debug.h.
 * The corresponding flag dbgAJSVCAPP is defined in the containing sample app.
 */
#define AJ_MODULE AJSVCAPP
#include <aj_debug.h>

#include "ControlPanelProvided.h"
#include "ControlPanelGenerated.h"
#include "alljoyn/controlpanel/Common/BaseWidget.h"

#ifndef snprintf
#include <stdio.h>
#endif

#ifndef NDEBUG
extern AJ_EXPORT uint8_t dbgAJSVCAPP;
#endif

////////////////////////////////////////////////////////////////

static uint16_t currentHumidity = 40;
static char humidityBuff[11] = "40 %%";
static char* humidityString = humidityBuff;
static uint16_t currentTemperature = 72;
static char temperatureBuff[11] = "72 F";
static char* temperatureString = temperatureBuff;
static uint16_t targetTemp = 68;
static uint16_t prevTargetTemp = 68;
static uint16_t currentMode = 4;
static uint16_t previousMode = 4;
static uint16_t fanSpeed = 1;
static uint16_t previousFanSpeed = 1;
static char statusText[31] = "Unit is off";
static char* statusString = statusText;
static uint16_t triggerAnUpdate = 0;
static char notificationText[51] = "Notification text goes here";
static char* notificationString = notificationText;
static uint16_t sendANotification = 0;
static uint8_t signalsToSend = 0;
static uint8_t modeOrTargetTempChanged = 0;

void disableFan()
{
    setBaseEnabled(&MyDeviceFan_speed.base, FALSE);
}

void enableFan()
{
    setBaseEnabled(&MyDeviceFan_speed.base, TRUE);
}

void disableTempSelect()
{
    setBaseEnabled(&MyDeviceSet_temperature.base, FALSE);
}

void enableTempSelect()
{
    setBaseEnabled(&MyDeviceSet_temperature.base, TRUE);
}

const char* getNotificationString()
{
    sendANotification = 0;
    return notificationString;
}

uint16_t isThereANotificationToSend()
{
    return sendANotification;
}

// -- for string properties -- //
uint16_t getCurrentTargetTemp()
{
    return targetTemp;
}

uint16_t getCurrentTemp()
{
    return currentTemperature;
}

// -- for widgets --//

void* getCurrentTemperatureString()
{
    temperatureBuff[snprintf(temperatureString, sizeof(temperatureBuff), "%d F", currentTemperature)] = '\0';
    return &temperatureString;
}

void setCurrentTemperatureString(char const* newTemp)
{
    //do nothing
}

void* getCurrentHumidityString()
{
    humidityBuff[snprintf(humidityString, sizeof(humidityBuff), "%d %%", currentHumidity)] = '\0';
    return &humidityString;
}

void setCurrentHumidityString(char const* newHumidity)
{
    //do nothing
}

void* getTargetTemperature()
{
    return &targetTemp;
}
void setTargetTemperature(uint16_t newTemp)
{
    targetTemp = newTemp;
}

void* getCurrentMode()
{
    return &currentMode;
}

void setCurrentMode(uint16_t newMode)
{
    currentMode = newMode;
}

void* getFanSpeed()
{
    return &fanSpeed;
}

void setFanSpeed(uint16_t newSpeed)
{
    fanSpeed = newSpeed;
}

void* getStatusString()
{
    return &statusString;
}

void setStatusString(const char* newStatusString)
{
    strncpy(statusString, newStatusString, sizeof(statusText));
    statusString[30] = '\0';
}

void checkTargetTempReached()
{
    if (currentTemperature == targetTemp) {
        statusText[snprintf(statusString, sizeof(statusText), "Target temp reached")] = '\0';
        setStatusFieldUpdate();
        notificationText[snprintf(notificationString, sizeof(notificationText), "Target temperature of %d F reached", targetTemp)] = '\0';
        sendANotification = 1;
    }
}

void setTemperatureFieldUpdate() {
    signalsToSend |= 1 << 0;
}

void setStatusFieldUpdate() {
    signalsToSend |= 1 << 1;
}

void setTempSelectorFieldUpdate() {
    signalsToSend |= 1 << 2;
}

void setFanSpeedSelectorFieldUpdate() {
    signalsToSend |= 1 << 3;
}

uint8_t checkForUpdatesToSend()
{
    // this needs to be the brain
    // check for what mode we are in and what the current & target temps are
    // figure out if we are heating, cooling, doing nothing

    // mode
    //0 == auto
    //1 == cool
    //2 == heat
    //3 == fan
    //4 == off

    signalsToSend = 0;
    // 0001 == need to update the temperature text field
    // 0010 == need to update the status text field
    // 0100 == need to update the state of temperature selector
    // 1000 == need to update the state of fan speed selector

    modeOrTargetTempChanged = 0;

    AJ_AlwaysPrintf(("In checkForUpdatesToSend, currentMode=%d, targetTemp=%d, currentTemperature=%d, fanSpeed=%d, triggerAnUpdate=%d \n", currentMode, targetTemp, currentTemperature, fanSpeed, triggerAnUpdate));

    // check if the target temperature has been changed & update accordingly
    if (targetTemp != prevTargetTemp) {
        AJ_AlwaysPrintf(("##### targetTemp (%d) != prevTargetTemp (%d) \n", targetTemp, prevTargetTemp));
        modeOrTargetTempChanged = 1;

        prevTargetTemp = targetTemp;
        setStatusFieldUpdate();

        if (currentMode == 0) {
            // auto mode
            if (targetTemp > currentTemperature) {
                //heating
                statusText[snprintf(statusString, sizeof(statusText), "Heating to %d F", targetTemp)] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Target temperature changed, now heating to %d F", targetTemp)] = '\0';
                sendANotification = 1;
            } else if (targetTemp < currentTemperature) {
                //cooling
                statusText[snprintf(statusString, sizeof(statusText), "Cooling to %d F", targetTemp)] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Target temperature changed, now cooling to %d F", targetTemp)] = '\0';
                sendANotification = 1;
            } else {
                //target temp reached
                statusText[snprintf(statusString, sizeof(statusText), "Target temp reached")] = '\0';
                snprintf(notificationString, sizeof(notificationText), "Target temperature of %d F reached \n", targetTemp);
                sendANotification = 1;
            }
        } else if (currentMode == 1) {
            // cooling mode
            if (targetTemp < currentTemperature) {
                //cooling
                statusText[snprintf(statusString, sizeof(statusText), "Cooling to %d F", targetTemp)] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Target temperature changed, now cooling to %d F", targetTemp)] = '\0';
                sendANotification = 1;
            } else if (targetTemp == currentTemperature) {
                //target temp reached
                statusText[snprintf(statusString, sizeof(statusText), "Target temp reached")] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Target temperature of %d F reached", targetTemp)] = '\0';
                sendANotification = 1;
            } else {
                // user set target temp higher than current temp, do nothing
                statusText[snprintf(statusString, sizeof(statusText), "Idle")] = '\0';
            }
        } else if (currentMode == 2) {
            // heating mode
            if (targetTemp > currentTemperature) {
                //heating
                statusText[snprintf(statusString, sizeof(statusText), "Heating to %d F", targetTemp)] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Target temperature changed, now heating to %d F", targetTemp)] = '\0';
                sendANotification = 1;
            } else if (targetTemp == currentTemperature) {
                //target temp reached
                statusText[snprintf(statusString, sizeof(statusText), "Target temp reached")] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Target temperature of %d F reached", targetTemp)] = '\0';
                sendANotification = 1;
            } else {
                // user set target temp lower than current temp, do nothing
                statusText[snprintf(statusString, sizeof(statusText), "Idle")] = '\0';
            }
        } else {
            // fan mode or off, don't do anything
        }
    }

    //check if the mode has been changed & update accordingly
    if (currentMode != previousMode) {
        AJ_AlwaysPrintf(("##### currentMode (%d) != previousMode (%d) \n", currentMode, previousMode));
        modeOrTargetTempChanged = 1;

        previousMode = currentMode;
        setStatusFieldUpdate();

        if (currentMode == 0) {
            // auto mode
            if (targetTemp > currentTemperature) {
                //heating
                statusText[snprintf(statusString, sizeof(statusText), "Heating to %d F", targetTemp)] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Mode changed to Auto, now heating to %d F", targetTemp)] = '\0';
                sendANotification = 1;
            } else if (targetTemp < currentTemperature) {
                //cooling
                statusText[snprintf(statusString, sizeof(statusText), "Cooling to %d F", targetTemp)] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Mode changed to Auto, now cooling to %d F", targetTemp)] = '\0';
                sendANotification = 1;
            } else {
                //target temp already reached
                statusText[snprintf(statusString, sizeof(statusText), "Idle")] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Mode changed to Auto")] = '\0';
                sendANotification = 1;
            }

            enableTempSelect();
            disableFan();
            setTempSelectorFieldUpdate();
            setFanSpeedSelectorFieldUpdate();
        } else if (currentMode == 1) {
            // cooling mode
            if (targetTemp < currentTemperature) {
                //cooling
                statusText[snprintf(statusString, sizeof(statusText), "Cooling to %d F", targetTemp)] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Mode changed to Cool, now cooling to %d F", targetTemp)] = '\0';
                sendANotification = 1;
            } else {
                //target temp already reached or set higher than current temp
                statusText[snprintf(statusString, sizeof(statusText), "Idle")] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Mode changed to Cool")] = '\0';
                sendANotification = 1;
            }

            enableTempSelect();
            disableFan();
            setTempSelectorFieldUpdate();
            setFanSpeedSelectorFieldUpdate();
        } else if (currentMode == 2) {
            // heating mode
            if (targetTemp > currentTemperature) {
                //heating
                statusText[snprintf(statusString, sizeof(statusText), "Heating to %d F", targetTemp)] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Mode changed to Heat, now heating to %d F", targetTemp)] = '\0';
                sendANotification = 1;
            } else {
                //target temp already reached or set lower than current temp
                statusText[snprintf(statusString, sizeof(statusText), "Idle")] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Mode changed to Heat")] = '\0';
                sendANotification = 1;
            }

            enableTempSelect();
            disableFan();
            setTempSelectorFieldUpdate();
            setFanSpeedSelectorFieldUpdate();
        } else if (currentMode == 3) {
            // In fan mode
            //0==low
            //1==medium
            //2==high
            if (fanSpeed == 0) {
                statusText[snprintf(statusString, sizeof(statusText), "Fan on low")] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Mode changed to Fan, fan on low")] = '\0';
                sendANotification = 1;
            } else if (fanSpeed == 1) {
                statusText[snprintf(statusString, sizeof(statusText), "Fan on medium")] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Mode changed to Fan, fan on medium")] = '\0';
                sendANotification = 1;
            } else {
                statusText[snprintf(statusString, sizeof(statusText), "Fan on high")] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Mode changed to Fan, fan on high")] = '\0';
                sendANotification = 1;
            }

            // if in fan mode, disable the temperature selector
            enableFan();
            disableTempSelect();
            setTempSelectorFieldUpdate();
            setFanSpeedSelectorFieldUpdate();
        } else {
            // Off
            statusText[snprintf(statusString, sizeof(statusText), "Unit is off")] = '\0';
            notificationText[snprintf(notificationString, sizeof(notificationText), "Unit has been turned off")] = '\0';
            sendANotification = 1;

            //if unit mode == off, disable temperature selector & fan widgets
            disableFan();
            disableTempSelect();
            setTempSelectorFieldUpdate();
            setFanSpeedSelectorFieldUpdate();
        }

    }

    if (currentMode == 3) {
        // In fan mode
        //0==low
        //1==medium
        //2==high
        if (fanSpeed != previousFanSpeed) {
            setStatusFieldUpdate();
            previousFanSpeed = fanSpeed;
            if (fanSpeed == 0) {
                statusText[snprintf(statusString, sizeof(statusText), "Fan on low")] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Fan on low")] = '\0';
                sendANotification = 1;
            } else if (fanSpeed == 1) {
                statusText[snprintf(statusString, sizeof(statusText), "Fan on medium")] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Fan on medium")] = '\0';
                sendANotification = 1;
            } else {
                statusText[snprintf(statusString, sizeof(statusText), "Fan on high")] = '\0';
                notificationText[snprintf(notificationString, sizeof(notificationText), "Fan on high")] = '\0';
                sendANotification = 1;
            }
        }
    }

    // check if we need to simulate changing the temperature
    if (targetTemp != currentTemperature) {
        AJ_AlwaysPrintf(("##### target temp (%d) != current temp (%d) \n", targetTemp, currentTemperature));

        if (modeOrTargetTempChanged == 1) {
            modeOrTargetTempChanged = 0;
        } else {
            if (currentMode == 0) {
                // auto mode
                if (targetTemp > currentTemperature) {
                    //heating
//          previousTemperature = currentTemperature;
                    currentTemperature++;
                    setTemperatureFieldUpdate();
                    checkTargetTempReached();
                } else if (targetTemp < currentTemperature) {
                    //cooling
//          previousTemperature = currentTemperature;
                    currentTemperature--;
                    setTemperatureFieldUpdate();
                    checkTargetTempReached();
                }
            } else if (currentMode == 1) {
                if (targetTemp < currentTemperature) {
                    //cooling
//          previousTemperature = currentTemperature;
                    currentTemperature--;
                    setTemperatureFieldUpdate();
                    checkTargetTempReached();
                }
            } else if (currentMode == 2) {
                if (targetTemp > currentTemperature) {
                    //heating
//          previousTemperature = currentTemperature;
                    currentTemperature++;
                    setTemperatureFieldUpdate();
                    checkTargetTempReached();
                }
            } else {
                // mode is either fan only or off, so don't need to do anything
            }
        }


    }

    return signalsToSend;
}

