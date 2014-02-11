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

#include <alljoyn/controlpanel/ControlPanelInterfaces.h>

/**
 * Properties for interfaces
 */
static const char Version[] = "@Version>q";
static const char States[] = "@States>u";
static const char OptParams[] = "@OptParams>a{qv}";
static const char Value[] = "@Value=v";
static const char Message[] = "@Message>s";
static const char NumActions[] = "@NumActions>q";
static const char PropertiesChanged[] = "!MetadataChanged";
static const char Label[] = "@Label>s";

/**
 * ControlPanel Interfaces
 */
static const char ControlPanelInterfaceName[] = "org.alljoyn.ControlPanel.ControlPanel";

static const char* ControlPanelInterface[] =
{
    ControlPanelInterfaceName,             /* The first entry is the interface name. */
    Version,                               /* The Version Property */
    NULL
};

const AJ_InterfaceDescription ControlPanelInterfaces[] =
{
    AJ_PropertiesIface,
    ControlPanelInterface,
    NULL
};

/**
 * NotificationAction Interfaces
 */
static const char NotificationActionInterfaceName[] = "org.alljoyn.ControlPanel.NotificationAction";
static const char NotificationActionDismiss[] = "!Dismiss";

static const char* NotificationActionInterface[] =
{
    NotificationActionInterfaceName,       /* The first entry is the interface name. */
    Version,                               /* The Version Property */
    NotificationActionDismiss,             /* Signal to dismiss notification action */
    NULL
};

const AJ_InterfaceDescription NotificationActionInterfaces[] =
{
    AJ_PropertiesIface,
    NotificationActionInterface,
    NULL
};

/**
 * HTTPControl Interfaces
 */
static const char HttpControlInterfaceName[] = "org.alljoyn.ControlPanel.HTTPControl";
static const char HttpControlGetUrl[] = "?GetRootURL >s";

static const char* HttpControlInterface[] =
{
    HttpControlInterfaceName,              /* The first entry is the interface name. */
    Version,                               /* The Version Property */
    HttpControlGetUrl,                     /* Method to retrieve the Control Url */
    NULL
};

const AJ_InterfaceDescription HttpControlInterfaces[] =
{
    AJ_PropertiesIface,
    HttpControlInterface,
    NULL
};

/**
 * Container Interfaces
 */
static const char ContainerControlInterfaceName[] = "org.alljoyn.ControlPanel.Container";
static const char SecuredContainerControlInterfaceName[] = "$org.alljoyn.ControlPanel.SecuredContainer";

static const char* ContainerControlInterface[] =
{
    ContainerControlInterfaceName,         /* The first entry is the interface name. */
    Version,                               /* The Version of the Container */
    States,                                /* The States of the Container */
    OptParams,                             /* The OptParams of the Container */
    PropertiesChanged,                     /* One of the Properties changed*/
    NULL
};

static const char* SecuredContainerControlInterface[] =
{
    SecuredContainerControlInterfaceName,         /* The first entry is the interface name. */
    Version,                               /* The Version of the Container */
    States,                                /* The States of the Container */
    OptParams,                             /* The OptParams of the Container */
    PropertiesChanged,                     /* One of the Properties changed*/
    NULL
};

const AJ_InterfaceDescription ContainerInterfaces[] =
{
    AJ_PropertiesIface,
    ContainerControlInterface,
    NULL
};

const AJ_InterfaceDescription SecuredContainerInterfaces[] =
{
    AJ_PropertiesIface,
    SecuredContainerControlInterface,
    NULL
};

/**
 * PropertyInterfaces
 */
static const char PropertyControlInterfaceName[] = "org.alljoyn.ControlPanel.Property";
static const char SecuredPropertyControlInterfaceName[] = "$org.alljoyn.ControlPanel.SecuredProperty";
static const char ValueChanged[] = "!ValueChanged >v";

static const char* PropertyControlInterface[] =
{
    PropertyControlInterfaceName,          /* The first entry is the interface name. */
    Version,                               /* The Version of the Property */
    States,                                /* The States of the Property */
    OptParams,                             /* The OptParams of the Property */
    PropertiesChanged,                     /* One of the Properties changed*/
    Value,                                 /* The Value of the Property */
    ValueChanged,                          /* A signal to announce to property's value has changed */
    NULL
};

static const char* SecuredPropertyControlInterface[] =
{
    SecuredPropertyControlInterfaceName,          /* The first entry is the interface name. */
    Version,                               /* The Version of the Property */
    States,                                /* The States of the Property */
    OptParams,                             /* The OptParams of the Property */
    PropertiesChanged,                     /* One of the Properties changed*/
    Value,                                 /* The Value of the Property */
    ValueChanged,                          /* A signal to announce to property's value has changed */
    NULL
};

