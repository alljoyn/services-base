# Copyright (c) 2013, AllSeen Alliance. All rights reserved.
#
#    Permission to use, copy, modify, and/or distribute this software for any
#    purpose with or without fee is hereby granted, provided that the above
#    copyright notice and this permission notice appear in all copies.
#
#    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
#    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
#    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
#    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
#    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
#    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
#    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
import sys

class HttpControl:

    def __init__(self, generated, url) :
        self.generated = generated
        self.url = url
        self.name = self.generated.unitId + "HttpControl"
        self.capName = self.generated.unitId.upper() + "_HTTPCONTROL"        
                
        self.httpCasesString = """#define {0}_GET_VALUE             AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET)
#define {0}_SET_VALUE             AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_SET)
#define {0}_GET_ALL_VALUES        AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET_ALL)
#define {0}_VERSION_PROPERTY      AJ_APP_PROPERTY_ID({1} + NUM_PRECEDING_OBJECTS, 1, 0)
#define {0}_GET_ROOT_URL          AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 1, 1)\n\n"""
         
        self.httpIdentifyString = """    case {0}_GET_ROOT_URL:
        *widgetType = WIDGET_TYPE_HTTP;
        *propType = PROPERTY_TYPE_URL;
        return &{1};\n\n"""

        self.httpRootIdentifyString = """    case {0}_GET_ALL_VALUES:
    case {0}_VERSION_PROPERTY:
        return TRUE;\n\n"""

    def generate(self) :
        objectPathVar = "{0}ObjectPath".format(self.name)

        self.generated.appObjects += "    {0}  {1}, HttpControlInterfaces  {2}, \\\n".format("{", objectPathVar, "}")
        self.generated.announceObjects += "    {0}  {1}, HttpControlInterfaces  {2}, \\\n".format("{", objectPathVar, "}")
        
        self.generated.defines += self.httpCasesString.format(self.capName, self.generated.definesIndx)  

        self.generated.definesIndx += 1	
        self.generated.getRootValuesCases += "case {0}_GET_VALUE:\\\n".format(self.capName)
        self.generated.setValuesCases += "case {0}_SET_VALUE:\\\n".format(self.capName)
        self.generated.getAllRootCases += "case {0}_GET_ALL_VALUES:\\\n".format(self.capName)
        self.generated.httpCases += "case {0}_GET_ROOT_URL:\\\n".format(self.capName)

        self.generated.identifyRoot += self.httpRootIdentifyString.format(self.capName)
        self.generated.identify += self.httpIdentifyString.format(self.capName, self.name)

        self.generated.objectPathsDecl += "extern const char {0}[];\n".format(objectPathVar)
        self.generated.objectPathsDef += "const char {0}[] = \"{1}\";\n".format(objectPathVar, self.generated.ObjectPathPrefix + "HTTPControl")

        self.generated.staticVars += "static HttpControl {0};\n".format(self.name)
        self.generated.initFunction += "    initializeHttpControl(&{0});\n".format(self.name)

        if hasattr(self.url, "attr") and "code" in self.url.attr and self.url.attr["code"] == "true" : 
           self.generated.initFunction += "    {0}.getUrl = {1};\n".format(self.name, self.url)
        else :
           self.generated.staticVars += "static const char* {0}Url = \"{1}\";\n".format(self.name, self.url)    
           self.generated.initFunction += "    {0}.url = {0}Url;\n\n".format(self.name)

        self.generated.initTests += "    testsToRun[{1}].msgId = {0}_GET_ROOT_URL;\n    testsToRun[{1}].numParams = 0;\n""".format(self.capName, self.generated.numTests)
        self.generated.numTests = self.generated.numTests + 1

        self.generated.allReplies += "case AJ_REPLY_ID({0}_GET_ROOT_URL): \\\n".format(self.capName)

