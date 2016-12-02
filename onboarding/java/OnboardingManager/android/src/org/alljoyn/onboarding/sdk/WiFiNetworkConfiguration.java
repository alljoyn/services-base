/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

package org.alljoyn.onboarding.sdk;

import org.alljoyn.onboarding.OnboardingService.AuthType;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * WiFiNetwork encapsulates information how to connect to a WIFI network
 *
 */
public class WiFiNetworkConfiguration extends WiFiNetwork implements Parcelable {

    /**
     * WIFI password
     */
    private String password = null;
    /**
     * WIFI SSID hidden
     */
    private boolean hidden = false;

    /**
     * Constructor with SSID,authType,password,cipher
     *
     * @param SSID
     * @param authType
     * @param password
     */
    public WiFiNetworkConfiguration(String SSID, AuthType authType, String password) {
        super.setSSID(SSID);
        this.password = password;
        this.authType = authType;

    }

    public WiFiNetworkConfiguration(String SSID, AuthType authType, String password, boolean isHidden) {
        super.setSSID(SSID);
        this.password = password;
        this.authType = authType;
        this.hidden = isHidden;
    }

    /**
     * Constructor from a Parcel
     *
     * @param in
     */
    public WiFiNetworkConfiguration(Parcel in) {
        SSID = in.readString();
        password = in.readString();
        authType = AuthType.getAuthTypeById((short) in.readInt());
        hidden = (in.readByte() != 0);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    /**
     *  write the members to the Parcel
     */
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(SSID);
        dest.writeString(password);
        dest.writeInt(authType.getTypeId());
        dest.writeByte((byte) (hidden ? 1 : 0));
    }

    public static final Parcelable.Creator<WiFiNetworkConfiguration> CREATOR = new Parcelable.Creator<WiFiNetworkConfiguration>() {
        @Override
        public WiFiNetworkConfiguration createFromParcel(Parcel in) {
            return new WiFiNetworkConfiguration(in);
        }

        @Override
        public WiFiNetworkConfiguration[] newArray(int size) {
            return new WiFiNetworkConfiguration[size];
        }
    };

    /**
     * Get {@link #password}
     *
     * @return the password of the network {@link #password}
     */
    public String getPassword() {
        return password;
    }

    public boolean isHidden() {
        return hidden;
    }

}