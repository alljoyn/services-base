# Copyright (c) 2013 -2014, AllSeen Alliance. All rights reserved.
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
import dialogWidget as dw

class Action (common.Widget):

    def __init__(self, generated, actionElement, parentObjectPath, languageSetName) :
        common.Widget.__init__(self, generated, actionElement, parentObjectPath, languageSetName)
        self.widgetName = "ActionWidget"
        self.widgetType = "WIDGET_TYPE_ACTION"
        self.nonSecuredInterfaceName = "ActionInterfaces"
        self.securedInterfaceName = "SecuredActionInterfaces"
        self.hintPrefix = "ACTION_WIDGET_HINT_"
        self.executeCases = ""

    def generate(self) :
        common.Widget.generate(self)
        self.generateOnAction()

    def generateDefines(self, capName) :
        common.Widget.generateDefines(self, capName) 
        self.generated.defines += "#define {0}_EXEC                       AJ_APP_MESSAGE_ID({1} + NUM_PRECEDING_OBJECTS, 1, 4)\n".format(capName, self.generated.definesIndx)
        self.generated.actionCases += "case {0}_EXEC: \\\n".format(capName)
        self.executeCases += "    case {0}_EXEC:\n".format(capName) 

    def generateTests(self, capName) : 
        common.Widget.generateTests(self, capName) 
        self.generated.initTests += """    testsToRun[{1}].msgId = {0}_EXEC;
    testsToRun[{1}].numParams = 0;\n""".format(capName, self.generated.numTests, self.generated.definesIndx - 1)
        self.generated.numTests = self.generated.numTests + 1
        self.generated.allReplies += "case AJ_REPLY_ID({0}_EXEC): \\\n".format(capName)

    def generateOnAction (self) :
        onAction = self.element.onAction
        if hasattr(onAction, "executeCode") : 
            self.generated.executeAction += self.executeCases + "        {0}\n".format("{")    
            self.generated.executeAction += "            {0}\n        {1}\n        break;\n\n".format(onAction.executeCode, "}")
        elif hasattr(onAction, "dialog") : 
            dialogElem = onAction.dialog
            dialog = dw.Dialog(self.generated, dialogElem, (self.parentObjectPath + self.objectPathSuffix), self.languageSetName)
            dialog.generate()




