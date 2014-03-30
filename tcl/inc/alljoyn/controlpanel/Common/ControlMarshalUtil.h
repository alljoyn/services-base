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

#ifndef CONTROL_MARSHAL_UTIL_H_
#define CONTROL_MARSHAL_UTIL_H_

#include <alljoyn.h>
#include <alljoyn/controlpanel/Definitions.h>
#include <alljoyn/controlpanel/Common/BaseWidget.h>

#define StartOptionalParams(reply, arg) AJ_MarshalContainer(reply, arg, AJ_ARG_ARRAY)

/**
 * Add a layout Hint. Can be used for all kinds of MetaData (Property, Action etc)
 * @param reply
 * @param hints
 * @param numHints
 * @return aj_status
 */
AJ_Status AddHints(AJ_Message* reply, const uint16_t hints[], uint16_t numHints);

/**
 * Add Constraint Value
 * @param reply
 * @param sig
 * @param value
 * @param displayValue
 * @return aj_status
 */
AJ_Status AddConstraintValue(AJ_Message* reply, const char* sig, const void* value, const char* displayValue);

/**
 * Add ConstraintRange component
 * @param reply
 * @param valueSig
 * @param min
 * @param max
 * @param increment
 * @return aj_status
 */
AJ_Status AddConstraintRange(AJ_Message* reply, const char* valueSig, const void* min, const void* max, const void* increment);

/**
 * Start a Complex OptionalParam component
 * @param reply
 * @param arg
 * @param key
 * @param sig
 * @return aj_status
 */
AJ_Status StartComplexOptionalParam(AJ_Message* reply, AJ_Arg* arg, uint16_t key, const char* sig);

/**
 * Add a basic type Optional Param
 * @param reply
 * @param key
 * @param sig
 * @param value
 * @return aj_status
 */
AJ_Status AddBasicOptionalParam(AJ_Message* reply, uint16_t key, const char* sig, const void* value);

/**
 * Add a property for GetAll response
 * @param reply
 * @param key
 * @param sig
 * @param widget
 * @param language
 * @param functionPtr
 * @return aj_status
 */
AJ_Status AddPropertyForGetAll(AJ_Message* reply, char* key, const char* sig,
                               BaseWidget* widget, uint16_t lang, MarshalWidgetFptr functionPtr);

/**
 * Helper functions. Receives sig value and void* and does the marshalling
 * @param reply
 * @param sig
 * @param value
 * @return status
 */
AJ_Status MarshalVariant(AJ_Message* reply, const char* sig, const void* value);

/**
 * MarshalAllRootProperties - includes only version
 * @param reply
 * @return aj_status
 */
AJ_Status MarshalAllRootProperties(AJ_Message* reply);

/**
 * Marshal Version Property for root interfaces
 * @param reply
 * @return aj_status
 */
AJ_Status MarshalVersionRootProperties(AJ_Message* reply);

#endif /* CONTROL_SERVICE_H_ */
