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

package org.alljoyn.config.sample;



import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.FileOutputStream;
import java.io.UnsupportedEncodingException;
import java.io.File;
import java.io.Writer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.UUID;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathFactory;

import org.alljoyn.services.common.PropertyStore;
import org.alljoyn.services.common.PropertyStoreException;
import org.alljoyn.services.common.PropertyStore.Filter;
import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.BusListener;
import org.alljoyn.bus.BusObject;
import org.alljoyn.bus.Mutable;
import org.alljoyn.bus.SessionOpts;
import org.alljoyn.bus.SessionPortListener;
import org.alljoyn.bus.Status;
import org.alljoyn.about.AboutKeys;
import org.alljoyn.about.AboutServiceImpl;
import org.alljoyn.config.ConfigServiceImpl;
import org.alljoyn.config.transport.ConfigTransport;
import org.alljoyn.bus.AuthListener;
import org.alljoyn.bus.Variant;
import org.alljoyn.config.server.ConfigChangeListener;
import org.alljoyn.config.server.FactoryResetHandler;
import org.alljoyn.config.server.PassphraseChangedListener;
import org.alljoyn.config.server.RestartHandler;
import org.alljoyn.config.server.SetPasswordHandler;
import org.w3c.dom.DOMException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.UserDataHandler;



public class ConfigServerSample {

	
	   private static String configFile=null;
	
	
	
	
	    static { 
	        System.loadLibrary("alljoyn_java");
	    }

	    private static final short CONTACT_PORT=1000;
	    static BusAttachment mBus;
	    
	    static boolean sessionEstablished = false;
	    static int sessionId;

	    public static String m_keyStoreFileName;

		public static SrpKeyXListener m_authListener;
		
	

	    private static class MyBusListener extends BusListener {
	        public void nameOwnerChanged(String busName, String previousOwner, String newOwner) {
	            if ("com.my.well.known.name".equals(busName)) {
	                System.out.println("BusAttachement.nameOwnerChanged(" + busName + ", " + previousOwner + ", " + newOwner);
	            }
	        }
	    }
	    
	    
	    private static String mPassword="000000";
	
	  
	    
	
	    /*
	     * This Secure Remote Password Key eXchange listener creates a random 
	     * 6-digit pincode for each authentication attempt.   
	     */
	    private static class SrpKeyXListener implements AuthListener {
	      
	        private boolean mGeneratePassword;

	        public SrpKeyXListener() {
	          
	            mGeneratePassword = true;
	        }

	        public boolean requested(String mechanism, String peerName, int count,
	                String userName, AuthRequest[] requests) {
	            System.out.println(String.format("AuthListener.requested(%s, %s, %d, %s, %s);", 
	                    mechanism ,
	                    peerName,
	                    count,
	                    userName,
	                    AuthRequestToString(requests)));
	            /*
	            if (mGeneratePassword) {
	                mGeneratePassword = false;
	                mPassword = String.format("%06d", mPasswordGenerator.nextInt(1000000));
	                System.out.println("One Time Password : " + mPassword);
	            }*/
	            for (AuthRequest request : requests) {
	                if (request instanceof PasswordRequest) {
	                    ((PasswordRequest) request).setPassword(mPassword.toCharArray());
	                }
	            }
	            return true;
	        }

	        public void completed(String mechanism, String peerName, boolean authenticated) {
	            mGeneratePassword = true;
	        }

	        private String AuthRequestToString(AuthListener.AuthRequest[] requests) {
	            String str;
	            str = "[";
	            for (AuthListener.AuthRequest request : requests) {
	                if (request instanceof AuthListener.AuthRequest) {
	                    str += "AuthRequest ";
	                }
	                if (request instanceof AuthListener.CertificateRequest) {
	                    str += "CertificateRequest ";
	                }
	                if (request instanceof AuthListener.LogonEntryRequest) {
	                    str += "LogonEntryRequest ";
	                }
	                if (request instanceof AuthListener.PrivateKeyRequest) {
	                    str += "PrivateKeyRequest ";
	                }
	                if (request instanceof AuthListener.UserNameRequest) {
	                    str += "UserNameRequest ";
	                }
	                if (request instanceof AuthListener.PasswordRequest) {
	                    str += "PasswordRequest ";
	                }
	                if (request instanceof AuthListener.VerifyRequest) {
	                    str += "VerifyRequest ";
	                }
	            }
	            str += "]";
	            return str;
	        }
	    }
	    
	    
	    private static  class  PropertyStoreImpl implements PropertyStore, RestartHandler, ConfigChangeListener, FactoryResetHandler, PassphraseChangedListener,SetPasswordHandler{
	    	
	    	private Map<String,List<Property>> m_internalMap=null;
	    	private Map<String,List<Property>> m_internalBackUp=null;
	    	
	    	private String m_XmlOriginal;
	    	private String m_ConfigFile;
	    	
