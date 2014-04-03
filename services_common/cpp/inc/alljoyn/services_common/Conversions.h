/******************************************************************************
 * Copyright (c) 2013-2014, AllSeen Alliance. All rights reserved.
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

#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_

#include <qcc/platform.h>
#include <alljoyn/Status.h>
#include <qcc/String.h>

namespace ajn {

class MsgArg;

namespace services {
/**
 * class contains methods for conversion needs
 */
class Conversions {
  public:
    /**
     * Convert MsgArg to array of bytes
     *
     * @param msgArg    An array of MsgArgs to unpack.
     * @param byteArray    pointer to ponter to uint8_t to return.
     * @param len       The length of byte array.
     * @return status ER_BUS_BAD_VALUE_TYPE in case of failure
     */
    static QStatus MsgArgToArrayOfBytes(const MsgArg* msgArg, uint8_t** byteArray, size_t* len);
    /**
     * Convert MsgArg to array of bytes
     *
     * @param msgBin   An array of bytes
     * @param len      The length of byte array.
     * @param byteArrayStr    String to receive.
     *
     */
    static void ArrayOfBytesToString(uint8_t const*const* msgBin, size_t len, qcc::String* byteArrayStr);
    /**
     * Convert hex to char
     *
     * @param hex string
     * @param outBytes
     * @param len - expected len of outbytes
     * @return status
     */
    static QStatus HexStringToBytes(const qcc::String& hex, uint8_t* outBytes, size_t len);

  private:
    Conversions();
    ~Conversions();
    /**
     * Convert hex to char
     *
     * @param c        hex
     * @return
     *    expected char
     *    -1 on error
     */

    static char HexToChar(char c);

};

} //services
} //ajn

#endif /* CONVERSIONS_H_ */
