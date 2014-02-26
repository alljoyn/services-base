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
#include <alljoyn/controlpanel/Definitions.h>

#ifndef snprintf
#include <stdio.h>
#endif

#ifndef NDEBUG
extern AJ_EXPORT uint8_t dbgAJSVCAPP;
#endif

static uint16_t uint16Var = 0;
static int16_t int16Var = 0;

static uint32_t uint32Var = 0;
static int32_t int32Var = 0;

static uint64_t uint64Var = 0;
static int64_t int64Var = 0;

static double doubleVar = 0;

static char initialString[100] = "75 F";
static char* stringVar = initialString;

static const char* sampleString = "This is a test";
static const char* sampleUrlString = "www.ControlPanelTest.com";

static DatePropertyValue date = { .fullYear = 2006, .month = 6, .mDay = 13 };
static TimePropertyValue time = { .hour = 18, .minute = 0, .second = 0 };

void addDismissSignal(ExecuteActionContext* context, int32_t dismissSignal)
{
    context->numSignals = 1;
    context->signals[0].signalId = dismissSignal;
    context->signals[0].signalType = SIGNAL_TYPE_DISMISS;
}

const char* getUrlString()
{
    return sampleUrlString;
}

void* getDateProperty()
{
    return &date;
}

void setDateProperty(DatePropertyValue* datePropertyValue)
{
    date.fullYear = datePropertyValue->fullYear;
    date.month = datePropertyValue->month;
    date.mDay = datePropertyValue->mDay;
}

void* getTimeProperty()
{
    return &time;
}
void setTimeProperty(TimePropertyValue* timePropertyValue)
{
    time.hour = timePropertyValue->hour;
    time.minute = timePropertyValue->minute;
    time.second = timePropertyValue->second;
}

uint8_t getEnabledFunc()
{
    return TRUE;
}
uint8_t getWriteableFunc()
{
    return TRUE;
}

const char* getTestString(uint16_t language)
{
    return sampleString;
}

void* getuint16Var()
{
    return &uint16Var;
}

void setuint16Var(uint16_t newuint16Var)
{
    uint16Var = newuint16Var;
}

void* getint16Var()
{
    return &int16Var;
}

void setint16Var(int16_t newint16Var)
{
    int16Var = newint16Var;
}

void* getuint32Var()
{
    return &uint32Var;
}

void setuint32Var(uint32_t newuint32Var)
{
    uint32Var = newuint32Var;
}

void* getint32Var()
{
    return &int32Var;
}

void setint32Var(int32_t newint32Var)
{
    int32Var = newint32Var;
}

void* getuint64Var()
{
    return &uint64Var;
}

void setuint64Var(uint64_t newuint64Var)
{
    uint64Var = newuint64Var;
}

void* getint64Var()
{
    return &int64Var;
}

void setint64Var(int64_t newint64Var)
{
    int64Var = newint64Var;
}

void* getdoubleVar()
{
    return &doubleVar;
}

void setdoubleVar(double newdoubleVar)
{
    doubleVar = newdoubleVar;
}

void* getStringVar()
{
    return &stringVar;
}

void setStringVar(const char* newStringVar)
{
    strncpy(stringVar, newStringVar, 99);
    stringVar[99] = '\0';
}

/* For Oven.XML */

static int temperature = 100;
static int program = 0;
static const char* programString = "Program";

void startOven()
{
    AJ_AlwaysPrintf(("**************** Starting the Oven ****************\n"));
}

void stopOven()
{
    AJ_AlwaysPrintf(("**************** Stopping the Oven ****************\n"));
}

void*getTemperature()
{
    return &temperature;
}

void setTemperature(uint16_t t)
{
    temperature = t;
}

void*getProgram()
{
    return &program;
}

void setProgram(uint16_t p)
{
    program = p;
}

const char* getProgramString(uint16_t language)
{
    return programString;
}
