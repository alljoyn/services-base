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

#ifndef PROPERTYWIDGET_H_
#define PROPERTYWIDGET_H_

/** @defgroup PropertyWidget Property Widget
 * details Property Widget Struct and Functions that assist in Un/Marshaling
 * Property from/to a message.
 *  @{
 */

#include <alljoyn/controlpanel/Common/BaseWidget.h>
#include <alljoyn/controlpanel/Common/ConstraintList.h>
#include <alljoyn/controlpanel/Common/ConstraintRange.h>

/**
 * PropertyType enumeration
 */
typedef enum {
    SINGLE_VALUE_PROPERTY = 0, //!< SINGLE_VALUE_PROPERTY - Property has a simple scalar value
    DATE_VALUE_PROPERTY = 1,   //!< DATE_VALUE_PROPERTY   - Property has a Date value (DatePropertyValue)
    TIME_VALUE_PROPERTY = 2,   //!< TIME_VALUE_PROPERTY   - Property has a Time value (TimePropertyValue)
} PropertyType;

struct PropertyWidget;

/////////////////////////* PropertyWidget OptParams *//////////////////////////////////////

/**
 * PropertyOptParams structure
 */
typedef struct {
    const char* const* unitOfMeasure;                                             //!< The unit of measurement e.g. "cm"
    const char* (*getUnitOfMeasure)(struct PropertyWidget* thisWidget, uint16_t); //!< Getter for the unit of measurement

    /**
     * Optional function that provices an alternative way to lookup constraint information. If this function pointer is non-NULL this
     * function is called to obtain contraint information and the contstraintList is ignored
     *
     * @param thisWidget Identifies the property widget that has constraints.
     * @param index      The index of the constraint value requested
     * @param val        A pointer to return the constraint value
     * @param language   Specifies the language index for the returned text label.
     *
     * @return Returns the text label for the requested constraint.
     */
    const char* (*getConstraint)(struct PropertyWidget* thisWidget, uint16_t index, const void** val, uint16_t language);
    ConstraintList* constraintList;                                               //!< The list of Constraints defined for the Property's value
    uint16_t numConstraints;                                                      //!< The number of Constraints in the list

    ConstraintRange constraintRange;                                              //!< ConstraintRange defined for the Property's value
    uint8_t constraintRangeDefined;                                               //!< Flag indicating the presence of Range definition for the Constraint

} PropertyOptParams;

/**
 * Initialize PropertyOptParam
 */
void initializePropertyOptParam(PropertyOptParams* widget);

/////////////////////////*     PropertyWidget     *//////////////////////////////////////

/**
 * PropertyWidget structure
 */
typedef struct PropertyWidget {
    BaseWidget base;                                      //!< Internal BaseWidget

    PropertyType propertyType;                            //!< The PropertyType

    const char* signature;                                //!< The Property's signature
    void* (*getValue)(struct PropertyWidget* thisWidget); //!< Getter for the Property's value

    PropertyOptParams optParams;                          //!< Property optional parameters
} PropertyWidget;

/**
 * Initialize PropertyWidget
 * @param widget - assumed to be a PropertyWidget
 */
void initializePropertyWidget(PropertyWidget* widget);

/**
 * Marshal PropertyValue into given reply message
 * @param widget - assumed to be a PropertyWidget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalPropertyValue(PropertyWidget* widget, AJ_Message* reply, uint16_t language);

/**
 * Unmarshal PropertyValue from given message
 * @param widget - assumed to be a PropertyWidget
 * @param message
 * @param newValue
 * @param lockerId
 * @return aj_status
 */
AJ_Status unmarshalPropertyValue(PropertyWidget* widget, AJ_Message* message, void* newValue, const char* lockerId);

/**
 * Marshal PropertyOptParam into given reply message
 * @param widget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalPropertyOptParam(BaseWidget* widget, AJ_Message* reply, uint16_t language);

/**
 * Marshal All Property Properties from given widget into given reply message
 * @param widget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalAllPropertyProperties(BaseWidget* widget, AJ_Message* reply, uint16_t language);

/** @} */
#endif /* PROPERTYWIDGET_H_ */