	    	private XPathFactory m_XPathFactory=null;
	    	
	    	
	    	public static class Property implements Cloneable 
	  	    {	    		
	  	    	private  String m_language=null;
	  	    	private final boolean m_isWritable;
	  	    	private final boolean m_isAnnounced;
	  	    	private final boolean m_isPublic;
	  	    	private final String m_name;
	  	    	private Object m_object=null;
	  	      // public.write.announce
	  	    	
	  	    	
	  	    	public Property(String m_name,Object value, boolean isPublic,boolean isWritable,boolean isAnnounced)
	  	    	{
	  	    		super();
	  	    		this.m_isWritable = isWritable;
	  	    		this.m_isAnnounced = isAnnounced;
	  	    		this.m_isPublic = isPublic;
	  	    		this.m_name = m_name;
	  	    		this.m_object=value;
	  	    	}
	  	    	
	  	    	public boolean isWritable()
	  	    	{
	  	    		return m_isWritable;
	  	    	}
	  	    	public boolean isAnnounced()
	  	    	{
	  	    		return m_isAnnounced;
	  	    	}
	  	    	public boolean isPublic()
	  	    	{
	  	    		return m_isPublic;
	  	    	}
	  	    	public String getName()
	  	    	{
	  	    		return m_name;
	  	    	}
	  	    	
	  	    	public String getLangauge()
	  	    	{
	  	    		return m_language;
	  	    	}	    	
	  	    	public void setLanguage(String language ) { this.m_language = language; }	 
	  	    	
	  	    	
	  	    	public void setValue(Object o){
	  	    		m_object=o;
	  	    	}
	  	    	public Object getObject() {
	  	    		return m_object;
	  	    	}
	  	    	
	  	    	public Object clone() throws CloneNotSupportedException {
	  	    	    return super.clone();
	  	    	  }
	  	    	
	  	    	public String toString(){
	  	    		return new String("name="+m_name+
	  	    						  " pubic="+m_isPublic+ 
	  	    						  " writable="+m_isWritable+
	  	    						  " announcable"+ m_isAnnounced+ 
	  	    						  " lanaguge="+m_language+
	  	    						  " object="+m_object	  	    						  
	  	    						  );
	  	    	}
	  	    }
	    	
	    	
	    	
	    	
	    	
	    	@Override
	    	public void setPassword(String peerName, char[] password){
	    		System.out.println("setPassword called");
				System.out.println("peerName [" +peerName+"] + password ["+new String(password)+"]" );
				mPassword=new String(password);		
				try{
					setPasswordToPersistance(peerName,mPassword);
				}catch(Exception e){
					e.printStackTrace();
				}
				// need to disconnect the bus so this can take effect 
	    	}
	    	
	    	
	    	@Override
			public void onPassphraseChanged(byte[] passphrase){
				System.out.println("onPassphraseChanged called");
				
			}
			@Override
			public void doFactoryReset(){
				System.out.println("doFactoryReset called");
				
				try{			    
			    	
			    	 Writer writer = new OutputStreamWriter(
		                       new FileOutputStream(m_ConfigFile), "UTF-8");
			    	 BufferedWriter fout = new BufferedWriter(writer);
			    	 fout.write(m_XmlOriginal);			    	
			         fout.close();
			       // copy from m_internalBackUp  to m_internalMap
			          Set<Map.Entry<String, List<Property>>> entries = m_internalBackUp.entrySet();			         
			          m_internalMap.clear();			         	
					  for(Map.Entry<String, List<Property>> entry : entries) {
						 String key = entry.getKey();
						 List<Property> src = entry.getValue();
						 List<Property> dst=new ArrayList<Property>();
						 for (int i=0;i<src.size();i++)
						 {
							 try{							 
								 dst.add((Property)src.get(i).clone());							 
							 }catch (Exception e){
								 
							 }
						 }
						 m_internalMap.put(key, dst);				 
				      }	  
			         
			         
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}
			    	
				
				
			}
			@Override
			public void onConfigChanged(Map<String, Variant> arg0, String arg1){
				System.out.println("onConfigChanged called");
			}
			@Override
			public void restart(){
				System.out.println("restart called");					
			}
			
			@Override
			public void onResetConfiguration(String language, String[] fieldsToRemove) {				
				System.out.println("onResetConfiguration called");	
				
				
				System.out.print("Language ["+ language+ "] fieldsToReset [ ");
				for (int i=0;i<fieldsToRemove.length;i++){
					System.out.print(fieldsToRemove[i]+" ");
				}
				System.out.print("]\n");	
				
			}
	    	
	    	
	    	
	    	  
	    
