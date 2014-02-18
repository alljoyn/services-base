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

#include <iostream>
#include <sstream>
#include <cstdio>
#include <signal.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include <alljoyn/controlpanel/ControlPanelControllee.h>
#include <SrpKeyXListener.h>
#include <CommonSampleUtil.h>
#include <GuidUtil.h>
#include <ControlPanelGenerated.h>
#include <alljoyn/services_common/LogModulesNames.h>

#define SERVICE_PORT 900

using namespace ajn;
using namespace services;
using namespace qcc;

AboutPropertyStoreImpl* propertyStoreImpl = 0;
CommonBusListener* controlpanelBusListener = 0;
BusAttachment* bus = 0;
ControlPanelService* controlPanelService = 0;
ControlPanelControllee* controlPanelControllee = 0;
SrpKeyXListener* srpKeyXListener = 0;

void exitApp(int32_t signum)
{
    std::cout << "Program Finished" << std::endl;

    CommonSampleUtil::aboutServiceDestroy(bus, controlpanelBusListener);
    if (controlPanelService) {
        controlPanelService->shutdownControllee();
    }
    ControlPanelGenerated::Shutdown();
    if (controlPanelControllee) {
        delete controlPanelControllee;
    }
    if (controlpanelBusListener) {
        delete controlpanelBusListener;
    }
    if (propertyStoreImpl) {
        delete (propertyStoreImpl);
    }
    if (controlPanelService) {
        delete controlPanelService;
    }
    if (srpKeyXListener) {
        delete srpKeyXListener;
    }
    if (bus) {
        delete bus;
    }

    std::cout << "Goodbye!" << std::endl;
    exit(signum);
}

int32_t main()
{
    QStatus status;

    // Allow CTRL+C to end application
    signal(SIGINT, exitApp);
    std::cout << "Beginning ControlPanel Application. (Press CTRL+C to end application)" << std::endl;

    // Initialize Service objects
#ifdef QCC_USING_BD
    PasswordManager::SetCredentials("ALLJOYN_PIN_KEYX", "000000");
#endif

    controlpanelBusListener = new CommonBusListener();;
    controlPanelService = ControlPanelService::getInstance();
    QCC_SetDebugLevel(logModules::CONTROLPANEL_MODULE_LOG_NAME, logModules::ALL_LOG_LEVELS);

    srpKeyXListener = new SrpKeyXListener();

    bus = CommonSampleUtil::prepareBusAttachment(srpKeyXListener);
    if (bus == NULL) {
        std::cout << "Could not initialize BusAttachment." << std::endl;
        exitApp(1);
    }

    qcc::String device_id, app_id;
    qcc::String app_name = "testappName", device_name = "testdeviceName";
    GuidUtil::GetInstance()->GetDeviceIdString(&device_id);
    GuidUtil::GetInstance()->GenerateGUID(&app_id);

    propertyStoreImpl = new AboutPropertyStoreImpl();
    status = CommonSampleUtil::fillPropertyStore(propertyStoreImpl, app_id, app_name, device_id, device_name);
    if (status != ER_OK) {
        std::cout << "Could not fill PropertyStore." << std::endl;
        exitApp(1);
    }

    status = CommonSampleUtil::prepareAboutService(bus, propertyStoreImpl,
                                                   controlpanelBusListener, SERVICE_PORT);
    if (status != ER_OK) {
        std::cout << "Could not register bus object." << std::endl;
        exitApp(1);
    }

    status = ControlPanelGenerated::PrepareWidgets(controlPanelControllee);
    if (status != ER_OK) {
        std::cout << "Could not prepare Widgets." << std::endl;
        exitApp(1);
    }

    status = controlPanelService->initControllee(bus, controlPanelControllee);
    if (status != ER_OK) {
        std::cout << "Could not initialize Controllee." << std::endl;
        exitApp(1);
    }

    status = CommonSampleUtil::aboutServiceAnnounce();
    if (status != ER_OK) {
        std::cout << "Could not announce." << std::endl;
        exitApp(1);
    }

    std::cout << "Sent announce, waiting for Contollers" << std::endl;
    while (1) {
        sleep(1);
    }
}
