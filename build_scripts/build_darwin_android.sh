#!/bin/bash -xe
# #    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
#    Source Project (AJOSP) Contributors and others.
#
#    SPDX-License-Identifier: Apache-2.0
#
#    All rights reserved. This program and the accompanying materials are
#    made available under the terms of the Apache License, Version 2.0
#    which accompanies this distribution, and is available at
#    http://www.apache.org/licenses/LICENSE-2.0
#
#    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
#    Alliance. All rights reserved.
#
#    Permission to use, copy, modify, and/or distribute this software for
#    any purpose with or without fee is hereby granted, provided that the
#    above copyright notice and this permission notice appear in all
#    copies.
#
#     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
#     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
#     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
#     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
#     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
#     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
#     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#     PERFORMANCE OF THIS SOFTWARE.
#
exit_status=0

BASE_SERVICE=$1
VARIANT=$2
WORKSPACE=${WORKSPACE:-$ALLJOYN_ROOT_DIR}

case $BASE_SERVICE in
	controlpanel) SERVICES=(ControlPanel) ;;
	notification) SERVICES=(ControlPanel Notification) ;;
	onboarding) SERVICES=(Onboarding) ;;
esac

SERVICES=( Common_Utils ${SERVICES[@]} )

VRNT=$(echo $VARIANT | tr [A-Z] [a-z])

JAR_DIR=${JAR_DIR:-$WORKSPACE/artifacts/$VRNT/jar}
LIB_DIR=${LIB_DIR:-$WORKSPACE/artifacts/$VRNT/lib}

CopyAndroidJars () {
	cp $JAR_DIR/{alljoyn,alljoyn_about}.jar $1 || { 
		echo "Copy of android jars failed!"
		exit 10
	}
}

CopyAndroidLib () {
	cp $LIB_DIR/liballjoyn_java.so $1 || { 
    		echo "Copy of alljoyn lib failed" 
    		exit 10
	}
}

CopyCommonUtils () {
	cp $WORKSPACE/services/base/sample_apps/android/common_utils/build/deploy/alljoyn_apps_android_utils.jar $1 || {
    		echo "Copy of Common Utils failed"
    		exit 10
	}
}

# TODO make document more DRY: use functions with arguments for other repeated patterns

