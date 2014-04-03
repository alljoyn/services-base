# Copyright (c) 2013 - 2014, AllSeen Alliance. All rights reserved.
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
import xml.etree.ElementTree as xml
import commonWidget as common

class Property (common.Widget):

    def __init__(self, generated, propertyElement, parentObjectPath, languageSetName) :
        common.Widget.__init__(self, generated, propertyElement, parentObjectPath, languageSetName)
        self.widgetName = "PropertyWidget"
        self.widgetType = "WIDGET_TYPE_PROPERTY"
        self.nonSecuredInterfaceName = "PropertyInterfaces"
        self.securedInterfaceName = "SecuredPropertyInterfaces"
        self.hintPrefix = "PROPERTY_WIDGET_HINT_"
        self.propertyCases = ""
        self.getSignatureAndVarType()
        self.signalsString = ""
        self.signalsIndx = 0

    def generateDefines(self, capName) :
        common.Widget.generateDefines(self, capName) 
        self.generated.defines += "#define {0}_VALUE_PROPERTY             AJ_APP_PROPERTY_ID({1} + NUM_PRECEDING_OBJECTS, 1, 4)\n".format(capName, self.generated.definesIndx)
        self.generated.defines += "#define {0}_SIGNAL_VALUE_CHANGED       AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 1, 5)\n".format(capName, self.generated.definesIndx)
        self.propertyCases += "    case {0}_VALUE_PROPERTY:\n".format(capName) 
        self.signalsString += "            ((SetValueContext*)context)->signals[{0}] = {1}_SIGNAL_VALUE_CHANGED;\n".format(self.signalsIndx, capName)
        self.signalsIndx += 1

    def generateIdentify(self, capName, language) :
        common.Widget.generateIdentify(self, capName, language)
        self.generated.identify += """    case {0}_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = {2};
        return &{1};\n\n""".format(capName, self.name, language)

        self.generated.identifySignal += """    case {0}_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &{1};\n\n""".format(capName, self.name) 

    def generateMandatoryVariables (self) :
        self.generated.initFunction  += "    initializePropertyWidget(&{0});\n".format(self.name)
        self.setNumLanguages()
        self.setEnabled()
        self.setWritable()
        self.setPropertyVariables()
        self.generated.initFunction += "\n"

    def setPropertyVariables (self) :
        if len(self.signature) > 0 :
            self.generated.staticVars += "static const char* const {0}Signature = \"{1}\";\n".format(self.name, self.signature)  
            self.generated.initFunction += "\n"
            self.generated.initFunction += "    {0}.signature = {0}Signature;\n".format(self.name)
        self.generated.initFunction += "    {0}.propertyType = {1};\n".format(self.name, self.propType)
        self.generated.initFunction += "    {0}.getValue = &{1};\n".format(self.name, self.element.getCode)
        ### setProperty
        setCode = self.element.setCode
        if self.propType == "SINGLE_VALUE_PROPERTY" :
            setCode = setCode.replace("%s", "newValue")
        else :
            setCode = setCode.replace("%s", "&newValue")
        self.generated.setWidgetPropFunc += self.propertyCases + "        {0}\n".format("{")    
        self.generated.setWidgetPropFunc += "            {0} newValue;\n".format(self.varType)
        self.generated.setWidgetPropFunc += "            status = unmarshalPropertyValue(&{0}, replyMsg, &newValue, ((SetValueContext*)context)->sender);\n".format(self.name)
        self.generated.setWidgetPropFunc += "            if (status != AJ_OK) {0}\n".format("{")
        self.generated.setWidgetPropFunc += "                return status;\n"
        self.generated.setWidgetPropFunc += "            }\n"
        self.generated.setWidgetPropFunc += "            {0};\n".format(setCode)
        self.generated.setWidgetPropFunc += "            ((SetValueContext*)context)->numSignals = {0};\n".format(self.signalsIndx)
        self.generated.setWidgetPropFunc += self.signalsString + "        }\n        break;\n\n"

    def generateOptionalVariables (self) :
        self.setBgColor()
        self.setLabel()
        self.setHints() 
        self.setUnitMeasure() 
        self.setConstraintList() 
        self.setConstraintRange() 


    def getSignatureAndVarType(self) :
        self.signature = ""
        self.varType = ""
        self.propType = ""
    
        propertyType = self.element._name
        if propertyType == "stringProperty" :
            self.signature = "s"
            self.varType = "const char*"
            self.propType = "SINGLE_VALUE_PROPERTY"
        elif propertyType == "recordName" :
            self.signature = "s"
            self.varType = "const char*"
            self.propType = "SINGLE_VALUE_PROPERTY"
        elif propertyType == "booleanProperty" :
            self.signature = "b"
            self.varType = "uint32_t"
            self.propType = "SINGLE_VALUE_PROPERTY"
        elif propertyType == "dateProperty" :
            self.varType = "DatePropertyValue"
            self.propType = "DATE_VALUE_PROPERTY"
        elif propertyType == "timeProperty" :
            self.varType = "TimePropertyValue"
            self.propType = "TIME_VALUE_PROPERTY"
        elif propertyType == "scalarProperty" :
            self.propType = "SINGLE_VALUE_PROPERTY"
            dataType = self.element.attr["dataType"]
       	    if dataType == "INT16":
                self.signature = "n"
                self.varType = "int16_t"
            elif dataType == "UINT16":
                self.signature = "q"
                self.varType = "uint16_t"
            elif dataType == "INT32":
                self.signature = "i"
                self.varType = "int32_t"
	    elif dataType == "UINT32":
                self.signature = "u"
                self.varType = "uint32_t"
            elif dataType == "INT64":
                self.signature = "x"
                self.varType = "int64_t"
            elif dataType == "UINT64":
                self.signature = "t"
                self.varType = "uint64_t"
	    elif dataType == "DOUBLE":
                self.signature = "d"
                self.varType = "double"
            else :
                print >> sys.stderr, "ERROR - dataType of property Unknown. Exiting"
                sys.exit(3)
        else :
            print >> sys.stderr, "ERROR - Element Type of property Unknown. Exiting"
            sys.exit(3)






