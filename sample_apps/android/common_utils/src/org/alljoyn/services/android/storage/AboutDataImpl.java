/******************************************************************************
 *  *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

package org.alljoyn.services.android.storage;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.UUID;

import javax.xml.parsers.ParserConfigurationException;

import org.alljoyn.bus.AboutKeys;
import org.alljoyn.bus.AboutData;
import org.alljoyn.bus.AboutDataListener;
import org.alljoyn.bus.ErrorReplyBusException;
import org.alljoyn.bus.Variant;
import org.alljoyn.common.TransformUtil;
import org.xml.sax.SAXException;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

/**
 * Reads the factory defaults from assets/Config.xml and creates an AboutData object.
 * Stores custom configuration in local XML files.
 */
public class AboutDataImpl extends AboutData
{
    protected static final String CONFIG_XML = "Config.xml";
    public static final String TAG = AboutDataImpl.class.getName();
    private String m_defaultLanguage = "en";
    private Set<String> m_supportedLanguages = new HashSet<String>();

    /**
     * configuration storage.
     */
    private final AssetManager m_assetMgr;
    private Context m_context;
    private Map<String, Property> m_aboutConfigMap;

    public String getDefaultLanguage()
    {
        return m_defaultLanguage;
    }

    public Set<String> getLanguages()
    {
        return m_supportedLanguages;
    }

    public Context getContext()
    {
        return m_context;
    }

    public Map<String, Property> getAboutConfigMap()
    {
        return m_aboutConfigMap;
    }

    public AboutDataImpl(Context context)
    {
        super();
        this.m_context = context;
        m_assetMgr = context.getAssets();
        m_context = context;

        // read factory defaults from assets/Config.xml
        loadFactoryDefaults();

        // figure out the available languages from the contents of Config.xml
        loadLanguages();

        // read modified-and-persisted configuration and override factory
        // defaults
        loadStoredConfiguration();

        // read the user's default language
        setDefaultLanguageFromProperties();

        // load the unique Id. Create if not exist
        loadAppId();

        // load the device Id. Create if not exist
        loadDeviceId();

        storeConfiguration();
    }

    protected void getConfiguration(String languageTag, Map<String, Object> configuration)
    {
        for (String key : m_aboutConfigMap.keySet())
        {
            Property property = m_aboutConfigMap.get(key);
            if (!property.isPublic() || !property.isWritable())
            {
                continue;
            }
            Object value = property.getValue(languageTag, m_defaultLanguage);
            if (value != null)
                configuration.put(key, value);
        }
    }

    protected void getAbout(String languageTag, Map<String, Object> about)
    {
        for (String key : m_aboutConfigMap.keySet())
        {
            Property property = m_aboutConfigMap.get(key);
            if (!property.isPublic())
            {
                continue;
            }
            Object value = property.getValue(languageTag, m_defaultLanguage);
            if (value != null)
                about.put(key, value);
        }
    }

    protected void getAnnouncement(String languageTag, Map<String, Object> announce)
    {
        for (String key : m_aboutConfigMap.keySet())
        {
            Property property = m_aboutConfigMap.get(key);
            if (!property.isPublic() || !property.isAnnounced())
            {
                continue;
            }
            Object value = property.getValue(languageTag, m_defaultLanguage);
            if (value != null)
                announce.put(key, value);
        }
    }

    /**
     * Set a value for a property
     * 
     * @param key
     *            property name
     * @param value
     *            property value
     * @param languageTag
     *            the language for which to set the value
     * @see Property
     */
    public void setValue(String key, Object value, String languageTag)
    {
        Property property = m_aboutConfigMap.get(key);
        if (property == null)
        {
            property = new Property(key, true, true, true);
            m_aboutConfigMap.put(key, property);
        }
        property.setValue(languageTag, value);

    }