for SERVICE in ${SERVICES[@]}; do

	SERVICE_NAME=${SERVICE}Service
	SRVC=$(echo $SERVICE | tr [A-Z] [a-z])

	if [ "$SERVICE" = "Common_Utils" ]
	then
		PROJ_DIR=$WORKSPACE/services/base/sample_apps/android/common_utils/
	elif [ "$SERVICE" = "Notification" ]
	then
		PROJ_DIR=$WORKSPACE/services/base/$SRVC/java/native_platform/NotificationServiceNativePlatformAndroid/
	else
		PROJ_DIR=$WORKSPACE/services/base/$SRVC/java/$SERVICE_NAME/
	fi

	pushd $PROJ_DIR
	mkdir -p libs/
	CopyAndroidJars libs/

	if [ "$SERVICE" = "Notification" ]
	then
		CopyCommonUtils libs/
	fi

	ant -Dsdk.dir=$ANDROID_SDK || {
		echo "Building $SERVICE failed!"
		exit 20
	}
	popd

	case $SERVICE in
		Common_Utils)	continue ;;
		ControlPanel)	PROJ_DIR=$WORKSPACE/services/base/$SRVC/java/${SERVICE}Adapter/ ;;
		Onboarding)	PROJ_DIR=$WORKSPACE/services/base/$SRVC/java/${SERVICE}Manager/android/ ;;
	esac

	if [ "$SERVICE" != "Notification" ]
	then
		pushd $PROJ_DIR
		mkdir -p libs/
	fi

	if [ "$SERVICE" = "ControlPanel" ]
	then
		cp ../$SERVICE_NAME/build/deploy/${SERVICE_NAME}.jar libs/ || {
			echo "Copy of $SERVICE_NAME failed!"
			exit 30
		}
		
		ant -Dsdk.dir=$ANDROID_SDK $VARIANT || {
			echo "Building $PROJ_DIR failed"
			exit 20
		}
		popd

	fi

	if [ "$SERVICE" = "Onboarding" ]
	then
		cp ../../$SERVICE_NAME/build/deploy/alljoyn_onboarding.jar libs/ || {
			echo "Copy of $SERVICE_NAME failed!"
			exit 30
		}
		CopyAndroidJars libs/
		CopyCommonUtils libs/

		ant -Dsdk.dir=$ANDROID_SDK || {
			echo "Building $PROJ_DIR failed"
			exit 20
		}
		popd

	fi

	# compile Android .apk
	case $SERVICE in
		ControlPanel)	APP_DIR=$WORKSPACE/services/base/$SRVC/java/sample_applications/android/${SERVICE}Browser/ ;;
		Onboarding)	APP_DIR=$WORKSPACE/services/base/$SRVC/java/sample_applications/android/${SERVICE}SampleClient/ ;;
		Notification)	APP_DIR=$WORKSPACE/services/base/$SRVC/java/sample_applications/android/${SERVICE}ServiceUISample/ ;;
	esac

	pushd $APP_DIR
	mkdir -p libs/armeabi
	CopyAndroidJars libs/
	CopyAndroidLib libs/armeabi/
	CopyCommonUtils libs/

	case $SERVICE in
		ControlPanel|Notification)
			cp ../../../../../controlpanel/java/ControlPanelService/build/deploy/ControlPanelService.jar ../../../../../controlpanel/java/ControlPanelAdapter/bin/ControlPanelAdapter.jar libs/ || { 
    				echo "Copy of ControlPanel libs failed" 
    				exit 10
			}
			
			if [ "$SERVICE" = "ControlPanel" ]
			then
				cp $ANDROID_SDK/extras/android/support/v4/android-support-v4.jar libs/ || {
					echo "Copy of android-support-v4.jar failed"
					exit 10
				}
			fi

			if [ "$SERVICE" = "Notification" ]
			then
				cp ../../../native_platform/NotificationServiceNativePlatformAndroid/build/deploy/{NotificationService,NSCommons,NSNativePlatformAndr}.jar libs/ || {
					echo "Copy of notification jars failed"
					exit 10
				}	
			fi
			;;
		Onboarding)
			cp ../../../$SERVICE_NAME/build/deploy/alljoyn_onboarding.jar libs/ || { 
				echo "Copy of alljoyn_onboarding.jar failed" 
				exit 10
			}
            cp ../../../OnboardingManager/android/build/deploy/alljoyn_onboarding_manager.jar libs/ || { 
    			echo "Copy of alljoyn_onboarding_manager.jar failed" 
    			exit 10
		    }
			;;
	esac

	ant -Dsdk.dir=$ANDROID_SDK $VARIANT || {
    		echo "Building $SERVICE Android .apk failed"
    		exit 20
	}

	# compile 2nd Onboarding sample app and Onboardee server sample
	if [ "$SERVICE" = "Onboarding" ]
	then
		cd ../OnboardingManagerSampleClient/
		mkdir -p libs/armeabi
		CopyAndroidJars libs/
		CopyAndroidLib libs/armeabi/
		CopyCommonUtils libs/

		cp ../../../$SERVICE_NAME/build/deploy/alljoyn_onboarding.jar libs/ || { 
			echo "Copy of alljoyn_onboarding.jar failed" 
			exit 10
		}

		cp ../../../OnboardingManager/android/build/deploy/alljoyn_onboarding_manager.jar libs/ || { 
    			echo "Copy of alljoyn_onboarding_manager.jar failed" 
    			exit 10
		}

		ant -Dsdk.dir=$ANDROID_SDK $VARIANT || {
    			echo "Building $SERVICE Android .apk failed"
    			exit 20
		}

        cd $WORKSPACE/services/base/simulators/android/about_conf_onb_server/
        mkdir -p libs/armeabi
        CopyAndroidJars libs/
        CopyAndroidLib libs/armeabi/
        CopyCommonUtils libs/

        cp ../../../$SERVICE/java/OnboardingService/build/deploy/alljoyn_onboarding.jar libs/ || {
            echo "Copy of alljoyn_onboarding.jar failed"
            exit 10
        }

        cp $JAR_DIR/alljoyn_config.jar libs/ || {
            echo "Copy of alljoyn_config.jar failed"
            exit 10
        }

        ant -Dsdk.dir=$ANDROID_SDK $VARIANT || {
            echo "Building About Conf server failed"
            exit 20
        }
	fi

	popd
done #SERVICE

echo "exit_status = $exit_status"
exit $exit_status