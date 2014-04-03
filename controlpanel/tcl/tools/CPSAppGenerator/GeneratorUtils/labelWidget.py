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
import xml.etree.ElementTree as xml
import commonWidget as common

class Label (common.Widget):

    def __init__(self, generated, labelElement, parentObjectPath, languageSetName) :
        common.Widget.__init__(self, generated, labelElement, parentObjectPath, languageSetName)
        self.widgetName = "LabelWidget"
        self.widgetType = "WIDGET_TYPE_LABEL"
        self.nonSecuredInterfaceName = "LabelPropertyInterfaces"
        self.hintPrefix = "LABEL_HINT_"

    def generateDefines(self, capName) :
        common.Widget.generateDefines(self, capName) 
        self.generated.defines += "#define {0}_LABEL_PROPERTY             AJ_APP_PROPERTY_ID({1} + NUM_PRECEDING_OBJECTS, 1, 4)\n".format(capName, self.generated.definesIndx)

    def generateIdentify(self, capName, language) :
        common.Widget.generateIdentify(self, capName, language)
        self.generated.identify += """    case {0}_LABEL_PROPERTY:
        *widgetType = WIDGET_TYPE_LABEL;
        *propType = PROPERTY_TYPE_LABEL;
        *language = {2};
        return &{1};\n\n""".format(capName, self.name, language)

    def generateMandatoryVariables (self) :
        self.generated.initFunction  += "    initializeLabelWidget(&{0});\n".format(self.name)
        self.setNumLanguages()
        self.setEnabled()
        self.setLabel(1)
        self.generated.initFunction += "\n"

    def generateOptionalVariables (self) :
        self.setBgColor()
        self.setHints() 
    	

