/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

#ifndef CONTROLPANELPROVIDED_H_
#define CONTROLPANELPROVIDED_H_

#include <qcc/platform.h>
#include <stdio.h>
#include <alljoyn/controlpanel/CPSDate.h>
#include <alljoyn/controlpanel/CPSTime.h>

void disableFan();
void enableFan();
void disableTempSelect();
void enableTempSelect();

const char* getNotificationString();
uint16_t isThereANotificationToSend();

uint16_t getCurrentTargetTemp();
void checkTargetTempReached();

void setTemperatureFieldUpdate();
void setStatusFieldUpdate();
void setTempSelectorFieldUpdate();
void setFanSpeedSelectorFieldUpdate();

char const* getCurrentTemperatureString();
void setCurrentTemperatureString(char const* newTemp);
char const* getCurrentHumidityString();
void setCurrentHumidityString(char const* newHumidity);

uint16_t getTargetTemperature();
void setTargetTemperature(uint16_t newTemp);

uint16_t getCurrentMode();
void setCurrentMode(uint16_t newMode);

uint16_t getFanSpeed();
void setFanSpeed(uint16_t newSpeed);

const char* getStatusString();
void setStatusString(const char* newStatusString);

uint8_t checkForUpdatesToSend();

void OnTurnFanOnButton(bool chooseYes);
void OnTurnFanOffButton(bool chooseYes);

bool getOfferToTurnOffTheFan();
void setOfferToTurnOffTheFan(bool turnOffTheFan);

bool getOfferToTurnOnTheFan();
void setOfferToTurnOnTheFan(bool turnOnTheFan);

uint8_t checkForEventsToSend();
void resetEventsToSend();
uint8_t getEventsToSend();

#endif /* CONTROLPANELPROVIDED_H_ */