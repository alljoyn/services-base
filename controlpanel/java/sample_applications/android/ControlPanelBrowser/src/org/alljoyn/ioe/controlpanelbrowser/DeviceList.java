package org.alljoyn.ioe.controlpanelbrowser;
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
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.alljoyn.services.common.BusObjectDescription;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

/**
 * Helper class for providing sample content for user interfaces created by
 * Android template wizards.
 * <p>
 * TODO: Replace all uses of this class before publishing your app.
 */
public class DeviceList {

	/**
	 * An array of ControlPanelContexts.
	 */
	private List<DeviceContext> contexts = new ArrayList<DeviceContext>();

	public void addItem(DeviceContext item) {
		contexts.add(item);
	}
	
	public List<DeviceContext> getContexts() {
		return contexts;
	}

	/**
	 * An item representing a device.
	 */
	public static class DeviceContext implements Parcelable {
		final private String deviceId;
		final private String busName;
		final private String label;
		final private Map<String,String[]> object2Interfaces; 

		public DeviceContext(String deviceId, String busName, String deviceName) {
			Log.d("DeviceList", String.format("Adding a new device. id=%s, busName=%s, name=%s",  deviceId, busName, deviceName));
			this.deviceId = deviceId;
			this.busName = busName;
			this.object2Interfaces = new HashMap<String,String[]>(5);
			label = deviceName + '(' + busName + ')';
		}

		
		public String getDeviceId() {
			return deviceId;
		}


		public String getBusName() {
			return busName;
		}


		public String getLabel() {
			return label;
		}

		public void addObjectInterfaces(String objPath, String[] interfaces) {
			object2Interfaces.put(objPath, interfaces);
		}
		
		public String[] getInterfaces(String objPath) {
			return object2Interfaces.get(objPath);
		}
		
		@Override
		public String toString() {
			return label;
		}

		@Override
	     public int describeContents() {
	         return 0;
	     }

	     public void writeToParcel(Parcel out, int flags) {
	         out.writeStringArray(new String[]{deviceId, busName, label});
	         out.writeMap(this.object2Interfaces);
	     }

	     public static final Parcelable.Creator<DeviceContext> CREATOR
	             = new Parcelable.Creator<DeviceContext>() {
	         public DeviceContext createFromParcel(Parcel in) {
	             return new DeviceContext(in);
	         }

	         public DeviceContext[] newArray(int size) {
	             return new DeviceContext[size];
	         }
	     };
	     
	     private DeviceContext(Parcel in) {
	    	 String[] fields = new String[3];
	    	 in.readStringArray(fields);
	    	 this.deviceId 	= fields[0];
	    	 this.busName 	= fields[1];
	    	 this.label 	= fields[2];
	    	 this.object2Interfaces = new HashMap<String,String[]>(5);
	    	 in.readMap(object2Interfaces, getClass().getClassLoader());
	     }


		public Set<String> getBusObjects() {
			return object2Interfaces.keySet();
		}
	}

	public void onDeviceOffline(String busName) {
		Collection<DeviceContext> toRemove = new ArrayList<DeviceContext>();
		for (DeviceContext context: contexts) {
			if (context.busName.equals(busName)) {
				toRemove.add(context);
			}
		}
		contexts.removeAll(toRemove);
	}
	
	
}
