# # 
#    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
#    Source Project Contributors and others.
#    
#    All rights reserved. This program and the accompanying materials are
#    made available under the terms of the Apache License, Version 2.0
#    which accompanies this distribution, and is available at
#    http://www.apache.org/licenses/LICENSE-2.0

#
import xml.etree.ElementTree as xml
import commonWidget as common

class Label (common.Widget):

    def __init__(self, generated, labelElement, parentObjectPath, languageSetName) :
        common.Widget.__init__(self, generated, labelElement, parentObjectPath, languageSetName)
        self.widgetName = "Label"