const AJ_InterfaceDescription PropertyInterfaces[] =
{
    AJ_PropertiesIface,
    PropertyControlInterface,
    NULL
};


const AJ_InterfaceDescription SecuredPropertyInterfaces[] =
{
    AJ_PropertiesIface,
    SecuredPropertyControlInterface,
    NULL
};


/**
 * LabelProperty Interfaces
 */
static const char LabelPropertyInterfaceName[] = "org.alljoyn.ControlPanel.LabelProperty";

static const char* LabelPropertyInterface[] =
{
    LabelPropertyInterfaceName,        /* The first entry is the interface name. */
    Version,                               /* The Version of the LabelProperty */
    States,                                /* The States of the LabelProperty */
    OptParams,                             /* The OptParams of the LabelProperty */
    PropertiesChanged,                     /* One of the Properties changed*/
    Label,                                 /* The Label of the LabelProperty */
    NULL
};

const AJ_InterfaceDescription LabelPropertyInterfaces[] =
{
    AJ_PropertiesIface,
    LabelPropertyInterface,
    NULL
};

/**
 * Action Interfaces
 */
static const char ActionControlInterfaceName[] = "org.alljoyn.ControlPanel.Action";
static const char SecuredActionControlInterfaceName[] = "$org.alljoyn.ControlPanel.SecuredAction";
static const char Exec[] = "?Exec";

static const char* ActionControlInterface[] =
{
    ActionControlInterfaceName,            /* The first entry is the interface name. */
    Version,                               /* The Version of the Action */
    States,                                /* The States of the Action */
    OptParams,                             /* The OptParams of the Action */
    PropertiesChanged,                     /* One of the Properties changed*/
    Exec,                                  /* The Method to execute the action */
    NULL
};

static const char* SecuredActionControlInterface[] =
{
    SecuredActionControlInterfaceName,     /* The first entry is the interface name. */
    Version,                               /* The Version of the Action */
    States,                                /* The States of the Action */
    OptParams,                             /* The OptParams of the Action */
    PropertiesChanged,                     /* One of the Properties changed*/
    Exec,                                  /* The Method to execute the action */
    NULL
};

const AJ_InterfaceDescription ActionInterfaces[] =
{
    AJ_PropertiesIface,
    ActionControlInterface,
    NULL
};

const AJ_InterfaceDescription SecuredActionInterfaces[] =
{
    AJ_PropertiesIface,
    SecuredActionControlInterface,
    NULL
};

/**
 * Dialog Interfaces
 */
static const char DialogInterfaceName[] = "org.alljoyn.ControlPanel.Dialog";
static const char SecuredDialogInterfaceName[] = "$org.alljoyn.ControlPanel.SecuredDialog";
static const char Action1[] = "?Action1";
static const char Action2[] = "?Action2";
static const char Action3[] = "?Action3";

static const char* DialogInterface[] =
{
    DialogInterfaceName,                   /* The first entry is the interface name. */
    Version,                               /* The Version of the Dialog */
    States,                                /* The States of the Dialog */
    OptParams,                             /* The OptParams of the Dialog */
    PropertiesChanged,                     /* One of the Properties changed*/
    Message,                               /* The Message of the Dialog */
    NumActions,                            /* The NumActions of the Dialog */
    Action1,                               /* The Method Action1 of the Dialog */
    Action2,                               /* The Method Action2 of the Dialog */
    Action3,                               /* The Method Action3 of the Dialog */
    NULL
};

static const char* SecuredDialogInterface[] =
{
    SecuredDialogInterfaceName,            /* The first entry is the interface name. */
    Version,                               /* The Version of the Dialog */
    States,                                /* The States of the Dialog */
    OptParams,                             /* The OptParams of the Dialog */
    PropertiesChanged,                     /* One of the Properties changed*/
    Message,                               /* The Message of the Dialog */
    NumActions,                            /* The NumActions of the Dialog */
    Action1,                               /* The Method Action1 of the Dialog */
    Action2,                               /* The Method Action2 of the Dialog */
    Action3,                               /* The Method Action3 of the Dialog */
    NULL
};

const AJ_InterfaceDescription DialogInterfaces[] =
{
    AJ_PropertiesIface,
    DialogInterface,
    NULL
};

const AJ_InterfaceDescription SecuredDialogInterfaces[] =
{
    AJ_PropertiesIface,
    SecuredDialogInterface,
    NULL
};
