/******************************************************************************
 * Copyright (c) 2013 - 2014, AllSeen Alliance. All rights reserved.
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

#include <alljoyn/controlpanel/Common/BaseWidget.h>
#include <alljoyn/controlpanel/Definitions.h>
#include <alljoyn/controlpanel/Common/ControlMarshalUtil.h>

#define BASE_ENABLED 0x01
#define BASE_WRITABLE 0x02

void initializeBaseWidget(BaseWidget* widget)
{
    widget->states = 0;
    widget->getEnabled = 0;
    widget->getWritable = 0;
    widget->numLanguages = 0;
    widget->interfaceVersion = 1;

    widget->marshalVersion = marshalBaseVersion;
    widget->marshalStates = marshalBaseStates;
    widget->marshalOptParam = marshalOnlyBaseOptParam;
    widget->marshalAllProp = marshalAllOnlyBaseProperties;

    initializeBaseOptParam(&widget->optParams);
}

void setBaseEnabled(BaseWidget* widget, uint8_t enabled)
{
    if (enabled) {
        widget->states = widget->states | BASE_ENABLED;
    } else {
        widget->states = widget->states & ~BASE_ENABLED;
    }
}

void setBaseWritable(BaseWidget* widget, uint8_t writeable)
{
    if (writeable) {
        widget->states = widget->states | BASE_WRITABLE;
    } else {
        widget->states = widget->states & ~BASE_WRITABLE;
    }
}

AJ_Status marshalBaseVersion(BaseWidget* widget, AJ_Message* reply, uint16_t language)
{
    return AJ_MarshalArgs(reply, PROPERTY_TYPE_VERSION_SIG, widget->interfaceVersion);
}

AJ_Status marshalBaseStates(BaseWidget* widget, AJ_Message* reply, uint16_t languages)
{
    if (widget->getEnabled) {
        setBaseEnabled(widget, widget->getEnabled());
    }
    if (widget->getWritable) {
        setBaseWritable(widget, widget->getWritable());
    }
    return AJ_MarshalArgs(reply, PROPERTY_TYPE_STATES_SIG, widget->states);
}

void initializeBaseOptParam(BaseOptParams* optParam)
{
    optParam->label = 0;
    optParam->getLabel = 0;

    optParam->bgColor = UINT32_MAX;
    optParam->getBgColor = 0;

    optParam->hints = 0;
    optParam->numHints = 0;
}

AJ_Status marshalOnlyBaseOptParam(BaseWidget* widget, AJ_Message* reply, uint16_t language)
{
    AJ_Status status;
    AJ_Arg baseOptParams;

    status = StartOptionalParams(reply, &baseOptParams);
    if (status != AJ_OK) {
        return status;
    }

    status = marshalBaseOptParam(widget, reply, language);
    if (status != AJ_OK) {
        return status;
    }

    return AJ_MarshalCloseContainer(reply, &baseOptParams);
}

AJ_Status marshalBaseOptParam(BaseWidget* widget, AJ_Message* reply, uint16_t language)
{
    BaseOptParams* optParams = &widget->optParams;
    AJ_Status status = AJ_OK;
    AJ_Arg baseLayoutHints;

    if (language >= widget->numLanguages) {
        return AJ_ERR_UNEXPECTED;
    }
    if (optParams->getLabel) {
        const char* label = optParams->getLabel(language);
        status = AddBasicOptionalParam(reply, BASE_LABEL, BASE_LABEL_SIG, &label);
        if (status != AJ_OK) {
            return status;
        }
    } else if (optParams->label) {
        const char* label = optParams->label[language];
        status = AddBasicOptionalParam(reply, BASE_LABEL, BASE_LABEL_SIG, &label);
        if (status != AJ_OK) {
            return status;
        }
    }

    if (optParams->getBgColor) {
        uint32_t bgColor = optParams->getBgColor();
        status = AddBasicOptionalParam(reply, BASE_BG_COLOR, BASE_BG_COLOR_SIG, &bgColor);
        if (status != AJ_OK) {
            return status;
        }
    } else if (optParams->bgColor != UINT32_MAX) {
        status = AddBasicOptionalParam(reply, BASE_BG_COLOR, BASE_BG_COLOR_SIG, &optParams->bgColor);
        if (status != AJ_OK) {
            return status;
        }
    }

    if (optParams->hints && optParams->numHints) {
        status = StartComplexOptionalParam(reply, &baseLayoutHints, BASE_LAYOUT_HINTS, BASE_LAYOUT_HINTS_SIG);
        if (status != AJ_OK) {
            return status;
        }

        status = AddHints(reply, optParams->hints, optParams->numHints);
        if (status != AJ_OK) {
            return status;
        }

        status = AJ_MarshalCloseContainer(reply, &baseLayoutHints);
        if (status != AJ_OK) {
            return status;
        }
    }

    return status;
}

AJ_Status marshalAllBaseProperties(BaseWidget* widget, AJ_Message* reply, uint16_t language)
{
    AJ_Status status;
    status = AddPropertyForGetAll(reply, PROPERTY_TYPE_VERSION_NAME, PROPERTY_TYPE_VERSION_SIG, widget, language, marshalBaseVersion);
    if (status != AJ_OK) {
        return status;
    }

    return AddPropertyForGetAll(reply, PROPERTY_TYPE_STATES_NAME, PROPERTY_TYPE_STATES_SIG,
                                widget, language, marshalBaseStates);
}

AJ_Status marshalAllOnlyBaseProperties(BaseWidget* widget, AJ_Message* reply, uint16_t language)
{
    AJ_Status status;
    AJ_Arg widgetGetAllArray;

    status = AJ_MarshalContainer(reply, &widgetGetAllArray, AJ_ARG_ARRAY);
    if (status != AJ_OK) {
        return status;
    }

    status = marshalAllBaseProperties(widget, reply, language);
    if (status != AJ_OK) {
        return status;
    }

    status = AddPropertyForGetAll(reply, PROPERTY_TYPE_OPTPARAMS_NAME, PROPERTY_TYPE_OPTPARAMS_SIG, widget, language, marshalOnlyBaseOptParam);
    if (status != AJ_OK) {
        return status;
    }

    return AJ_MarshalCloseContainer(reply, &widgetGetAllArray);
}
