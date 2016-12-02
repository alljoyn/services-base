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

#ifndef ALLJOYN_CONFIG_OBJC_AJCFGCONFIGSERVICELISTENERADAPTER_H
#define ALLJOYN_CONFIG_OBJC_AJCFGCONFIGSERVICELISTENERADAPTER_H

#import <alljoyn/config/ConfigService.h>
#import "AJCFGConfigServiceListener.h"
#import "alljoyn/about/AJNConvertUtil.h"

/**
 AJCFGConfigServiceListenerImplAdapter enable bind the C++ ConfigService Listener API with an objective-c ConfigService listener
 */
class AJCFGConfigServiceListenerImplAdapter : public ajn::services::ConfigService::Listener {
public:
	AJCFGConfigServiceListenerImplAdapter(id <AJCFGConfigServiceListener> configServiceListener)
	{
		qcsConfigServiceListener = configServiceListener;
	}
	/**
     Restart of the device.
     @return ER_OK if successful.
	 */
	virtual QStatus Restart()
	{
		return [qcsConfigServiceListener restart];
	}
    
	/**
     Factory reset of the device ,return to default values including password!
     @return ER_OK if successful.
	 */
	virtual QStatus FactoryReset()
	{
		return [qcsConfigServiceListener factoryReset];
	}
    
	/**
     Receive Passphrase info and persist it.
     @param daemonRealm Daemon realm to persist.
     @param passcodeSize passcode size.
     @param passcode passcode content.
     @return ER_OK if successful.
	 */
	virtual QStatus SetPassphrase(const char *daemonRealm, size_t passcodeSize, const char *passcode, ajn::SessionId sessionId)
	{
		NSString *passphrase = [[NSString alloc] initWithBytes:passcode length:passcodeSize encoding:NSASCIIStringEncoding];
        
		return [qcsConfigServiceListener setPassphrase:[AJNConvertUtil convertConstCharToNSString:daemonRealm] withPasscode:passphrase];
	}

    
	virtual ~AJCFGConfigServiceListenerImplAdapter()
	{
	}
    
private:
	id <AJCFGConfigServiceListener> qcsConfigServiceListener;
};

#endif //ALLJOYN_CONFIG_OBJC_AJCFGCONFIGSERVICELISTENERADAPTER_H