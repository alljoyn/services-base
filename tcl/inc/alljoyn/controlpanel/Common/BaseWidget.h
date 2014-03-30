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

#ifndef BASEWIDGET_H_
#define BASEWIDGET_H_

/** @defgroup ConstraintList
 *
 *  @{
 */

#include <alljoyn.h>

#ifndef UINT32_MAX
#define UINT32_MAX      (4294967295U) //!< Maximum integer value
#endif

struct BaseWidget;

/////////////////////////* BaseWidget OptParams *//////////////////////////////////////

/**
 * BaseOptParams structure
 */
typedef struct {
    const char* const* label;                                         //!< The url of the Http control
    const char* (*getLabel)(struct BaseWidget* thisWidget, uint16_t); //!< The url of the Http control

    uint32_t bgColor;                                                 //!< The url of the Http control
    uint32_t (*getBgColor)(struct BaseWidget* thisWidget);            //!< The url of the Http control

    uint16_t numHints;                                                //!< The url of the Http control
    const uint16_t* hints;                                            //!< The url of the Http control

} BaseOptParams;

/**
 * Initialize BaseOptParam
 * @param optParam
 */
void initializeBaseOptParam(BaseOptParams* optParam);

/////////////////////////*     BaseWidget     *//////////////////////////////////////

/**
 * Funtion pointer for marshaling a given widget structure
 */
typedef AJ_Status (*MarshalWidgetFptr)(struct BaseWidget*, AJ_Message*, uint16_t language);

/**
 * BaseWidget structure
 */
typedef struct BaseWidget {
    uint16_t interfaceVersion;                             //!< Version of the widget interface
    uint32_t states;                                       //!< States bitmap
    uint8_t (*getEnabled)(struct BaseWidget* thisWidget);  //!< Getter for Enabled attribute of widget
    uint8_t (*getWritable)(struct BaseWidget* thisWidget); //!< Getter for Writable attribute of widget

    uint16_t numLanguages;                                 //!< Number of languages
    BaseOptParams optParams;                               //!< Base optional parameters

    MarshalWidgetFptr marshalVersion;                      //!< Funtion pointer to marshal the Version
    MarshalWidgetFptr marshalStates;                       //!< Funtion pointer to marshal the States
    MarshalWidgetFptr marshalOptParam;                     //!< Funtion pointer to marshal the OptParam
    MarshalWidgetFptr marshalAllProp;                      //!< Funtion pointer to marshal All the Properties

} BaseWidget;

/**
 * Initialize BaseWidget
 * @param widget
 */
void initializeBaseWidget(BaseWidget* widget);

/**
 * Set Enabled state of widget
 * @param widget
 * @param enabled
 */
void setBaseEnabled(BaseWidget* widget, uint8_t enabled);

/**
 * Set Writable state of widget
 * @param widget
 * @param writeable
 */
void setBaseWritable(BaseWidget* widget, uint8_t writeable);

/**
 * Marshal BaseVersion into given reply message
 * @param widget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalBaseVersion(BaseWidget* widget, AJ_Message* reply, uint16_t language);

/**
 * Marshal BaseStates into given reply message
 * @param widget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalBaseStates(BaseWidget* widget, AJ_Message* reply, uint16_t language);

/**
 * Marshal Only the BaseOptParam of the given widget into given reply message
 * @param widget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalOnlyBaseOptParam(BaseWidget* widget, AJ_Message* reply, uint16_t language);

/**
 * Marshal BaseOptParam into given reply message
 * @param widget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalBaseOptParam(BaseWidget* widget, AJ_Message* reply, uint16_t language);

/**
 * Marshal All BaseProperties of given widget into given reply message
 * @param widget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalAllBaseProperties(BaseWidget* widget, AJ_Message* reply, uint16_t language);

/**
 * Marshal All the BaseProperties of a given widget into given reply message
 * @param widget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalAllOnlyBaseProperties(BaseWidget* widget, AJ_Message* reply, uint16_t language);

/** @} */
#endif /* BASEWIDGET_H_ */