    protected void loadLanguages()
    {
        Set<String> languages = new HashSet<String>(3);
        for (String key : m_aboutConfigMap.keySet())
        {
            Property property = m_aboutConfigMap.get(key);
            Set<String> langs = property.getLanguages();
            if (langs.size() != 0)
                languages.addAll(langs);
        }
        languages.remove(Property.NO_LANGUAGE);
        m_supportedLanguages = languages;
        Property property = new Property(AboutKeys.ABOUT_SUPPORTED_LANGUAGES, false, false, true);
        property.setValue(Property.NO_LANGUAGE, m_supportedLanguages);
        m_aboutConfigMap.put(AboutKeys.ABOUT_SUPPORTED_LANGUAGES, property);
    }

    protected void loadFactoryDefaults()
    {
        try
        {
            InputStream is = m_assetMgr.open(CONFIG_XML);
            m_aboutConfigMap = PropertyParser.readFromXML(is);
        }
        catch (IOException e)
        {
            Log.e(TAG, "Error loading file assets/" + CONFIG_XML, e);
            m_aboutConfigMap = createCannedMap();
        }
        catch (ParserConfigurationException e)
        {
            Log.e(TAG, "Error parsing xml file assets/" + CONFIG_XML, e);
            m_aboutConfigMap = createCannedMap();
        }
        catch (SAXException e)
        {
            Log.e(TAG, "Error parsing xml file assets/" + CONFIG_XML, e);
            m_aboutConfigMap = createCannedMap();
        }
    }

    protected void loadStoredConfiguration()
    {
        try
        {
            if (new File(m_context.getFilesDir() + "/" + CONFIG_XML).exists())
            {
                InputStream is = m_context.openFileInput(CONFIG_XML);
                Map<String, Property> storedConfiguration = PropertyParser.readFromXML(is);
                for (String key : storedConfiguration.keySet())
                {

                    Property property = m_aboutConfigMap.get(key);
                    Property storedProperty = storedConfiguration.get(key);

                    if (storedProperty != null)
                    { // should never happen

                        if (property == null)
                        {
                            m_aboutConfigMap.put(key, storedProperty);
                        }
                        else
                        {
                            for (String language : storedProperty.getLanguages())
                            {
                                Object languageValue = storedProperty.getValue(language, language);
                                property.setValue(language, languageValue);
                            }
                        }

                    }// if :: storedProperty != null

                }// for :: storedConfiguration
            }
        }
        catch (IOException e)
        {
            Log.e(TAG, "Error loading file " + CONFIG_XML, e);
        }
        catch (ParserConfigurationException e)
        {
            Log.e(TAG, "Error parsing xml file " + CONFIG_XML, e);
        }
        catch (SAXException e)
        {
            Log.e(TAG, "Error parsing xml file " + CONFIG_XML, e);
        }
    }

