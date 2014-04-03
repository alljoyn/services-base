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

#ifndef CONTROLPANELPROVIDED_H_
#define CONTROLPANELPROVIDED_H_

#include "alljoyn.h"
#include "alljoyn/controlpanel/Common/DateTimeUtil.h"
#include "ControlPanelGenerated.h"

//#define UNICODE_HI_IN_HEBREW "\xd7\xa9\xd7\x9c\xd7\x95\xd7\x9d\x00"
//#define UNICODE_HI_IN_CHINESE "\xe5\x97\xa8"
//
//void addDismissSignal(ExecuteActionContext* context, int32_t dismissSignal);
//
//uint8_t getEnabledFunc();
//uint8_t getWriteableFunc();
//const char* getTestString(uint16_t language);
//const char* getUrlString();
//
//void* getDateProperty(PropertyWidget* thisWidget);
//void setDateProperty(DatePropertyValue* datePropertyValue);
//
//void* getTimeProperty(PropertyWidget* thisWidget);
//void setTimeProperty(TimePropertyValue* timePropertyValue);
//
//void* getuint16Var(PropertyWidget* thisWidget);
//void setuint16Var(uint16_t newuint16Var);
//
//void* getint16Var(PropertyWidget* thisWidget);
//void setint16Var(int16_t newint16Var);
//
//void* getuint32Var(PropertyWidget* thisWidget);
//void setuint32Var(uint32_t newuint32Var);
//
//void* getint32Var(PropertyWidget* thisWidget);
//void setint32Var(int32_t newint32Var);
//
//void* getuint64Var(PropertyWidget* thisWidget);
//void setuint64Var(uint64_t newuint64Var);
//
//void* getint64Var(PropertyWidget* thisWidget);
//void setint64Var(int64_t newint64Var);
//
//void* getdoubleVar(PropertyWidget* thisWidget);
//void setdoubleVar(double newdoubleVar);
//
//void* getStringVar(PropertyWidget* thisWidget);
//void setStringVar(const char* newStringVar);

//////////////////////////////////////////////////////

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

//char const* getCurrentTemperatureString(uint16_t lang);
//char const* getCurrentHumidityString(uint16_t lang);

void* getCurrentTemperatureString(PropertyWidget* thisWidget);
void setCurrentTemperatureString(char const* newTemp);
void* getCurrentHumidityString(PropertyWidget* thisWidget);
void setCurrentHumidityString(char const* newHumidity);

void* getTargetTemperature(PropertyWidget* thisWidget);
void setTargetTemperature(uint16_t newTemp);

void* getCurrentMode(PropertyWidget* thisWidget);
void setCurrentMode(uint16_t newMode);

void* getFanSpeed(PropertyWidget* thisWidget);
void setFanSpeed(uint16_t newSpeed);

void* getStatusString(PropertyWidget* thisWidget);
void setStatusString(const char* newStatusString);

//void simulateTemperatureChange();
uint8_t checkForUpdatesToSend();

#endif /* CONTROLPANELPROVIDED_H_ */

