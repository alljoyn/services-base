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

#include <alljoyn/notification/common.h>

/**
 * Static non consts.
 */
const char InterfaceName[]            = "org.alljoyn.Notification";
const char SignalName[]               = "!notify >q >i >q >s >s >ay >s >a{iv} >a{ss} >a(ss)";
const char Version[]                  = "@Version>q";

const uint16_t version = 1;

/**
 * The interface name followed by the method signatures.
 *
 * See also .\inc\aj_introspect.h
 */
const char* NotificationInterface[] = {
    InterfaceName,                           /*The first entry is the interface name.*/
    SignalName,                              /*Signal at index 0 - See above for signature*/
    Version,
    NULL
};

/**
 * A NULL terminated collection of all interfaces.
 */
const AJ_InterfaceDescription NotificationInterfaces[] = {
    AJ_PropertiesIface,
    NotificationInterface,
    NULL
};

