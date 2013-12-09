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

import sys
import subprocess

def as_list(s):
	if isinstance(s, list): return s
	return [s]

class Generator:

    def __init__(self, scriptDir, path):
        self.scriptDir = scriptDir
        self.path = path
        self.languageSets = {}
        self.languageSetsCode = ""
        self.headerIncludes = ""
        self.initCode = ""
        self.varDecl = ""
        self.varDef = ""
        self.shutdown = ""

    def setControlDeviceData(self, unitName, headerCode) :
        self.unitName = unitName
        self.ObjectPathPrefix = "/ControlPanel/" + self.unitName + "/"
        self.srcIncludes = headerCode

    def setLanguageSets(self, languageSet) :
        for langElement in languageSet:
            name = langElement.attr["name"]
            self.languageSets[name] = []
            self.languageSetsCode += """    LanguageSet {0}("{0}");\n""".format(name)
            languages = as_list(langElement.language)
            for language in languages :
                self.languageSets[name].append(language)
                self.languageSetsCode += """    {0}.addLanguage("{1}");\n""".format(name, language.replace("-", "_"))
            self.languageSetsCode += """    LanguageSets::add({0}.getLanguageSetName(), {0});\n\n""".format(name)    
    
    def addControlPanel(self, rootElement, languageSet) :
        name = rootElement.name + "ControlPanel"
        self.varDecl += """    static ajn::services::ControlPanel* {0};\n\n""".format(name)
        self.varDef += """ControlPanel* ControlPanelGenerated::{0} = 0;\n""".format(name)

        self.initCode += """\n    {0} = ControlPanel::createControlPanel(LanguageSets::get("{1}"));\n""".format(name, languageSet)
        self.initCode += """    if (!{0})\n        return ER_FAIL;\n""".format(name)
        self.initCode += """    CHECK(controlPanelControllee->addControlPanel({0}));\n""".format(name)

        self.shutdown += """    if ({0}) {1}\n        delete ({0});\n        {0} = 0;\n    {2}\n""".format(name, "{", "}")

    def addNotificationAction(self, rootElement, languageSet) :
        name = rootElement.name + "NotificationAction"
        self.varDecl += """    static ajn::services::NotificationAction* {0};\n\n""".format(name)
        self.varDef += """NotificationAction* ControlPanelGenerated::{0} = 0;\n""".format(name)

        self.initCode += """\n    {0} = NotificationAction::createNotificationAction(LanguageSets::get("{1}"));\n""".format(name, languageSet)
        self.initCode += """    if (!{0})\n        return ER_FAIL;\n""".format(name)
        self.initCode += """    CHECK(controlPanelControllee->addNotificationAction({0}));\n""".format(name)

        self.shutdown += """    if ({0}) {1}\n        delete ({0});\n        {0} = 0;\n    {2}\n""".format(name, "{", "}")

    def initializeFiles(self) :

        inputFile = open(self.scriptDir + '/PreGenFiles/PreGenControlPanelGenerated.h', 'r')
        self.genHeaderFile = inputFile.read()
        inputFile.close()

        inputFile = open(self.scriptDir + '/PreGenFiles/PreGenControlPanelGenerated.cc', 'r')
        self.genSrcFile = inputFile.read()
        inputFile.close()

        inputFile = open(self.scriptDir + '/PreGenFiles/PreGenActionImpl.h', 'r')
        self.actionHeaderFile = inputFile.read()
        inputFile.close()

        inputFile = open(self.scriptDir + '/PreGenFiles/PreGenActionImpl.cc', 'r')
        self.actionSrcFile = inputFile.read()
        inputFile.close()

        inputFile = open(self.scriptDir + '/PreGenFiles/PreGenDialogImpl.h', 'r')
        self.dialogHeaderFile = inputFile.read()
        inputFile.close()

        inputFile = open(self.scriptDir + '/PreGenFiles/PreGenDialogImpl.cc', 'r')
        self.dialogSrcFile = inputFile.read()
        inputFile.close()

        inputFile = open(self.scriptDir + '/PreGenFiles/PreGenPropertyImpl.h', 'r')
        self.propertyHeaderFile = inputFile.read()
        inputFile.close()

        inputFile = open(self.scriptDir + '/PreGenFiles/PreGenPropertyImpl.cc', 'r')
        self.propertySrcFile = inputFile.read()
        inputFile.close()


    def writeFiles(self) :

        genH = open(self.path + "ControlPanelGenerated.h", 'w')
        genH.write(self.genHeaderFile)
        genH.close()

        genC = open(self.path + "/ControlPanelGenerated.cc", 'w')
        genC.write(self.genSrcFile)
        genC.close()


    def replaceInFiles(self) :

        self.genHeaderFile = self.genHeaderFile.replace("//VARIABLE_DECLARATIONS_GO_HERE", self.varDecl)
        self.genHeaderFile = self.genHeaderFile.replace("//NEW_WIDGET_CLASSES_HERE", self.headerIncludes)

        self.genSrcFile = self.genSrcFile.replace("//UNITNAME_GO_HERE", self.unitName)
        self.genSrcFile = self.genSrcFile.replace("//INCLUDES_GO_HERE", self.srcIncludes)
        self.genSrcFile = self.genSrcFile.replace("//STATIC_DECLARATION_HERE", self.varDef)
        self.genSrcFile = self.genSrcFile.replace("//LANGUAGESET_CODE_HERE", self.languageSetsCode)
        self.genSrcFile = self.genSrcFile.replace("//INITCODE_GO_HERE", self.initCode)
        self.genSrcFile = self.genSrcFile.replace("//SHUTDOWN_GO_HERE", self.shutdown)

    def confirmGenerate(self) :
        confirm = self.confirm()
	if confirm :
            subprocArgs = "rm -f {0}/*.cc {0}/*.h".format(self.path)
            rc = subprocess.call(subprocArgs, shell=True)
            if rc != 0 :
                print >> sys.stderr, "\nERROR - Could not delete the current generated files"
                sys.exit(4)
        else :
            print "\nStopping the generating process"
            sys.exit(0)   
   
    def confirm(self):
        default = False
        prompt = "\nGenerating this xml will cause previously generated files to be deleted. Do you wish to continue? (y/n) (Default is n): "
        while True:
            response = raw_input(prompt)
            if not response:
                return default
            if response not in ['y', 'Y', 'n', 'N']:
                print 'Please enter y or n.'
                continue
            if response.upper() == 'Y':
                return True
            if response.upper() == 'N':
                return False

