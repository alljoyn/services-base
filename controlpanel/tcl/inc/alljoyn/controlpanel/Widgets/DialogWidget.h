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

#ifndef DIALOGWIDGET_H_
#define DIALOGWIDGET_H_

/** @defgroup DialogWidget Dialog Widget
 * details Dialog Widget Struct and Functions that assist in Un/Marshaling
 * Dialog Widget from/to a message.
 *  @{
 */

#include <alljoyn/controlpanel/Common/BaseWidget.h>

/////////////////////////* DialogWidget OptParams *//////////////////////////////////////

struct DialogWidget;

/**
 * DialogOptParams structure
 */
typedef struct DialogOptParams {
    const char* const* labelAction1;                                           //!< The first action's label text
    const char* (*getLabelAction1)(struct DialogWidget* thisWidget, uint16_t); //!< Getter for the first action's label text
    const char* const* labelAction2;                                           //!< The second action's label text
    const char* (*getLabelAction2)(struct DialogWidget* thisWidget, uint16_t); //!< Getter for the second action's label text
    const char* const* labelAction3;                                           //!< The third action's label text
    const char* (*getLabelAction3)(struct DialogWidget* thisWidget, uint16_t); //!< Getter for the third action's label text

} DialogOptParams;

/**
 * Initialize DialogOptParam
 * @param optParam
 */
void initializeDialogOptParam(DialogOptParams* optParam);

/////////////////////////*     DialogWidget     *//////////////////////////////////////

/**
 * DialogWidget structure
 */
typedef struct DialogWidget {
    BaseWidget base;                                                      //!< Internal BaseWidget

    const char* const* message;                                           //!< The Dialog's message text
    const char* (*getMessage)(struct DialogWidget* thisWidget, uint16_t); //!< Getter to the Dialog's message text
    uint16_t numActions;                                                  //!< The number of Actions

    DialogOptParams optParams;                                            //!< Dialog optional parameters

} DialogWidget;

/**
 * Initialize DialogWidget
 * @param widget - assumed to be a DialogWidget
 */
void initializeDialogWidget(DialogWidget* widget);

/**
 * Marshal DialogMessage of given widget into given reply message
 * @param widget - assumed to be a DialogWidget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalDialogMessage(DialogWidget* widget, AJ_Message* reply, uint16_t language);

/**
 * Marshal DialogNumActions of given widget into given reply message
 * @param widget - assumed to be a DialogWidget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalDialogNumActions(DialogWidget* widget, AJ_Message* reply, uint16_t language);

/**
 * Marshal DialogOptParam of given widget into given reply message
 * @param widget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalDialogOptParam(BaseWidget* widget, AJ_Message* reply, uint16_t language);

/**
 * Marshal All DialogProperties of given widget into given reply message
 * @param widget
 * @param reply
 * @param language
 * @return aj_status
 */
AJ_Status marshalAllDialogProperties(BaseWidget* widget, AJ_Message* reply, uint16_t language);

/** @} */
#endif /* DIALOGWIDGET_H_ */

