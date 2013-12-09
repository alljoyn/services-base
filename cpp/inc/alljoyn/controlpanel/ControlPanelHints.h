/******************************************************************************
 * Copyright (c) 2013, AllSeen Alliance. All rights reserved.
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

#ifndef CONTROLPANELHINTS_H_
#define CONTROLPANELHINTS_H_

namespace ajn {
namespace services {

/**
 * Hints for Containers Widgets
 * determining the layout
 */
enum LAYOUT_HINTS {
    VERTICAL_LINEAR = 1, //!< VERTICAL_LINEAR
    HORIZONTAL_LINEAR = 2 //!< HORIZONTAL_LINEAR
};

/**
 * Hints for Dialog Widgets
 */
enum DIALOG_HINTS {
    ALERTDIALOG = 1 //!< ALERTDIALOG
};

/**
 * Hints for Property Widgets
 */
enum PROPERTY_HINTS {
    SWITCH = 1,      //!< SWITCH
    CHECKBOX = 2,    //!< CHECKBOX
    SPINNER = 3,     //!< SPINNER
    RADIOBUTTON = 4, //!< RADIOBUTTON
    SLIDER = 5,      //!< SLIDER
    TIMEPICKER = 6,  //!< TIMEPICKER
    DATEPICKER = 7,  //!< DATEPICKER
    NUMBERPICKER = 8, //!< NUMBERPICKER
    KEYPAD = 9,      //!< KEYPAD
    ROTARYKNOB = 10, //!< ROTARYKNOB
    TEXTVIEW = 11,   //!< TEXTVIEW
    NUMERICVIEW = 12, //!< NUMERICVIEW
    EDITTEXT = 13    //!< EDITTEXT
};

/**
 * Hints for Label Widgets
 */
enum LABEL_HINTS {
    TEXTLABEL = 1 //!< TEXTLABEL
};

/**
 * Hints for ListProperty Widgets
 */
enum LIST_PROPERTY_HINTS {
    DYNAMICSPINNER = 1 //!< DYNAMICSPINNER
};

/**
 * Hints for Action Widgets
 */
enum ACTION_HINTS {
    ACTIONBUTTON = 1 //!< ACTIONBUTTON
};

} //namespace services
} //namespace ajn

#endif /* CONTROLPANELHINTS_H_ */


