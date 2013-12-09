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

#include <alljoyn/controlpanel/ControlPanelService.h>
#include <alljoyn/controlpanel/Widget.h>
#include "../ControlPanelConstants.h"
#include "../BusObjects/WidgetBusObject.h"

#ifndef UINT32_MAX
#define UINT32_MAX      (4294967295U)
#endif
#define STATE_ENABLED 0x01
#define STATE_WRITABLE 0x02

namespace ajn {
namespace services {
using namespace cpsConsts;

Widget::Widget(qcc::String const& name, WidgetType widgetType, qcc::String const& tag) : m_Name(name), m_WidgetType(widgetType),
    m_IsSecured(false), m_Version(1), m_States(0), m_GetEnabled(0), m_GetWritable(0), m_BgColor(UINT32_MAX),
    m_GetBgColor(0), m_Label(""), m_GetLabels(0), m_WidgetMode(CONTROLLEE_WIDGET), m_Device(0), TAG(tag)
{
}

Widget::Widget(qcc::String const& name, ControlPanelDevice* device, WidgetType widgetType, qcc::String const& tag) : m_Name(name),
    m_WidgetType(widgetType), m_IsSecured(false), m_Version(1), m_States(0), m_GetEnabled(0), m_GetWritable(0), m_BgColor(UINT32_MAX),
    m_GetBgColor(0), m_Label(""), m_GetLabels(0), m_WidgetMode(CONTROLLER_WIDGET), m_Device(device), TAG(tag)
{
}

Widget::~Widget()
{

}

WidgetType Widget::getWidgetType() const
{
    return m_WidgetType;
}

qcc::String const& Widget::getWidgetName() const
{
    return m_Name;
}

WidgetMode Widget::getWidgetMode() const
{
    return m_WidgetMode;
}

const uint16_t Widget::getInterfaceVersion() const
{
    if (!m_BusObjects.size())
        return 1;

    return m_WidgetMode == CONTROLLEE_WIDGET ? m_BusObjects[0]->getInterfaceVersion() : m_Version;
}

void Widget::setIsSecured(bool secured)
{
    m_IsSecured = secured;
}

bool Widget::getIsSecured() const
{
    return m_IsSecured;
}

bool Widget::getIsEnabled() const
{
    return (m_States & STATE_ENABLED) == STATE_ENABLED;
}

bool Widget::getIsWritable() const
{
    return (m_States & STATE_WRITABLE) == STATE_WRITABLE;
}

void Widget::setEnabled(bool enabled)
{
    if (enabled)
        m_States = m_States | STATE_ENABLED;
    else
        m_States = m_States & ~STATE_ENABLED;
}

GetBoolFptr Widget::getGetEnabled() const
{
    return m_GetEnabled;
}

void Widget::setGetEnabled(GetBoolFptr getEnabled)
{
    m_GetEnabled = getEnabled;
}

void Widget::setWritable(bool writable)
{
    if (writable)
        m_States = m_States | STATE_WRITABLE;
    else
        m_States = m_States & ~STATE_WRITABLE;
}

void Widget::setGetWritable(GetBoolFptr getWritable)
{
    m_GetWritable = getWritable;
}

GetBoolFptr Widget::getGetWritable() const
{
    return m_GetWritable;
}

uint32_t Widget::getStates() const
{
    return m_States;
}

void Widget::setStates(uint8_t enabled, uint8_t writable)
{
    setEnabled(enabled);
    setWritable(writable);
}

uint32_t Widget::getBgColor() const
{
    return m_BgColor;
}

void Widget::setBgColor(uint32_t bgColor)
{
    this->m_BgColor = bgColor;
}

GetUint32Fptr Widget::getGetBgColor() const
{
    return m_GetBgColor;
}

void Widget::setGetBgColor(GetUint32Fptr getBgColor)
{
    m_GetBgColor = getBgColor;
}

const qcc::String& Widget::getLabel() const
{
    return m_Label;
}

const std::vector<qcc::String>& Widget::getLabels() const
{
    return m_Labels;
}

void Widget::setLabels(const std::vector<qcc::String>& labels)
{
    m_Labels = labels;
}

GetStringFptr Widget::getGetLabels() const
{
    return m_GetLabels;
}

void Widget::setGetLabels(GetStringFptr getLabels)
{
    m_GetLabels = getLabels;
}

const std::vector<uint16_t>& Widget::getHints() const
{
    return m_Hints;
}

void Widget::setHints(const std::vector<uint16_t>& hints)
{
    m_Hints = hints;
}

QStatus Widget::registerObjects(BusAttachment* bus, LanguageSet const& languageSet,
                                qcc::String const& objectPathPrefix, qcc::String const& objectPathSuffix, bool isRoot)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (!bus) {
        if (logger)
            logger->warn(TAG, "Could not register Object. Bus is NULL");
        return ER_BAD_ARG_1;
    }

