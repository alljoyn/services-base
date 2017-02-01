/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF), AllJoyn Open Source
 *    Project (AJOSP) Contributors and others.
 *    
 *    SPDX-License-Identifier: Apache-2.0
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *    
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *    
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *    
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
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
    /**
     * ParseResultCode
     */
    enum ParseResultCode {
        PR_OK, PR_EXIT_NO_ERROR, PR_INVALID_OPTION, PR_MISSING_OPTION, PR_INVALID_APPID
    };

    /**
     * OptParser
     * @param argc
     * @param argv
     */
    OptParser(int argc, char** argv);

    /**
     * ParseResult
     * @return
     */
    ParseResultCode ParseResult();

    /**
     * GetFactoryConfigFile
     * @return
     */
    qcc::String const& GetFactoryConfigFile() const;

    /**
     * GetConfigFile
     * @return
     */
    qcc::String const& GetConfigFile() const;

    /**
     * GetAppId
     * @return
     */
    qcc::String const& GetAppId() const;

  private:
    int argc;
    char** argv;

    bool IsAllHex(const char* data);

    void PrintUsage();

    qcc::String factoryConfigFile;
    qcc::String configFile;
    qcc::String appGUID;
};

#endif /* OPTPARSER_H_ */