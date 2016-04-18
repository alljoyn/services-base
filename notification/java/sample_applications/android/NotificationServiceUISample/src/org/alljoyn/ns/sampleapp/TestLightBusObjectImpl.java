package org.alljoyn.ns.sampleapp;

import org.alljoyn.bus.BusException;

/**
 * Simple implementation for a Light Switch object. Used by the notification
 * sample to showcase invoking a method on a remote object in response to receiving a notification.
 */
public class TestLightBusObjectImpl implements TestLightBusObject
{
    private boolean lightSwitchOn;

    public TestLightBusObjectImpl(){
        lightSwitchOn = false;
    }

    public void flipSwitch() throws BusException{
        lightSwitchOn = !lightSwitchOn;
    }

    public boolean isLightOn() throws BusException{
        return lightSwitchOn;
    }
}