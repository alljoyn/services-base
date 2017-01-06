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

#ifndef OPTPARSER_H_
#define OPTPARSER_H_

#include <map>
#include <qcc/String.h>

/**
 * Class to parse arguments
 */
class OptParser {
  public:
    static char const AUTH_MECHANISM_NULL[];
    static char const AUTH_MECHANISM_PSK[];
    static char const AUTH_MECHANISM_ECDSA[];
    static char const AUTH_MECHANISM_SPEKE[];

    enum ParseResultCode {
        PR_OK, PR_EXIT_NO_ERROR, PR_INVALID_OPTION, PR_MISSING_OPTION, PR_INVALID_APPID, PR_UNKNOWN_AUTH
    };

    OptParser(int argc, char** argv);

    ParseResultCode ParseResult();

    qcc::String const& GetFactoryConfigFile() const;

    qcc::String const& GetConfigFile() const;

    qcc::String const& GetAppId() const;

    qcc::String const& GetDeviceId() const;

    qcc::String const& GetAuthPassword() const;

    qcc::String const& GetAuthMechanism() const;

  private:
    int argc;
    char** argv;

    bool IsAllHex(const char* data);

    void PrintUsage();

    qcc::String factoryConfigFile;
    qcc::String configFile;
    qcc::String appId;
    qcc::String deviceId;
    qcc::String authPassword;
    qcc::String authMech;
};

#endif /* OPTPARSER_H_ */
