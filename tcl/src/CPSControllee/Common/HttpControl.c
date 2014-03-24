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

#include <alljoyn/controlpanel/Common/HttpControl.h>
#include <alljoyn/controlpanel/Definitions.h>
#include <alljoyn/controlpanel/Common/ControlMarshalUtil.h>

void initializeHttpControl(HttpControl* httpControl)
{
    httpControl->url = NULL;
    httpControl->getUrl = NULL;
}

AJ_Status marshalHttpControlUrl(HttpControl* httpControl, AJ_Message* reply, uint16_t language)
{
    if (httpControl->getUrl == 0 && httpControl->url == 0) {
        return AJ_ERR_UNEXPECTED;
    }
    return AJ_MarshalArgs(reply, PROPERTY_TYPE_URL_SIG, httpControl->getUrl ? httpControl->getUrl() : httpControl->url);
}
