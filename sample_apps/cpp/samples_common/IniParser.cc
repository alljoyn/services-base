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

#include "IniParser.h"
#include <iostream>
#include <string>
#include <fstream>

IniParser::IniParser()
{
}

IniParser::~IniParser()
{
}

qcc::String IniParser::trim(const qcc::String& str,
                            const qcc::String& whitespace)
{
    const auto strBegin = str.find_first_not_of(whitespace.c_str());
    if (strBegin == std::string::npos) {
        return "";  // no content

    }
    const auto strEnd = str.find_last_not_of(whitespace.c_str());
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

bool IniParser::ParseFile(std::string const& fileName, std::map<qcc::String, qcc::String>& data)
{
    std::ifstream iniFile(fileName.c_str(), std::ifstream::in);
    if (!iniFile.is_open()) {
        return false;
    }

    char tmp[256];
    int lineNum = 0;
    while (iniFile.getline(tmp, 256)) {

        qcc::String line = tmp;
        lineNum++;
        line = trim(line); // remove leading and trailing whitespaces

        if (!line.length()) {
            continue;      // empty line skip

        }
        if (line[0] == '#') {
            continue;      // commented line skip

        }
        std::size_t found = line.find('=');
        if (found == std::string::npos) {
            std::cout << "Problem reading line: " << lineNum << std::endl;
            continue;
        }

        qcc::String name = line.substr(0, found);
        name = trim(name); // remove leading and trailing whitespaces

        qcc::String value = line.substr(found + 1);
        value = trim(value);

        data[name] = value;
    }

    iniFile.close();
    return true;
}

bool IniParser::UpdateFile(std::string const& fileName, std::map<qcc::String, qcc::String> const& data)
{
    std::ifstream iniFile(fileName.c_str(), std::ifstream::in);
    if (!iniFile.is_open()) {
        return false;
    }

    qcc::String content = "";
    char tmp[256];
    int lineNum = 0;
    while (iniFile.getline(tmp, 256)) {

        qcc::String origline = tmp;
        lineNum++;
        qcc::String line = trim(origline); // remove leading and trailing whitespaces

        if (!line.length()) {
            content.append(origline + "\n");
            continue;      // empty line skip
        }

        if (line[0] == '#') {
            content.append(origline + "\n");
            continue;      // commented line skip
        }

        std::size_t found = line.find('=');
        if (found == std::string::npos) {
            std::cout << "Problem reading line: " << lineNum << std::endl;
            content.append(origline + "\n");
            continue;
        }

        qcc::String name = line.substr(0, found);
        name = trim(name); // remove leading and trailing whitespaces

        std::map<qcc::String, qcc::String>::const_iterator iter = data.find(name);
        if (iter == data.end()) {
            content.append(origline + "\n");
            continue;
        }

        qcc::String newline = name + " = " + iter->second;
        content.append(newline + "\n");
    }

    iniFile.close();

    std::ofstream iniFileWrite(fileName.c_str(), std::ofstream::out | std::ofstream::trunc);
    iniFileWrite.write(content.c_str(), content.length());
    iniFileWrite.close();

    return true;
}
