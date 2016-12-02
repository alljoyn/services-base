# # 
#    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
#    Source Project Contributors and others.
#    
#    All rights reserved. This program and the accompanying materials are
#    made available under the terms of the Apache License, Version 2.0
#    which accompanies this distribution, and is available at
#    http://www.apache.org/licenses/LICENSE-2.0

#
import sys

class HttpControl:

    def __init__(self, generated, url) :
        self.generated = generated
        self.url = url
        self.name = generated.unitName + "HttpControl"
        
    def generate(self) :
        self.generated.varDecl += """    static ajn::services::HttpControl* {0};\n""".format(self.name)
        self.generated.varDef += """HttpControl* ControlPanelGenerated::{0} = 0;\n""".format(self.name)

        self.generated.initCode += """\n    {0} = new HttpControl("{1}");\n""".format(self.name, self.url)
        self.generated.initCode += """    CHECK({0}->setHttpControl({1}));\n""".format(self.generated.unitObject, self.name)

        self.generated.shutdown += """    if ({0}) {1}\n        delete ({0});\n        {0} = 0;\n    {2}\n""".format(self.name, "{", "}")