    if (!(bus->IsStarted() && bus->IsConnected())) {
        if (logger)
            logger->warn(TAG, "Could not register Object. Bus is not started or not connected");
        return ER_BAD_ARG_1;
    }

    QStatus status = ER_OK;
    const std::vector<qcc::String>& languages = languageSet.getLanguages();
    qcc::String newObjectPathSuffix = isRoot ? objectPathSuffix : objectPathSuffix + "/" + m_Name;
    for (size_t indx = 0; indx < languages.size(); indx++) {
        qcc::String objectPath = objectPathPrefix + languages[indx] + newObjectPathSuffix;
        WidgetBusObject* busObject = createWidgetBusObject(bus, objectPath, indx, status);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not Create BusObject");
            delete busObject;
            return status;
        }
        status = bus->RegisterBusObject(*busObject);
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not register BusObject");
            delete busObject;
            return status;
        }
        m_BusObjects.push_back(busObject);
    }
    return status;
}

QStatus Widget::registerObjects(BusAttachment* bus, qcc::String const& objectPath)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (m_BusObjects.size()) {
        if (logger)
            logger->debug(TAG, "BusObjects arleady set. returning");
        return ER_OK;
    }

    if (!bus) {
        if (logger)
            logger->warn(TAG, "Could not register Object. Bus is NULL");
        return ER_BAD_ARG_1;
    }

    if (!(bus->IsStarted() && bus->IsConnected())) {
        if (logger)
            logger->warn(TAG, "Could not register Object. Bus is not started or not connected");
        return ER_BAD_ARG_1;
    }

    QStatus status;
    WidgetBusObject* busObject = createWidgetBusObject(bus, objectPath, 0, status);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Could not Create BusObject");
        delete busObject;
        return status;
    }

    m_BusObjects.push_back(busObject);
    status = busObject->setRemoteController(bus, m_Device->getDeviceBusName(), m_Device->getSessionId());
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Call to SetRemoteController failed");
        return status;
    }

    status = checkVersions();
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Call to CheckVersions failed");
        return status;
    }

    status = fillProperties();
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Call to FillProperties failed");
        return status;
    }

    status = addChildren(bus);
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Call to addChildren failed");
        return status;
    }
    return status;
}

QStatus Widget::unregisterObjects(BusAttachment* bus)
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (!bus) {
        if (logger)
            logger->warn(TAG, "Could not register Object. Bus is NULL");
        return ER_BAD_ARG_1;
    }

    if (!(bus->IsStarted() && bus->IsConnected())) {
        if (logger)
            logger->warn(TAG, "Could not register Object. Bus is not started or not connected");
        return ER_BAD_ARG_1;
    }

    std::vector<WidgetBusObject*>::iterator it;
    for (it = m_BusObjects.begin(); it != m_BusObjects.end();) {
        bus->UnregisterBusObject(*(*it));
        delete *it;
        it = m_BusObjects.erase(it);
    }
    return ER_OK;
}

QStatus Widget::fillStatesArg(MsgArg& val, uint16_t languageIndx)
{
    if (m_GetEnabled)
        setEnabled(m_GetEnabled());
    if (m_GetWritable)
        setWritable(m_GetWritable());
    return val.Set(AJPARAM_UINT32.c_str(), m_States);
}

QStatus Widget::fillOptParamsArg(MsgArg& val, uint16_t languageIndx)
{
    QStatus status;
    MsgArg* optParams = new MsgArg[OPT_PARAM_KEYS::NUM_OPT_PARAMS];
    size_t optParamIndx = 0;

    status = fillOptParamsArg(optParams, languageIndx, optParamIndx);
    if (status != ER_OK) {
        GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
        if (logger)
            logger->warn(TAG, "Could not marshal optParams");
        delete[] optParams;
        return status;
    }

    return val.Set(AJPARAM_ARRAY_DICT_UINT16_VAR.c_str(), optParamIndx, optParams);
}

QStatus Widget::fillOptParamsArg(MsgArg* optParams, uint16_t languageIndx, size_t& optParamIndx)
{
    QStatus status = ER_OK;

    if (m_Labels.size() > languageIndx || m_GetLabels) {
        MsgArg* labelArg = new MsgArg(AJPARAM_STR.c_str(), m_GetLabels ?
                                      m_GetLabels(languageIndx) : m_Labels[languageIndx].c_str());

        if ((status = optParams[optParamIndx].Set(AJPARAM_DICT_UINT16_VAR.c_str(),
                                                  OPT_PARAM_KEYS::LABEL_KEY, labelArg)) != ER_OK) {
            delete labelArg;
            return status;
        }
        optParams[optParamIndx++].SetOwnershipFlags(MsgArg::OwnsArgs, true);
    }

    if (m_BgColor != UINT32_MAX || m_GetBgColor) {
        MsgArg* bgColorArg = new MsgArg(AJPARAM_UINT32.c_str(), m_GetBgColor ?
                                        m_GetBgColor() : m_BgColor);

        if ((status = optParams[optParamIndx].Set(AJPARAM_DICT_UINT16_VAR.c_str(),
                                                  OPT_PARAM_KEYS::BGCOLOR_KEY, bgColorArg)) != ER_OK) {
            delete bgColorArg;
            return status;
        }
        optParams[optParamIndx++].SetOwnershipFlags(MsgArg::OwnsArgs, true);
    }

    if (m_Hints.size()) {
        MsgArg* hintsArg = new MsgArg(AJPARAM_ARRAY_UINT16.c_str(),
                                      m_Hints.size(), m_Hints.data());
        if ((status = optParams[optParamIndx].Set(AJPARAM_DICT_UINT16_VAR.c_str(),
                                                  OPT_PARAM_KEYS::HINT_KEY, hintsArg)) != ER_OK) {
            delete hintsArg;
            return status;
        }
        optParams[optParamIndx++].SetOwnershipFlags(MsgArg::OwnsArgs, true);
    }
    return status;
}

