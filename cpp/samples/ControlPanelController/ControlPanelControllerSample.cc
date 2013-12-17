/******************************************************************************
 * Copyright (c) 2013, AllSeen Alliance. All rights reserved.
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
#include <alljoyn/about/AnnouncementRegistrar.h>
#include <alljoyn/notification/NotificationService.h>
#include <alljoyn/controlpanel/ControlPanelService.h>
#include <alljoyn/controlpanel/ControlPanelController.h>
#include <alljoyn/services_common/GenericLogger.h>
#include "ControlPanelListenerImpl.h"
#include "ControllerNotificationReceiver.h"
#include <SrpKeyXListener.h>
#include <CommonSampleUtil.h>
#include <AnnounceHandlerImpl.h>

#define SERVICE_PORT 900

using namespace ajn;
using namespace services;
using namespace qcc;

BusAttachment* bus = 0;
ControlPanelService* controlPanelService = 0;
ControlPanelController* controlPanelController = 0;
ControlPanelListenerImpl* controlPanelListener = 0;
SrpKeyXListener* srpKeyXListener = 0;
AnnounceHandlerImpl* announceHandler = 0;
NotificationService* conService = 0;
ControllerNotificationReceiver* receiver = 0;
qcc::String ControlPanelPrefix = "/ControlPanel/";

void exitApp(int32_t signum)
{
    std::cout << "Program Finished" << std::endl;

    if (bus && announceHandler)
        AnnouncementRegistrar::UnRegisterAnnounceHandler(*bus, *announceHandler);

    if (controlPanelService) {
        controlPanelService->shutdownController();
        delete controlPanelService;
    }
    if (controlPanelController)
        delete controlPanelController;
    if (controlPanelListener)
        delete controlPanelListener;
    if (announceHandler)
        delete announceHandler;
    if (srpKeyXListener)
        delete srpKeyXListener;
    if (conService)
        conService->shutdown();
    if (receiver)
        delete receiver;
    if (bus)
        delete bus;

    std::cout << "Goodbye!" << std::endl;
    exit(signum);
}

static void announceHandlerCallback(qcc::String const& busName, unsigned short version, unsigned short port,
                                    const AnnounceHandler::ObjectDescriptions& objectDescs, const AnnounceHandler::AboutData& aboutData)
{
    controlPanelController->createControllableDevice(busName, objectDescs);
}

int main()
{
    QStatus status;

    // Allow CTRL+C to end application
    signal(SIGINT, exitApp);
    std::cout << "Beginning ControlPanel Application. (Press CTRL+C to end application)" << std::endl;

    // Initialize Service objects
#ifdef QCC_USING_BD
    PasswordManager::SetCredentials("ALLJOYN_PIN_KEYX", "000000");
#endif

    controlPanelService = ControlPanelService::getInstance();
    controlPanelService->setLogLevel(Log::LogLevel::LEVEL_INFO);

    srpKeyXListener = new SrpKeyXListener();

    bus = CommonSampleUtil::prepareBusAttachment(srpKeyXListener);
    if (bus == NULL) {
        std::cout << "Could not initialize BusAttachment." << std::endl;
        exitApp(1);
    }

    controlPanelController = new ControlPanelController();
    controlPanelListener = new ControlPanelListenerImpl(controlPanelController);

    status = controlPanelService->initController(bus, controlPanelController, controlPanelListener);
    if (status != ER_OK) {
        std::cout << "Could not initialize Controllee." << std::endl;
        exitApp(1);
    }

    announceHandler = new AnnounceHandlerImpl(NULL, announceHandlerCallback);
    AnnouncementRegistrar::RegisterAnnounceHandler(*bus, *announceHandler);

    conService = NotificationService::getInstance();
    receiver = new ControllerNotificationReceiver(controlPanelController);
    status = conService->initReceive(bus, receiver);
    if (status != ER_OK) {
        std::cout << "Could not initialize receiver." << std::endl;
        exitApp(1);
    }

    status = CommonSampleUtil::addSessionlessMatch(bus);
    if (status != ER_OK) {
        std::cout << "Could not add Sessionless Match" << std::endl;
        exitApp(1);
    }

    std::cout << "Finished setup. Waiting for Contollees" << std::endl;
    while (1) {
        sleep(1);
    }
}


