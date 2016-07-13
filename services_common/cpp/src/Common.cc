/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
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

#include <qcc/StringUtil.h>

namespace ajn {
namespace services {
namespace common {

/*
 * Checks if a valid AllJoyn Object path has been specified in str.
 * Please note that this function is same as ajn::BusUtil::IsLegalObjectPath()
 * but since this is not exposed to external applications, this function is
 * being added here
 */
bool
IsLegalObjectPath(const char* str)
{
    if (!str) {
        return false;
    }
    /* Must begin with slash */
    char c = *str++;
    if (c != '/') {
        return false;
    }
    while ((c = *str++) != 0) {
        if ((!(qcc::IsAlphaNumeric(c))) && (c != '_')) {
            if ((c != '/') || (*str == '/') || (*str == 0)) {
                return false;
            }
        }
    }
    return true;
}

}         //namespace common
}     //namespace services
} //namespace ajn