QStatus Widget::readVersionArg(MsgArg* val)
{
    uint16_t version = 0;
    QStatus status;
    CHECK_AND_RETURN(val->Get(AJPARAM_UINT16.c_str(), &version))
    m_Version = version;
    return status;
}

QStatus Widget::readStatesArg(MsgArg* val)
{
    uint32_t states = 0;
    QStatus status;
    CHECK_AND_RETURN(val->Get(AJPARAM_UINT32.c_str(), &states))
    m_States = states;
    return status;
}

QStatus Widget::readOptParamsArg(MsgArg* val)
{
    MsgArg* optParamsEntries;
    size_t optParamsNum;
    QStatus status;

    CHECK_AND_RETURN(val->Get(AJPARAM_ARRAY_DICT_UINT16_VAR.c_str(), &optParamsNum, &optParamsEntries));
    for (size_t i = 0; i < optParamsNum; i++) {
        uint16_t key;
        MsgArg* variant;

        CHECK_AND_BREAK(optParamsEntries[i].Get(AJPARAM_DICT_UINT16_VAR.c_str(), &key, &variant));
        CHECK_AND_BREAK(readOptParamsArg(key, variant))
    }
    return status;
}

QStatus Widget::readOptParamsArg(uint16_t key, MsgArg* val)
{
    QStatus status = ER_BUS_NO_SUCH_PROPERTY;
    switch (key) {
    case OPT_PARAM_KEYS::LABEL_KEY:
    {
        char* label;
        CHECK_AND_RETURN(val->Get(AJPARAM_STR.c_str(), &label))
        m_Label = label;
        break;
    }

    case OPT_PARAM_KEYS::BGCOLOR_KEY:
    {
        uint32_t bgColor;
        CHECK_AND_RETURN(val->Get(AJPARAM_UINT32.c_str(), &bgColor))
        m_BgColor = bgColor;
        break;
    }

    case OPT_PARAM_KEYS::HINT_KEY:
    {
        std::vector<uint16_t> hints;
        uint16_t* hintEntries;
        size_t hintNum;
        CHECK_AND_RETURN(val->Get(AJPARAM_ARRAY_UINT16.c_str(), &hintNum, &hintEntries));
        for (size_t i = 0; i < hintNum; i++) {
            hints.push_back(hintEntries[i]);
        }
        m_Hints = hints;
        break;
    }
    }

    return status;
}

QStatus Widget::SendPropertyChangedSignal()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    QStatus status = ER_OK;

    for (size_t indx = 0; indx < m_BusObjects.size(); indx++) {
        status = m_BusObjects[indx]->SendPropertyChangedSignal();
        if (status != ER_OK) {
            if (logger)
                logger->warn(TAG, "Could not send Property Changed Signal");
            return status;
        }
    }
    return status;
}

QStatus Widget::checkVersions()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (!m_BusObjects.size()) {
        if (logger)
            logger->warn(TAG, "BusObject is not set");
        return ER_BUS_BUS_NOT_STARTED;
    }
    return m_BusObjects[0]->checkVersions();
}

QStatus Widget::addChildren(BusAttachment* bus)
{
    return ER_OK;
}

QStatus Widget::fillProperties()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    if (!m_BusObjects.size()) {
        if (logger)
            logger->warn(TAG, "BusObject is not set");
        return ER_BUS_BUS_NOT_STARTED;
    }
    return m_BusObjects[0]->fillProperties();
}

void Widget::PropertyChanged()
{
    GenericLogger* logger = ControlPanelService::getInstance()->getLogger();
    ControlPanelListener* listener = m_Device->getListener();
    QStatus status = fillProperties();
    if (status != ER_OK) {
        if (logger)
            logger->warn(TAG, "Something went wrong reloading properties");
        //TODO: Opportunity for errorOccured
        //if (listener) listener->errorOccured...
        return;
    }

    if (listener)
        listener->signalPropertiesChanged(m_Device, this);
}

} /* namespace services */
} /* namespace ajn */