	    	public PropertyStoreImpl(Map<String,List<Property>>   dataMap ,String configFile)
	    	{		    		
	    		try
	    		{
	    			
	    			m_XPathFactory=XPathFactory.newInstance();
	    			m_ConfigFile=configFile;
	    			m_XmlOriginal=readConfigFile(configFile);
	    			
	    			mPassword=readPasswordFromPersistance();
	    		}
	    		catch (Exception e)
	    		{
	    			e.printStackTrace();
	    		}
	    		
	    		m_internalBackUp=new HashMap<String, List<Property>>();
	    		 Set<Map.Entry<String, List<Property>>> entries = dataMap.entrySet();	    				
				  for(Map.Entry<String, List<Property>> entry : entries) {
					 String key = entry.getKey();
					 List<Property> src = entry.getValue();
					 List<Property> dst=new ArrayList<Property>();
					 for (int i=0;i<src.size();i++)
					 {
						 try{							 
							 dst.add((Property)src.get(i).clone());							 
						 }catch (Exception e){
							 
						 }
					 }
					 m_internalBackUp.put(key, dst);				 
			      }	  
				  m_internalMap=dataMap;	    	
	    	}
	    	
	    	
	    	public void readAll(String languageTag, Filter filter, Map<String, Object> dataMap) throws PropertyStoreException{
	    		if (filter==Filter.ANNOUNCE)
	    		{	    		
	    			
	    			if (m_internalMap!=null)
	    			{		    			
	    				  List<Property> langauge=m_internalMap.get(AboutKeys.ABOUT_DEFAULT_LANGUAGE);
	    				  if (langauge!=null)
	    				  {
	    					  languageTag=(String)langauge.get(0).getObject();
	    				  }else{
	    					  throw new PropertyStoreException(PropertyStoreException.UNSUPPORTED_LANGUAGE);
	    				  }
	    				  	    				
	    				  Set<Map.Entry<String, List<Property>>> entries = m_internalMap.entrySet();	    				
	    				  for(Map.Entry<String, List<Property>> entry : entries) {
	    					  	String key = entry.getKey();
	    					  	List<Property> properyList = entry.getValue();
	    					  	for (int i=0;i<properyList.size();i++)
	    					  	{
	    					  		Property property=properyList.get(i);
	    					  		if (!property.isAnnounced())
	    					  			continue;
	    					  		 if (!(property.getLangauge()==null|| property.getLangauge().compareTo(languageTag) == 0))
	    				                continue;
	    					  		dataMap.put(key, property.getObject());	    					  		
	    					  	}	    					  	
	    			      }	    					    				
	    			}else 
	    				throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_KEY); 
	    			
	    			
	    		}
	    		else if (filter==Filter.READ)
	    		{	    			
	    			if (languageTag!=null && languageTag.length()>1)
	    			{
		    			 List<Property> supportedLanguages=m_internalMap.get(AboutKeys.ABOUT_SUPPORTED_LANGUAGES);		    			
		    			 if (supportedLanguages==null)
		    					throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_KEY); 
		    			if (!( supportedLanguages.get(0).getObject() instanceof Set<?>)){	    						
							throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_LANGUAGE);
						}else{
							@SuppressWarnings("unchecked")
							Set<String> languages=(Set<String>)supportedLanguages.get(0).getObject();
							if (!languages.contains(languageTag)){
								throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_LANGUAGE);
							}								
						}
	    			}else{
	    				
	    				 List<Property> langauge=m_internalMap.get(AboutKeys.ABOUT_DEFAULT_LANGUAGE);
	    				  if (langauge!=null)
	    				  {
	    					  languageTag=(String)langauge.get(0).getObject();
	    				  }else{
	    					  throw new PropertyStoreException(PropertyStoreException.UNSUPPORTED_LANGUAGE);
	    				  }
	    			}	    			 	    			
	    		  Set<Map.Entry<String, List<Property>>> entries = m_internalMap.entrySet();    				
  				  for(Map.Entry<String, List<Property>> entry : entries) {
  					  	String key = entry.getKey();
  					  	List<Property> properyList = entry.getValue();
  					  	for (int i=0;i<properyList.size();i++)
  					  	{
  					  		Property property=properyList.get(i);
  					  		if (!property.isPublic())
  					  			continue;
  					  		 if (!(property.getLangauge()==null|| property.getLangauge().compareTo(languageTag) == 0))
  				                continue;
  					  		dataMap.put(key, property.getObject());	    					  		
  					  	}	    					  	
  			      }
	    			
	    		}//end of read.
	    		
	    		else if (filter==Filter.WRITE)
	    		{
	    			if (languageTag!=null && languageTag.length()>1)
	    			{
		    			 List<Property> supportedLanguages=m_internalMap.get(AboutKeys.ABOUT_SUPPORTED_LANGUAGES);		    			
		    			 if (supportedLanguages==null)
		    					throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_KEY); 
		    			if (!( supportedLanguages.get(0).getObject() instanceof Set<?>)){	    						
							throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_LANGUAGE);
						}else{
							@SuppressWarnings("unchecked")
							Set<String> languages=(Set<String>)supportedLanguages.get(0).getObject();
							if (!languages.contains(languageTag)){
								throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_LANGUAGE);
							}								
						}
	    			}else{
	    				throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_LANGUAGE);
	    			}
	    			
	    			Set<Map.Entry<String, List<Property>>> entries = m_internalMap.entrySet();    				
	  				  for(Map.Entry<String, List<Property>> entry : entries) {
	  					  	String key = entry.getKey();
	  					  	List<Property> properyList = entry.getValue();
	  					  	for (int i=0;i<properyList.size();i++)
	  					  	{
	  					  		Property property=properyList.get(i);
	  					  		if (!property.isWritable())
	  					  			continue;
	  					  		 if (!(property.getLangauge()==null|| property.getLangauge().compareTo(languageTag) == 0))
	  				                continue;
	  					  		dataMap.put(key, property.getObject());	    					  		
	  					  	}	    					  	
	  			      }	    				    		
	    		}
	    			    	
	    		else throw new PropertyStoreException(PropertyStoreException.ILLEGAL_ACCESS);	    	
	    		
	    	}
	    	
	    	public void update(String key, String languageTag, Object newValue)  throws PropertyStoreException{
	    		
	    		System.out.println("update ["+key+"] language ["+languageTag +"] value ["+newValue+"]");
	    		if (key.equals(AboutKeys.ABOUT_DEFAULT_LANGUAGE) ||  key.equals(AboutKeys.ABOUT_DEVICE_NAME))
	    		{	    		  
	    	            languageTag = null;
	    		}
	    		
	    		if (languageTag!=null)
    			{
	    			 List<Property> supportedLanguages=m_internalMap.get(AboutKeys.ABOUT_SUPPORTED_LANGUAGES);		    			
	    			 if (supportedLanguages==null)
	    					throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_KEY); 
	    			if (!( supportedLanguages.get(0).getObject() instanceof Set<?>)){	    						
						throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_LANGUAGE);
					}else{
						@SuppressWarnings("unchecked")
						Set<String> languages=(Set<String>)supportedLanguages.get(0).getObject();
						if (!languages.contains(languageTag)){
							throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_LANGUAGE);
						}								
					}
    			}
	    		List<Property> keyList= m_internalMap.get(key);
	    		
	    		if ( keyList==null){
	    			throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_KEY);	    			    	
	    		}
	    		boolean needUpdate=false;
	    		for (int i=0;i<keyList.size();i++){
	    			Property p=keyList.get(i);
	    			if (!p.isWritable()){
	    				throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_KEY);	    				
	    			}		 
	    			if (languageTag==null)
	    			{
	    				p.setValue(newValue);
	    				keyList.remove(i);
	    				keyList.add(p);	    				
	    				m_internalMap.put(key, keyList);
	    				needUpdate=true;
	    				break;
	    			}else if (p.getLangauge().equals(languageTag)){
	    				p.setValue(newValue);
	    				keyList.remove(i);
	    				keyList.add(p);	    				
	    				m_internalMap.put(key, keyList);	
	    				needUpdate=true;
	    				break;
	    			}	    				   		
	    		}// persist to file!!!
	    		
	    		try{
	    			if (needUpdate){
	    				persistUpdate(key,languageTag,newValue.toString());
	    			}
	    		}catch (Exception e){
	    			throw new PropertyStoreException(PropertyStoreException.UNSUPPORTED_KEY);
	    		}
	    		
	    		

	    	}

	    	public void reset(String key, String languageTag) throws PropertyStoreException{
	    			    	
	    		
	    		System.out.println("reset key["+key+"] language ["+languageTag+"]" );
	    		if (key.equals(AboutKeys.ABOUT_DEFAULT_LANGUAGE) ||  key.equals(AboutKeys.ABOUT_DEVICE_NAME))
	    		{	    		  
	    	            languageTag = null;
	    		}
	    		
	    		if (languageTag!=null )
    			{
	    			 List<Property> supportedLanguages=m_internalMap.get(AboutKeys.ABOUT_SUPPORTED_LANGUAGES);		    			
	    			 if (supportedLanguages==null)
	    					throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_KEY); 
	    			if (!( supportedLanguages.get(0).getObject() instanceof Set<?>)){	    						
						throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_LANGUAGE);
					}else{
						@SuppressWarnings("unchecked")
						Set<String> languages=(Set<String>)supportedLanguages.get(0).getObject();
						if (!languages.contains(languageTag)){
							throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_LANGUAGE);
						}								
					}
    			}
	    		
	    		List<Property> keyList= m_internalMap.get(key);
	    		
	    		
	    		if ( keyList==null){
	    			throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_KEY);	    			    	
	    		}
	    			    		
	    		boolean needUpdate=false;
	    		Object value=null;
	    		
	    		for (int i=0;i<keyList.size();i++){
	    			Property p=keyList.get(i);
	    			if (!p.isWritable()){
	    				throw new  PropertyStoreException(PropertyStoreException.UNSUPPORTED_KEY);	    				
	    			}		 
	    			if (languageTag==null)
	    			{	    					    				
	    				keyList.remove(i);	    				
	    				List<Property> keyListBackUp=m_internalBackUp.get(key);	    			
	    				keyList.add(m_internalBackUp.get(key).get(0));	    
	    				value=m_internalBackUp.get(key).get(0).getObject();
	    				m_internalMap.put(key, keyList);
	    				break;
	    			}else if (p.getLangauge().equals(languageTag)){
	    				keyList.remove(i);
	    				List<Property> keyListBackUp=m_internalBackUp.get(key);	    				
	    				for (int x=0;x<keyListBackUp.size();x++){
    						if (keyListBackUp.get(x).getLangauge().equals(languageTag))
    						{	    							
    							keyList.add(keyListBackUp.get(x));	
    							value=keyListBackUp.get(x).getObject();
    		    				m_internalMap.put(key, keyList);
    							break;
    						}
	    				}	    					    					
	    				break;
	    			}	    				   		
	    		}// persist to file!!!
	    		
	    		try{
	    			if (needUpdate){
	    				persistUpdate(key,languageTag,(String)value);
	    			}
	    		}catch (Exception e){
	    			throw new PropertyStoreException(PropertyStoreException.UNSUPPORTED_KEY);
	    		}
	    		
	    		
	    		
	    	}
	    	
	    	public void resetAll() throws PropertyStoreException{
	    		
	    	}
	    	
	    	void setPasswordToPersistance(String daemonrealm,String password) throws Exception
	    	{ 
	    		try{
		    		  FileInputStream file = new FileInputStream(new File(m_ConfigFile));
	                
		              DocumentBuilderFactory builderFactory = DocumentBuilderFactory.newInstance();
		               
		              DocumentBuilder builder =  builderFactory.newDocumentBuilder();
		               
		              Document xmlDocument = builder.parse(file);
		              
		              XPath xPath= m_XPathFactory.newXPath();
		              {
			              StringBuilder strCpathExpresionKey=new StringBuilder();
			              StringBuilder strCpathExpresionValue=new StringBuilder();		             
			              strCpathExpresionKey.append("//config/keys/key [@name='").append("passcode").append("']");
			              strCpathExpresionValue.append(strCpathExpresionKey).append(" //value");
			                           
			              NodeList nodeList = (NodeList) xPath.compile(strCpathExpresionValue.toString()).evaluate(xmlDocument, XPathConstants.NODESET);
			              
			              if (nodeList.getLength()>0)
			              {
			            	 nodeList.item(0).getFirstChild().setNodeValue(password);
			              }	
			              
		              }
		              
		            
		              {
			              StringBuilder strCpathExpresionKey=new StringBuilder();
			              StringBuilder strCpathExpresionValue=new StringBuilder();		             
			              strCpathExpresionKey.append("//config/keys/key [@name='").append("daemonrealm").append("']");
			              strCpathExpresionValue.append(strCpathExpresionKey).append(" //value");
			                           
			              NodeList nodeList = (NodeList) xPath.compile(strCpathExpresionValue.toString()).evaluate(xmlDocument, XPathConstants.NODESET);
			              
			              if (nodeList.getLength()>0)
			              {
			            	  Node n=nodeList.item(0);
			            	  if (n.hasChildNodes())
			            	  {
			            		n.getFirstChild().setNodeValue(daemonrealm);  
			            	  }else{			            		 			            		 
			            		n.appendChild( (Node)xmlDocument.createTextNode(daemonrealm));		            		
			            	  }
			              }
			              
		              }
		              
		          	TransformerFactory transformerFactory = TransformerFactory.newInstance();
		      		Transformer transformer = transformerFactory.newTransformer();
		      		DOMSource source = new DOMSource(xmlDocument);
		      		StreamResult result = new StreamResult(new File(m_ConfigFile));		     
		      		transformer.transform(source, result);
	    		}
    		  catch (Exception e)
    		  {
    			 throw e;
    		  }
              
	              
	              
	              
	    	}
	    	
	        String readPasswordFromPersistance()
	        {
	        	 String pass=null;
	        	 try
	    		  {		    		  
	    			  FileInputStream file = new FileInputStream(new File(m_ConfigFile));
		                 
		              DocumentBuilderFactory builderFactory = DocumentBuilderFactory.newInstance();
		               
		              DocumentBuilder builder =  builderFactory.newDocumentBuilder();
		               
		              Document xmlDocument = builder.parse(file);
		              
		              XPath xPath= m_XPathFactory.newXPath();

		              StringBuilder strCpathExpresionKey=new StringBuilder();
		              StringBuilder strCpathExpresionValue=new StringBuilder();		             
		              strCpathExpresionKey.append("//config/keys/key [@name='").append("passcode").append("']");
		              strCpathExpresionValue.append(strCpathExpresionKey).append(" //value");
		                           
		              NodeList nodeList = (NodeList) xPath.compile(strCpathExpresionValue.toString()).evaluate(xmlDocument, XPathConstants.NODESET);
		              
		              if (nodeList.getLength()>0)
		              {
		            	  pass= nodeList.item(0).getFirstChild().getNodeValue();  
		              }		            
	    		  }
	        	  catch (Exception e)
	    		  {
	    			 e.printStackTrace();
	    		  }
	        	 return pass;
	        
	        }
	    	
	    	 public void  persistUpdate(String key, String language, String value) throws Exception
	    	 {
	    		  try
	    		  {		    		  
	    			  FileInputStream file = new FileInputStream(new File(m_ConfigFile));
		                 
		              DocumentBuilderFactory builderFactory = DocumentBuilderFactory.newInstance();
		               
		              DocumentBuilder builder =  builderFactory.newDocumentBuilder();
		               
		              Document xmlDocument = builder.parse(file);
		              
		              XPath xPath= m_XPathFactory.newXPath();

		              StringBuilder strCpathExpresionKey=new StringBuilder();
		              StringBuilder strCpathExpresionValue=new StringBuilder();		             
		              strCpathExpresionKey.append("//config/keys/key [@name='").append(key).append("']");

		              strCpathExpresionValue.append(strCpathExpresionKey).append(" //value");
		              if (language != null) {
		                  strCpathExpresionValue.append("[@language='").append(language).append("']");
		              }
		              		              
		              NodeList nodeList = (NodeList) xPath.compile(strCpathExpresionValue.toString()).evaluate(xmlDocument, XPathConstants.NODESET);
		              
		              if (nodeList.getLength()>0)
		              {
		            		nodeList.item(0).getFirstChild().setNodeValue(value);  
		              }
			          
	        
		      		TransformerFactory transformerFactory = TransformerFactory.newInstance();
		      		Transformer transformer = transformerFactory.newTransformer();
		      		DOMSource source = new DOMSource(xmlDocument);
		      		StreamResult result = new StreamResult(new File(m_ConfigFile));		     
		      		transformer.transform(source, result);
		              
		              
		              
		          }
	    		  catch (Exception e)
	    		  {
	    			 throw e;
	    		  }
	    		
	    	 }
	    	
	    	
	    }
	    
	    
	    private static String readConfigFile(String fileName) throws UnsupportedEncodingException,IOException,Exception{
	    	StringBuilder retStr=new StringBuilder();
	    	try{
	    	String str;
	    	File fileDir = new File(fileName);	    	 
			BufferedReader in = new BufferedReader(
			   new InputStreamReader(
	                      new FileInputStream(fileDir), "UTF8"));
			while ((str =in.readLine()) != null) {
				retStr.append(str);
			}
	        in.close();	      
	    	}
	    	catch (UnsupportedEncodingException e) 
	  	    {
	  			throw new UnsupportedEncodingException();
	  	    } 
	  	    catch (IOException e) 
	  	    {
	  	    	throw new IOException(e);
	  	    }
	  	    catch (Exception e)
	  	    {
	  	    	throw new Exception(e);
	  	    }
	    	return retStr.toString();
	    }
	   
	    
	    
	    public static void main(String[] args) {
	    	
	    	class Triple{
	    		public Triple(String aKey,String aValue,String aLanguage){
	    			key=aKey;
	    			value=aValue;
	    			lanaguage=aLanguage;
	    		}
	    		
	    		public String key;
	    		public String value;
	    		public String lanaguage;
	    	}
	    	
	        Runtime.getRuntime().addShutdownHook(new Thread() {
				public void run() {
					mBus.release();
				}
			});
	       
	        Map<String, List<PropertyStoreImpl.Property>> data=new HashMap<String, List<PropertyStoreImpl.Property>>();	
	        
	        
	        if (args.length>0)
	        {
	        	for (int i=0;i<args.length;i++){
	        		String currentArg=args[i];
	        		System.out.println(currentArg);
	        		if (currentArg.startsWith("--config-file"))
	        		{
	        			configFile=currentArg.substring("--config-file".length()+1);
	        			try
	        			{
	        				String xmlData=readConfigFile(configFile);
	        					        					        			
	        				DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
	        				factory.setIgnoringElementContentWhitespace(true);	        				
	        				//factory.setValidating(true);
	        				factory.setIgnoringComments(true);
	        				
	        		        DocumentBuilder builder = factory.newDocumentBuilder();
	        		     
	        		        Document doc=builder.parse(new ByteArrayInputStream(xmlData.getBytes()));
	        		        doc.getDocumentElement().normalize();
	        		        	        		
	        		        
	        		        
	        		    	NodeList nList = doc.getElementsByTagName("key");
	        		    	for (int temp = 0; temp < nList.getLength(); temp++) {	        		    		 
	        		    		Node nNode = nList.item(temp);	        		     
	        		    		List <Triple> list=new ArrayList<Triple>();	        		     
	        		    		if (nNode.getNodeType() == Node.ELEMENT_NODE) {	      	        		    			
	        		    			Element eElement = (Element) nNode;	        		    				        		    			
	        		    			String keyname=eElement.getAttribute("name");	 	        	        		    				        		    			
	        		    			NodeList valueList = eElement.getElementsByTagName("value");		        		    				        		    			
	        		    			for (int t = 0; t < valueList.getLength(); t++) {	        		    				        		    			        		    				
	        		    				Node valueNode = valueList.item(t);
	        		    				if (nNode.getNodeType() == Node.ELEMENT_NODE) 
	        		    				{	        		    					
	        		    					Element elementValue = (Element) valueNode;	        		 
	        		    					String languge=elementValue.getAttribute("language");
	        		    					String value=null;
	        		    					
	        		    					System.out.println("Lanaguage "+elementValue.getAttribute("language"));
	        		    					if (elementValue.getChildNodes().getLength()>0)
	        		    					{
	        		    						value=elementValue.getFirstChild().getNodeValue();
	        		    					}
	        		    					list.add(new Triple(keyname,value,languge));
	        		    				}
	        		    			}//end of fr
	        		    				        		    				        		    			
	        		    			
    		    					if (keyname.equals(AboutKeys.ABOUT_DEFAULT_LANGUAGE))
    		    					{    		    						
    		    						 Triple t=list.get(0);
    		    						 data.put(AboutKeys.ABOUT_DEFAULT_LANGUAGE, new ArrayList<PropertyStoreImpl.Property>(
    		    				        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_DEFAULT_LANGUAGE,t.value,true,true,true))));	
    		    					}
    		    					
    		    					else if (keyname.equals(AboutKeys.ABOUT_DEVICE_NAME))
    		    					{
    		    						 Triple t=list.get(0);
    		    						  data.put(AboutKeys.ABOUT_DEVICE_NAME, new ArrayList<PropertyStoreImpl.Property>(
    		    				        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_DEVICE_NAME,t.value,true,true,true))));	  
    		    					}
    		    					
    		    					else if (keyname.equals(AboutKeys.ABOUT_DEVICE_ID))
    		    					{
    		    						Triple t=list.get(0);
    		    						 data.put(AboutKeys.ABOUT_DEVICE_ID, new ArrayList<PropertyStoreImpl.Property>(
    		    				        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_DEVICE_ID,t.value,true,false,true))));
    		    					}
    		    					
    		    					else if (keyname.equals(AboutKeys.ABOUT_DESCRIPTION))
    		    					{
    		    						
    		    						List<PropertyStoreImpl.Property> newData=new ArrayList<PropertyStoreImpl.Property>();
    		    						for (int x=0;x<list.size();x++)
    		    						{
    		    							PropertyStoreImpl.Property zz=new PropertyStoreImpl.Property(AboutKeys.ABOUT_DESCRIPTION,list.get(x).value,true,false,false);
    		    							if (list.get(x).lanaguage!=null){
    		    									zz.setLanguage(list.get(x).lanaguage);
    		    							}
    		    							newData.add(zz);
    		    						}
    		    						data.put(AboutKeys.ABOUT_DESCRIPTION,newData);
    		    					}	        		    			
	        		    		}
	        		    	}//end of for
	        		        	        		   	        				
	        			}catch (Exception e){
	        				e.printStackTrace();
	        				System.out.println("Unable to read config file");	        			
	        				System.exit(1);
	        			}	        			
	        		}	        		
	        	}	        	
	        }else{	        	
	        	System.out.println("No Config File found Terminate");
	        	System.exit(1);	        	
	        }
	        
	        
	       
	        
	      
	      
	        mBus = new BusAttachment("AppName", BusAttachment.RemoteMessage.Receive);
	        
	        BusListener myBusListener = new MyBusListener();
	        mBus.registerBusListener(myBusListener);
	        
	        Status status = mBus.connect();
	        if (status != Status.OK) {
	            System.exit(0);
	            return;
	        }
	       
	        if (data.size()==0){
	         data.put(AboutKeys.ABOUT_DEFAULT_LANGUAGE, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_DEFAULT_LANGUAGE,"en",true,true,true))));	        	 
	         
	         data.put(AboutKeys.ABOUT_DEVICE_NAME, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_DEVICE_NAME,"MyDeviceName",true,true,true))));	   
	         
	         data.put(AboutKeys.ABOUT_DEVICE_ID, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_DEVICE_ID,"1231232145667745675477",true,false,true))));
	         
	         data.put(AboutKeys.ABOUT_DESCRIPTION, new ArrayList<PropertyStoreImpl.Property>(
	        		 Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_DESCRIPTION,"This is car",true,false,false))));   	 	         
	        }
	        
	        
	         final UUID uid = UUID.fromString("38400000-8cf0-11bd-b23e-10b96e4ef00a");  
	         data.put(AboutKeys.ABOUT_APP_ID, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_APP_ID,uid,true,false,true))));    
	        
	         data.put(AboutKeys.ABOUT_APP_NAME, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_APP_NAME,"AboutConfig",true,false,true))));      
	         
	         data.put(AboutKeys.ABOUT_MANUFACTURER, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_MANUFACTURER,"Company",true,false,true))));   	    
	         	         
	         data.put(AboutKeys.ABOUT_MODEL_NUMBER, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_MODEL_NUMBER,"Wxfy388i",true,false,true))));   	    
	
	         data.put(AboutKeys.ABOUT_SUPPORTED_LANGUAGES, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_SUPPORTED_LANGUAGES,new HashSet <String>(){{add("en");add("sp");add("ru");}},true,false,true))));   	    
	   	         
	         data.put(AboutKeys.ABOUT_DATE_OF_MANUFACTURE, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_DATE_OF_MANUFACTURE,"10/1/2199",true,false,true)))); 	        	   	        	    
	         	         
	         data.put(AboutKeys.ABOUT_SOFTWARE_VERSION, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_SOFTWARE_VERSION,"12.20.44 build 44454",true,false,true))));     	   	        	    
	         	         
	         data.put(AboutKeys.ABOUT_AJ_SOFTWARE_VERSION, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_AJ_SOFTWARE_VERSION,"3.3.2",true,false,true))));          	   	        	    
	         	        
	         data.put(AboutKeys.ABOUT_HARDWARE_VERSION, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_HARDWARE_VERSION,"355.499. b",true,false,true))));         	   	        	    
	  
	         data.put(AboutKeys.ABOUT_SUPPORT_URL, new ArrayList<PropertyStoreImpl.Property>(
	        		  Arrays.asList(new PropertyStoreImpl.Property(AboutKeys.ABOUT_SUPPORT_URL,"http://www.alljoyn.org",true,false,true))));         	   	        	    
	     
	     
	         PropertyStoreImpl propertyStoreImpl=new PropertyStoreImpl(data,configFile);
	        
	         
	      
	         try{
	        	 AboutServiceImpl.getInstance().startAboutServer((short)CONTACT_PORT, (PropertyStore)propertyStoreImpl, mBus);	 
	         }catch (Exception e){
	        	 e.printStackTrace();
	         }	       	        

	         
	         try{
	            	
	            	
					
					byte aboutIconContent[] = new byte[]  {   (byte)0x89, (byte)0x50, (byte)0x4E, (byte)0x47, (byte)0x0D, (byte)0x0A, (byte)0x1A, (byte)0x0A,
								                              (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x0D, (byte)0x49, (byte)0x48, (byte)0x44, (byte)0x52,
								                              (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x0A, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x0A,
								                              (byte)0x08, (byte)0x02, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x02, (byte)0x50, (byte)0x58,
								                              (byte)0xEA, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x04, (byte)0x67, (byte)0x41, (byte)0x4D,
								                              (byte)0x41, (byte)0x00, (byte)0x00, (byte)0xAF, (byte)0xC8, (byte)0x37, (byte)0x05, (byte)0x8A,
								                              (byte)0xE9, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x19, (byte)0x74, (byte)0x45, (byte)0x58,
								                              (byte)0x74, (byte)0x53, (byte)0x6F, (byte)0x66, (byte)0x74, (byte)0x77, (byte)0x61, (byte)0x72,
								                              (byte)0x65, (byte)0x00, (byte)0x41, (byte)0x64, (byte)0x6F, (byte)0x62, (byte)0x65, (byte)0x20,
								                              (byte)0x49, (byte)0x6D, (byte)0x61, (byte)0x67, (byte)0x65, (byte)0x52, (byte)0x65, (byte)0x61,
								                              (byte)0x64, (byte)0x79, (byte)0x71, (byte)0xC9, (byte)0x65, (byte)0x3C, (byte)0x00, (byte)0x00,
								                              (byte)0x00, (byte)0x18, (byte)0x49, (byte)0x44, (byte)0x41, (byte)0x54, (byte)0x78, (byte)0xDA,
								                              (byte)0x62, (byte)0xFC, (byte)0x3F, (byte)0x95, (byte)0x9F, (byte)0x01, (byte)0x37, (byte)0x60,
								                              (byte)0x62, (byte)0xC0, (byte)0x0B, (byte)0x46, (byte)0xAA, (byte)0x34, (byte)0x40, (byte)0x80,
								                              (byte)0x01, (byte)0x00, (byte)0x06, (byte)0x7C, (byte)0x01, (byte)0xB7, (byte)0xED, (byte)0x4B,
								                              (byte)0x53, (byte)0x2C, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x49, (byte)0x45,
								                              (byte)0x4E, (byte)0x44, (byte)0xAE, (byte)0x42, (byte)0x60, (byte)0x82 };

					
	            		AboutServiceImpl.getInstance().registerIcon("image/png", 
	            			"http://tinyurl.com/m3ra78j", 
	            			aboutIconContent);
	            }
	            catch (Exception e) 
	            {
	                   e.printStackTrace();
	            }
	         
	         try
	         {
	        		//m_keyStoreFileName = getFileStreamPath("alljoyn_keystore").getAbsolutePath();
	    			m_authListener = new SrpKeyXListener();
	    			Status authStatus = mBus.registerAuthListener("ALLJOYN_SRP_KEYX", m_authListener);
	    			
	    			// Add configService to About announcement. 	              
	    			AboutServiceImpl.getInstance().addObjectDescription(ConfigTransport.OBJ_PATH, new String[] {ConfigTransport.INTERFACE_NAME});
	    			ConfigServiceImpl.getInstance().startConfigServer((PropertyStore)propertyStoreImpl, propertyStoreImpl, propertyStoreImpl, propertyStoreImpl, propertyStoreImpl, mBus);
	    			
	    			ConfigServiceImpl.getInstance().setSetPasswordHandler(propertyStoreImpl);
	          }
	         catch (Exception e)
	         {
	        	 e.printStackTrace();
	         }
	         
	         
	        Mutable.ShortValue contactPort = new Mutable.ShortValue(CONTACT_PORT);

	        SessionOpts sessionOpts = new SessionOpts();
	        sessionOpts.traffic = SessionOpts.TRAFFIC_MESSAGES;
	        sessionOpts.isMultipoint = false;
	        sessionOpts.proximity = SessionOpts.PROXIMITY_ANY;
	        sessionOpts.transports = SessionOpts.TRANSPORT_ANY;

	        status = mBus.bindSessionPort(contactPort, sessionOpts, 
	                new SessionPortListener() {
	            public boolean acceptSessionJoiner(short sessionPort, String joiner, SessionOpts sessionOpts) {
	                System.out.println("SessionPortListener.acceptSessionJoiner called");
	                if (sessionPort == CONTACT_PORT) {
	                    return true;
	                } else {
	                    return false;
	                }
	            }
	            public void sessionJoined(short sessionPort, int id, String joiner) {
	                System.out.println(String.format("SessionPortListener.sessionJoined(%d, %d, %s)", sessionPort, id, joiner));
	                sessionId = id;
	                sessionEstablished = true;
	            }
	        });
	        
	        
	        if (mBus.isConnected() && mBus.getUniqueName().length() > 0) {	        	
	        		status=mBus.advertiseName( mBus.getUniqueName(), SessionOpts.TRANSPORT_ANY);
	        		 if (status != Status.OK) {
	     	            System.out.println("Status = " + status);
	     	            mBus.releaseName("com.my.well.known.name");
	     	            System.exit(0);
	     	            return;
	     	        }
	        }
	        
	        
	        
	        System.out.println("BusAttachment session established");
	        AboutServiceImpl.getInstance().announce();
	        while (true) {
	            try {
	                Thread.sleep(10000);
	            } catch (InterruptedException e) {
	                System.out.println("Thead Exception caught");
	                e.printStackTrace();
	            }
	        }
	        
			
	      
	        
	    }// end of main
	    
}


	
	