    protected void storeConfiguration()
    {
        String localConfigFileName = CONFIG_XML;
        // Note: this one is on the app's folder, not in assets
        try
        {
            FileOutputStream openFileOutput = m_context.openFileOutput(localConfigFileName, Context.MODE_PRIVATE);
            PropertyParser.writeToXML(openFileOutput, m_aboutConfigMap);
        }
        catch (FileNotFoundException e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        catch (IllegalArgumentException e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        catch (IllegalStateException e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        catch (IOException e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    protected Map<String, Property> createCannedMap()
    {
        Map<String, Property> aboutMap = new HashMap<String, Property>(10);

        Property property = new Property(AboutKeys.ABOUT_APP_NAME, false, true, true);
        property.setValue(m_defaultLanguage, "Demo appname");
        aboutMap.put(AboutKeys.ABOUT_APP_NAME, property);

        property = new Property(AboutKeys.ABOUT_DEVICE_NAME, true, true, true);
        property.setValue(m_defaultLanguage, "Demo device");
        aboutMap.put(AboutKeys.ABOUT_DEVICE_NAME, property);

        property = new Property(AboutKeys.ABOUT_MANUFACTURER, false, true, true);
        property.setValue(m_defaultLanguage, "Demo manufacturer");
        aboutMap.put(AboutKeys.ABOUT_MANUFACTURER, property);

        property = new Property(AboutKeys.ABOUT_DESCRIPTION, false, false, true);
        property.setValue(m_defaultLanguage, "A default app that demonstrates the About/Config feature");
        aboutMap.put(AboutKeys.ABOUT_DESCRIPTION, property);

        property = new Property(AboutKeys.ABOUT_DEFAULT_LANGUAGE, true, true, true);
        property.setValue(m_defaultLanguage, m_defaultLanguage);
        aboutMap.put(AboutKeys.ABOUT_DEFAULT_LANGUAGE, property);

        property = new Property(AboutKeys.ABOUT_SOFTWARE_VERSION, false, false, true);
        property.setValue(m_defaultLanguage, "1.0.0.0");
        aboutMap.put(AboutKeys.ABOUT_SOFTWARE_VERSION, property);

        property = new Property(AboutKeys.ABOUT_AJ_SOFTWARE_VERSION, false, false, true);
        property.setValue(m_defaultLanguage, "3.3.1");
        aboutMap.put(AboutKeys.ABOUT_AJ_SOFTWARE_VERSION, property);

        property = new Property(AboutKeys.ABOUT_MODEL_NUMBER, false, true, true);
        property.setValue(m_defaultLanguage, "S100");
        aboutMap.put(AboutKeys.ABOUT_MODEL_NUMBER, property);

        property = new Property(AboutKeys.ABOUT_SUPPORTED_LANGUAGES, false, false, true);
        property.setValue(m_defaultLanguage, m_supportedLanguages);
        aboutMap.put(AboutKeys.ABOUT_SUPPORTED_LANGUAGES, property);

        return aboutMap;
    }

    /**
	 * 
	 */
    protected void setDefaultLanguageFromProperties()
    {
        Property defaultLanguageProperty = m_aboutConfigMap.get(AboutKeys.ABOUT_DEFAULT_LANGUAGE);
        if (defaultLanguageProperty != null)
        {
            m_defaultLanguage = (String) defaultLanguageProperty.getValue(Property.NO_LANGUAGE, Property.NO_LANGUAGE);
        }
    }

    /**
     * If appId was not found in factory defaults or on persistent storage,
     * generate it
     */
    public void loadAppId()
    {
        Property appIdProperty = m_aboutConfigMap.get(AboutKeys.ABOUT_APP_ID);

        if (appIdProperty == null || appIdProperty.getValue(Property.NO_LANGUAGE, Property.NO_LANGUAGE) == null)
        {
            UUID defaultAppId = UUID.randomUUID();
            // String sAppId =
            // String.valueOf(TransformUtil.uuidToByteArray(defaultAppId));
            // here we take the stored about map, and fill gaps by default
            // values. We don't shrink the map - other existing values will
            // remain.
            Property property = new Property(AboutKeys.ABOUT_APP_ID, false, true, true);
            property.setValue(Property.NO_LANGUAGE, defaultAppId);
            m_aboutConfigMap.put(AboutKeys.ABOUT_APP_ID, property);
        }
    }

    /**
     * If uniqueId was not found in factory defaults or on persistent storage,
     * generate it
     */
    public void loadDeviceId()
    {
        Property deviceIdProperty = m_aboutConfigMap.get(AboutKeys.ABOUT_DEVICE_ID);

        if (deviceIdProperty == null || deviceIdProperty.getValue(Property.NO_LANGUAGE, Property.NO_LANGUAGE) == null)
        {
            String defaultDeviceId = String.valueOf("IoE" + System.currentTimeMillis());

            // here we take the stored about map, and fill gaps by default
            // values. We don't shrink the map - other existing values will
            // remain.
            Property property = new Property(AboutKeys.ABOUT_DEVICE_ID, false, true, true);
            property.setValue(Property.NO_LANGUAGE, defaultDeviceId);
            m_aboutConfigMap.put(AboutKeys.ABOUT_DEVICE_ID, property);
        }

    }

    @Override
    public Map<String, Variant> getAboutData(String lang) throws ErrorReplyBusException
    {
        Map<String, Object> aboutData = new HashMap<String, Object>(3);
        getAbout(lang, aboutData);
        return TransformUtil.toVariantMap(aboutData);
    }

    @Override
    public Map<String, Variant> getAnnouncedAboutData() throws ErrorReplyBusException
    {
        Map<String, Object> announceData = new HashMap<String, Object>(3);
        getAnnouncement(m_defaultLanguage, announceData);
        return TransformUtil.toVariantMap(announceData);
    }
}