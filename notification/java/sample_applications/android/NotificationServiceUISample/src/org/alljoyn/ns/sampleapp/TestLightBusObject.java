package org.alljoyn.ns.sampleapp;

import org.alljoyn.bus.BusException;
import org.alljoyn.bus.BusObject;
import org.alljoyn.bus.annotation.BusInterface;
import org.alljoyn.bus.annotation.BusMethod;

/**
 * Simple interface for a Light Switch object. Used by the notification
 * sample to showcase invoking a method on a remote object in response to receiving a notification.
 */
@BusInterface(name=TestLightBusObject.INTERFACE_NAME)
public interface TestLightBusObject extends BusObject
{
    public static final String INTERFACE_NAME = "org.alljoyn.Samples.LightSwitch";
    public static final String OBJ_PATH = "/testLightObject";

    @BusMethod()
    public void flipSwitch() throws BusException;

    @BusMethod()
    public boolean isLightOn() throws BusException;
}