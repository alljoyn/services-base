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
import os
import subprocess
import xml.etree.ElementTree as xml

scriptDir = os.path.dirname(os.path.realpath(__file__))
sys.path.append( scriptDir + "/GeneratorUtils" )

import xml2objects 
import generatedCode as gen
import containerWidget as cw
import dialogWidget as dw
import commonWidget as common
import httpControl as http
import cpvalidate

## ERROR CODES ##
## 1 : missing command line arguments
## 2 : Did not pass xsd validation
## 3 : Did not pass logical validation

### Start by validating the input and the xml ###

if len(sys.argv) < 2 :
    print >> sys.stderr, "ERROR - Please provide the xml file as input"
    sys.exit(1)

xmlfile = sys.argv[1]
cpFile = scriptDir + "/cp.xsd"
subprocArgs = "xmllint --noout --schema {0} {1}".format(cpFile, xmlfile)
rc = subprocess.call(subprocArgs, shell=True)
if rc != 0 :
    print >> sys.stderr, "\nERROR - xml validation did not pass"
    sys.exit(2)

path = "."
if len(sys.argv) >= 3 :
    path = sys.argv[2]

testFilePath = ""
if len(sys.argv) >= 4 :
    testFilePath = sys.argv[3]
  
### Initialize the generated structure ###
o = xml2objects.ObjectBuilder(xmlfile)
	
if not cpvalidate.validate_all(o.root):
    sys.exit(3)

generated = gen.Generator(scriptDir, path, testFilePath)
generated.initializeFiles()
generated.setControlDeviceData(o.root.controlPanelDevice.name, o.root.controlPanelDevice.headerCode)
generated.setLanguageSets(o.root.controlPanelDevice.languageSet)

### Get and process HttpControlElements
if hasattr(o.root.controlPanelDevice, "url") :
    httpControl = http.HttpControl(generated, o.root.controlPanelDevice.url)
    httpControl.generate()

### Get and process all ControlPanels
if hasattr(o.root.controlPanelDevice, "controlPanels") :
   for cp in o.root.controlPanelDevice.controlPanels.controlPanel :
        generated.addControlPanel(cp.rootContainer)
        container = cw.Container(generated, cp.rootContainer, generated.ObjectPathPrefix + cp.rootContainer.name + "/REPLACE_LANG", cp.attr["languageSet"], 1)
        container.generate()

### Get and process all NotificationAction 
if hasattr(o.root.controlPanelDevice, "notificationActions") :
    if hasattr(o.root.controlPanelDevice.notificationActions, "dialog") :
        for notDialog in o.root.controlPanelDevice.notificationActions.dialog :
            generated.addNotificationAction(notDialog)
            dialog = dw.Dialog(generated, notDialog, generated.ObjectPathPrefix + notDialog.name + "/REPLACE_LANG", notDialog.attr["languageSet"], 1)
            dialog.generate()
    if hasattr(o.root.controlPanelDevice.notificationActions, "container") : 
        for notContainer in o.root.controlPanelDevice.notificationActions.container :
            generated.addNotificationAction(notContainer)
            container = cw.Container(generated, notContainer, generated.ObjectPathPrefix + notContainer.name + "/REPLACE_LANG", notContainer.attr["languageSet"], 1)
            container.generate()

### Finish up merging all the different components ###
generated.replaceInFiles()
generated.writeFiles()
sys.exit(0)

