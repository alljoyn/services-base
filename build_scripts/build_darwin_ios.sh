#!/bin/bash -ex
# # 
#    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
#    Source Project Contributors and others.
#    
#    All rights reserved. This program and the accompanying materials are
#    made available under the terms of the Apache License, Version 2.0
#    which accompanies this distribution, and is available at
#    http://www.apache.org/licenses/LICENSE-2.0

#

##
## Build services
##
#CONFIGURATION_BUILD_DIR=core/alljoyn/build
exit_status=0

BASE_SERVICE=$1 
VARIANT=$2
WORKSPACE=${WORKSPACE:-$ALLJOYN_ROOT_DIR}

case $BASE_SERVICE in
    controlpanel) SERVICES=(ControlPanel)              ;;
    notification) SERVICES=(ControlPanel Notification) ;;
    onboarding)   SERVICES=(Onboarding)                ;;
esac

SERVICES=( Services_Common ${SERVICES[@]} )

LANGUAGES=(objc cpp)

for SERVICE in ${SERVICES[@]}; do 

	SERVICE_NAME=${SERVICE}Service
	SRVC=$(echo $SERVICE | tr [A-Z] [a-z])

	for LANGUAGE in ${LANGUAGES[@]}; do
	
		# build product directory
		PROJ_DIR=$WORKSPACE/services/base/$SRVC/ios/samples/alljoyn_services_$LANGUAGE
		chmod -R 777 $PROJ_DIR
		
		for SDK in "iphonesimulator" "iphoneos"; 
		do
			echo -e "==========="
			echo -e "$SERVICE/$LANGUAGE/$SDK - Running 'xcodebuild' ($VARIANT)"
			echo -e "===========\n"
			
			# compile code
			( xcodebuild -project $PROJ_DIR/alljoyn_${SRVC}_${LANGUAGE}.xcodeproj -sdk $SDK -configuration $VARIANT ALLJOYN_SDK_ROOT=$3 ALLSEEN_BASE_SERVICES_ROOT=$WORKSPACE/services/base clean build )
			if [ $? != 0 ]
			then
				echo "failed to run xcodebuild $SERVICE/$LANGUAGE/$SDK ($VARIANT)"
				exit_status=45
				continue
			fi
		done # SDK
	done #LANGUAGE

	if [ "$SERVICE" = "Services_Common" ]
	then
		continue
	fi

	# sample app directory
	if [ "$SERVICE" = "Notification" ]
	then
		APP_DIR=$WORKSPACE/services/base/$SRVC/ios/samples/$SERVICE_NAME
	else
		APP_DIR=$WORKSPACE/services/base/$SRVC/ios/samples/sampleApp
	fi

	for SDK in "iphonesimulator" "iphoneos";
	do
		echo -e "=========="
		echo -e "$SERVICE/$SDK - Running 'xcodebuild' ($VARIANT)"
		echo -e "==========\n"
	
		# compile sample app
		( xcodebuild -project $APP_DIR/$SERVICE_NAME.xcodeproj CODE_SIGN_IDENTITY="" CODE_SIGNING_REQUIRED=NO -sdk $SDK -configuration $VARIANT ALLJOYN_SDK_ROOT=$3 ALLSEEN_BASE_SERVICES_ROOT=$WORKSPACE/services/base ENABLE_BITCODE=NO clean build )
		if [ $? != 0 ]
		then 
			echo "failed to run xcodebuild $SERVICE_NAME/$SDK ($VARIANT)"
			exit_status=45
			continue
		fi
	done #SDK
done #SERVICE
	
echo "exit_status = $exit_status"
exit $exit_status