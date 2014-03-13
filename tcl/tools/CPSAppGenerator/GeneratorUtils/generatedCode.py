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

import sys;

class Generator:

    def __init__(self, scriptDir, path, testFilePath):
        self.scriptDir = scriptDir
        self.path = path
        self.testFilePath = testFilePath
        self.ns = ""
        self.headerIncludes = ""
        self.units = {}

        self.headerFile = ""
        self.sampleFile = ""
        self.generatedFile = ""
        self.testerFile = ""
        self.testerGeneratedFile = ""
        self.testerGeneratedHeader = ""

        self.getValuesCases = ""
        self.setValuesCases = ""
        self.getAllCases = ""
        self.getRootValuesCases = ""
        self.getAllRootCases = ""
        self.actionCases = ""
        self.httpCases = ""
        self.maxNumLang = 0

        self.appObjects = ""
        self.announceObjects = ""
        self.defines = ""
        self.definesIndx = 0
        self.objectPathsDef = ""
        self.objectPathsDecl = ""
        self.widgetsDef = ""
        self.widgetsDecl = ""
        
        self.staticVars = ""
        self.initFunction = ""
        self.identify = ""
        self.identifySignal = ""
        self.identifyRoot = ""

        self.setWidgetPropFunc = ""
        self.executeAction = ""

        self.numTests = 0
        self.initTests = ""
        self.allReplies = ""

    def setControlDeviceData(self, unitId, headerCode) :
        self.unitId = unitId
        self.ObjectPathPrefix = "/ControlPanel/" + self.unitId + "/"
        self.headerIncludes += headerCode + "\n"
        if unitId in self.units : 
            print >> sys.stderr, "\nERROR - an xml with unit name " + unitId + " has already been generated. When generating multiple xmls the unit names must be unique\n"
            sys.exit(3)        
        self.units[unitId] = []

    def setLanguageSets(self, languageSet) :

        self.languageSets = {}
        for langElement in languageSet:
            name = langElement.attr["name"]
            self.languageSets[name] = []
            if isinstance(langElement.language, list) :
                langIndx = 0   
                for language in langElement.language :
                    self.languageSets[name].append(language)
                    self.defines += "#define {0}_{1}_{2} {3}\n".format(self.unitId.upper(), name.upper(), language.replace("-", "_").upper(), langIndx)
                    langIndx += 1
                self.defines += "\n"
                if langIndx > self.maxNumLang :
                    self.maxNumLang = langIndx
            else :
                self.languageSets[name].append(langElement.language)
                self.defines += "#define {0}_{1}_{2} {3}\n\n".format(self.unitId.upper(), name.upper(), langElement.language.replace("-", "_").upper(), 0)
                if 1 > self.maxNumLang :
                    self.maxNumLang = 1

    def addControlPanel(self, rootElement) :
        name = self.unitId + rootElement.name[:1].upper() + rootElement.name[1:]
        capName = self.unitId.upper() + "_" + "ROOT_CONTROLPANEL_" + rootElement.name.upper()
        objectPathVar = "{0}ObjectPath".format(name)
        myObjectPath = self.ObjectPathPrefix + rootElement.name
        self.objectPathsDecl += "extern const char {0}[];\n".format(objectPathVar)
        self.objectPathsDef += "const char {0}[] = \"{1}\";\n".format(objectPathVar, myObjectPath)
        self.appObjects += "    {0}  {1}, ControlPanelInterfaces, AJ_OBJ_FLAG_ANNOUNCED  {2}, \\\n".format("{", objectPathVar, "}")

        self.defines += """#define {0}_GET_VALUE                  AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET)
#define {0}_SET_VALUE                  AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_SET)
#define {0}_GET_ALL_VALUES             AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET_ALL)
#define {0}_VERSION_PROPERTY           AJ_APP_PROPERTY_ID({1} + NUM_PRECEDING_OBJECTS, 1, 0)\n""".format(capName, self.definesIndx)
        self.defines += "\n"
        self.definesIndx += 1	

        self.getRootValuesCases += "case {0}_GET_VALUE: \\\n".format(capName)
        self.getAllRootCases += "case {0}_GET_ALL_VALUES: \\\n".format(capName)
        self.setValuesCases += "case {0}_SET_VALUE: \\\n".format(capName)
        self.identifyRoot += """    case {0}_VERSION_PROPERTY:
    case {0}_GET_ALL_VALUES:
        return TRUE;\n\n""".format(capName)

        if rootElement._has("dismissable") and rootElement.dismissable :
            addNotificationAction(rootElement, 1)

    def addNotificationAction(self, rootElement, fromControlPanel = 0) :
        name = self.unitId + rootElement.name[:1].upper() + rootElement.name[1:]
        capName = self.unitId.upper() + "_" + "NOTIFICATION_ACTION_" + rootElement.name.upper()
        objectPathVar = "{0}ObjectPath".format(name)
        self.appObjects += "    {0}  {1}, NotificationActionInterfaces  {2}, \\\n".format("{", objectPathVar, "}")
       
        if not fromControlPanel :
            myObjectPath = self.ObjectPathPrefix + rootElement.name
            self.objectPathsDecl += "extern const char {0}[];\n".format(objectPathVar)
            self.objectPathsDef += "const char {0}[] = \"{1}\";\n".format(objectPathVar, myObjectPath)

        self.defines += """#define {0}_GET_VALUE                  AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET)
#define {0}_SET_VALUE                  AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_SET)
#define {0}_GET_ALL_VALUES             AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 0, AJ_PROP_GET_ALL)
#define {0}_VERSION_PROPERTY           AJ_APP_PROPERTY_ID({1} + NUM_PRECEDING_OBJECTS, 1, 0)
#define {0}_SIGNAL_DISMISS             AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 1, 1)\n""".format(capName, self.definesIndx)
        self.defines += "\n"
        self.definesIndx += 1	

        self.getRootValuesCases += "case {0}_GET_VALUE: \\\n".format(capName)
        self.getAllRootCases += "case {0}_GET_ALL_VALUES: \\\n".format(capName)
        self.setValuesCases += "case {0}_SET_VALUE: \\\n".format(capName)
        self.identifyRoot += """    case {0}_VERSION_PROPERTY:
    case {0}_GET_ALL_VALUES:
    case {0}_SIGNAL_DISMISS:
        return TRUE;\n\n""".format(capName)


    def initializeFiles(self) :

        genH = open(self.scriptDir + '/PreGenFiles/PreGenControlPanelGenerated.h', 'r')
        self.headerFile = genH.read()
        genH.close()

        genC = open(self.scriptDir + '/PreGenFiles/PreGenControlPanelGenerated.c', 'r')
        self.generatedFile = genC.read()
        genC.close()

    def writeFiles(self) :

        genH = open(self.path + "/ControlPanelGenerated.h", 'w')
        genH.write(self.headerFile)
        genH.close()

        genC = open(self.path + "/ControlPanelGenerated.c", 'w')
        genC.write(self.generatedFile)
        genC.close()

        if self.testFilePath == "" :
            return
        client = open(self.testFilePath + "/ControlPanelClientGenerated.c", 'w')
        client.write(self.testerGeneratedFile)
        client.close()

        client = open(self.testFilePath + "/ControlPanelClientGenerated.h", 'w')
        client.write(self.testerGeneratedHeader)
        client.close()

    def replaceInFiles(self) :

        self.headerFile = self.headerFile.replace("//DEFINES_GO_HERE", self.defines)
        self.headerFile = self.headerFile.replace("//OBJECTPATH_DECL_GO_HERE", self.objectPathsDecl)
        self.headerFile = self.headerFile.replace("//APPOBJECTS_GO_HERE", self.appObjects)
        self.headerFile = self.headerFile.replace("//GETVALUES_GO_HERE", self.getValuesCases)
        self.headerFile = self.headerFile.replace("//SETVALUES_GO_HERE", self.setValuesCases)
        self.headerFile = self.headerFile.replace("//GETALLVALUES_GO_HERE", self.getAllCases)
        self.headerFile = self.headerFile.replace("//ACTION_GO_HERE", self.actionCases)
        self.headerFile = self.headerFile.replace("//GET_URL_GO_HERE", self.httpCases)
        self.headerFile = self.headerFile.replace("//GETVALUES_ROOT_GO_HERE", self.getRootValuesCases)
        self.headerFile = self.headerFile.replace("//GETALLVALUES_ROOT_GO_HERE", self.getAllRootCases)
        self.headerFile = self.headerFile.replace("//MAXNUMLANGUAGES_GO_HERE", str(self.maxNumLang))
        self.headerFile = self.headerFile.replace("//NUM_CPSOBJ_GO_HERE", str(self.definesIndx))
        self.testerGeneratedHeader = self.headerFile ## Catch it after all replaces
        self.headerFile = self.headerFile.replace("//WIDGETS_DECL_GO_HERE", self.widgetsDecl)

        self.generatedFile = self.generatedFile.replace("//OBJECTPATH_DEF_GO_HERE", self.objectPathsDef)
        self.testerGeneratedFile = self.generatedFile ## Catch it before all unnecessary replaces
        self.generatedFile = self.generatedFile.replace("//STATICS_GO_HERE", self.staticVars)
        self.generatedFile = self.generatedFile.replace("//WIDGETS_DEF_GO_HERE", self.widgetsDef)
        self.generatedFile = self.generatedFile.replace("//INCLUDES_GO_HERE", self.headerIncludes)
        self.generatedFile = self.generatedFile.replace("//INITROOT_GO_HERE", self.initFunction)
        self.generatedFile = self.generatedFile.replace("//ADD_MAIN_CASE_HERE", self.identify)
        self.generatedFile = self.generatedFile.replace("//ADD_ROOT_CASE_HERE", self.identifyRoot)
        self.generatedFile = self.generatedFile.replace("//ADD_SIGNAL_CASE_HERE", self.identifySignal)
        self.generatedFile = self.generatedFile.replace("//SET_WIDGET_PROPERTY_GO_HERE", self.setWidgetPropFunc)
        self.generatedFile = self.generatedFile.replace("//EXECUTE_ACTION_GO_HERE", self.executeAction)
        self.generatedFile = self.generatedFile.replace("//INITTESTS_GO_HERE", "")
        
        self.testerGeneratedFile = self.testerGeneratedFile.replace("//INITTESTS_GO_HERE", self.initTests)
        self.testerGeneratedFile = self.testerGeneratedFile.replace("//STATICS_GO_HERE", "")
        self.testerGeneratedFile = self.testerGeneratedFile.replace("//INCLUDES_GO_HERE", "#include \"ControlPanelClientGenerated.h\"\n")
        self.testerGeneratedFile = self.testerGeneratedFile.replace("//INITROOT_GO_HERE", "")
        self.testerGeneratedFile = self.testerGeneratedFile.replace("//ADD_MAIN_CASE_HERE", "")
        self.testerGeneratedFile = self.testerGeneratedFile.replace("//ADD_ROOT_CASE_HERE", "")
        self.testerGeneratedFile = self.testerGeneratedFile.replace("//ADD_SIGNAL_CASE_HERE", "")
        self.testerGeneratedFile = self.testerGeneratedFile.replace("//SET_WIDGET_PROPERTY_GO_HERE", "")
        self.testerGeneratedFile = self.testerGeneratedFile.replace("//EXECUTE_ACTION_GO_HERE", "")
        self.testerGeneratedFile = self.testerGeneratedFile.replace("//WIDGETS_DEF_GO_HERE", "")

        self.testerGeneratedHeader = self.testerGeneratedHeader.replace("//NUM_TESTS_GO_HERE", str(self.numTests))
        self.testerGeneratedHeader = self.testerGeneratedHeader.replace("//ALL_REPLIES_GO_HERE", self.allReplies)
        self.testerGeneratedHeader = self.testerGeneratedHeader.replace("AJ_APP_MESSAGE_ID", "AJ_PRX_MESSAGE_ID")
        self.testerGeneratedHeader = self.testerGeneratedHeader.replace("AJ_APP_PROPERTY_ID", "AJ_PRX_PROPERTY_ID")



