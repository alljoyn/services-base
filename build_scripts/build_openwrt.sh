#!/usr/bin/env bash

if [ -z "$WORKSPACE" ]; then
	echo "Environment variable WORKSPACE is not set. Aborting."
	exit 1
fi

OPENWRT_PROJECT_DIR=$WORKSPACE/openwrt
OPENWRT_FEEDS_CONF=$OPENWRT_PROJECT_DIR/feeds.conf
OPENWRT_CONFIG=$OPENWRT_PROJECT_DIR/.config

#----------------------------
# Local AJ repo and src locations
#----------------------------
ALLJOYN_PROJECT_DIR=$WORKSPACE/alljoyn_source
ALLJOYN_CORE_PROJECT_DIR=$ALLJOYN_PROJECT_DIR/core/alljoyn
ALLJOYN_BASE_PROJECT_DIR=$ALLJOYN_PROJECT_DIR/services/base

ALLJOYN_TARBALL_DIR=$WORKSPACE/alljoyn_tarball
AJ_CORE_TARBALL_ROOT_DIRNAME=alljoyn-16.04.00-src
AJ_BASE_TARBALL_ROOT_DIRNAME=alljoyn-services-16.04.00-src
AJ_CORE_TARBALL=$AJ_CORE_TARBALL_ROOT_DIRNAME.tar.gz
AJ_BASE_TARBALL=$AJ_BASE_TARBALL_ROOT_DIRNAME.tar.gz

if [ -d $OPENWRT_PROJECT_DIR/feeds/alljoyn ]; then
	cd $OPENWRT_PROJECT_DIR
	make defconfig
	make V=s package/feeds/alljoyn/alljoyn/clean
	make V=s package/feeds/alljoyn/alljoyn-services_common/clean
	make V=s package/feeds/alljoyn/alljoyn-controlpanel/clean
	make V=s package/feeds/alljoyn/alljoyn-notification/clean
	make V=s package/feeds/alljoyn/alljoyn-onboarding/clean
	make V=s package/feeds/alljoyn/alljoyn-sample_apps/clean

	rm -rf $ALLJOYN_TARBALL_DIR
	rm -rf $OPENWRT_PROJECT_DIR/dl/alljoyn*
	rm -rf $OPENWRT_PROJECT_DIR/feeds/alljoyn*
fi

echo "Making config"
echo CONFIG_TARGET_x86=y > $OPENWRT_CONFIG

#
# Initialize feeds.conf
#
cd $OPENWRT_PROJECT_DIR

# Basic feeds
grep "src-git packages" $OPENWRT_FEEDS_CONF.default > $OPENWRT_FEEDS_CONF
grep "src-git luci" $OPENWRT_FEEDS_CONF.default >> $OPENWRT_FEEDS_CONF

# AllJoyn feed
AJ_OPENWRT_FEED=https://bitbucket.org/twobulls/alljoyn-openwrt-feed.git
AJ_OPENWRT_FEED_BRANCH=16.04-baseservices-2b

echo "src-git alljoyn $AJ_OPENWRT_FEED;$AJ_OPENWRT_FEED_BRANCH" >> $OPENWRT_FEEDS_CONF

./scripts/feeds update -a
./scripts/feeds install -a

#
# Select AllJoyn Packages
#
# AJ package identifiers
AJ_CORE_PKG="CONFIG_PACKAGE_alljoyn"
AJ_ABOUT_PKG="CONFIG_PACKAGE_alljoyn-about"
AJ_C_PKG="CONFIG_PACKAGE_alljoyn-c"
AJ_CONFIG_PKG="CONFIG_PACKAGE_alljoyn-config"
AJ_CONTROLPANEL_PKG="CONFIG_PACKAGE_alljoyn-controlpanel"
AJ_NON_GW_CONFIG_PKG="CONFIG_PACKAGE_alljoyn-non-gw-config"
AJ_NOTIFICATION_PKG="CONFIG_PACKAGE_alljoyn-notification"
AJ_SAMPLES_PKG="CONFIG_PACKAGE_alljoyn-samples"
AJ_SERVICES_SAMPLE_APPS_PKG="CONFIG_PACKAGE_alljoyn-services_common"
AJ_SERVICES_COMMON_PKG="CONFIG_PACKAGE_alljoyn-sample_apps"
AJ_CONFIG_SAMPLES_PKG="CONFIG_PACKAGE_alljoyn-config-samples"
AJ_CONTROLPANEL_SAMPLES_PKG="CONFIG_PACKAGE_alljoyn-controlpanel-samples"
AJ_NOTIFICATION_SAMPLES_PKG="CONFIG_PACKAGE_alljoyn-notification-samples"
AJ_ONBOARDING_SAMPLES_PKG="CONFIG_PACKAGE_alljoyn-onboarding"

# Prerequisite pacakges for onboarding
WPADMINI_DEFAULT=CONFIG_DEFAULT_wpad-mini
WPADMINI_PKG=CONFIG_PACKAGE_wpad-mini
WPACLI_PKG=CONFIG_PACKAGE_wpa-cli
WPASUPPLICANT_PKG=CONFIG_PACKAGE_wpa-supplicant
HOSTAPD_PKG=CONFIG_PACKAGE_hostapd

# Enable AllJoyn Core
make defconfig
sed -i "s/.*$AJ_CORE_PKG.*$/$AJ_CORE_PKG\=y/g" $OPENWRT_CONFIG

# Enable some services
make defconfig
sed -i "s/.*$AJ_ABOUT_PKG.*$/$AJ_ABOUT_PKG\=y/g" $OPENWRT_CONFIG
sed -i "s/.*$AJ_C_PKG.*$/$AJ_C_PKG\=y/g" $OPENWRT_CONFIG
sed -i "s/.*$AJ_CONFIG_PKG.*$/$AJ_CONFIG_PKG\=n/g" $OPENWRT_CONFIG
sed -i "s/.*$AJ_CONTROLPANEL_PKG.*$/$AJ_CONTROLPANEL_PKG\=y/g" $OPENWRT_CONFIG
sed -i "s/.*$AJ_NON_GW_CONFIG_PKG.*$/$AJ_NON_GW_CONFIG_PKG\=y/g" $OPENWRT_CONFIG
sed -i "s/.*$AJ_NOTIFICATION_PKG.*$/$AJ_NOTIFICATION_PKG\=y/g" $OPENWRT_CONFIG
sed -i "s/.*$AJ_SAMPLES_PKG.*$/$AJ_SAMPLES_PKG\=y/g" $OPENWRT_CONFIG
sed -i "s/.*$AJ_SERVICES_SAMPLE_APPS_PKG.*$/$AJ_SERVICES_SAMPLE_APPS_PKG\=y/g" $OPENWRT_CONFIG
sed -i "s/.*$AJ_SERVICES_COMMON_PKG.*$/$AJ_SERVICES_COMMON_PKG\=y/g" $OPENWRT_CONFIG

# ...more services
make defconfig
sed -i "s/.*$AJ_CONFIG_SAMPLES_PKG.*$/$AJ_CONFIG_SAMPLES_PKG\=y/g" $OPENWRT_CONFIG
sed -i "s/.*$AJ_CONTROLPANEL_SAMPLES_PKG.*$/$AJ_CONTROLPANEL_SAMPLES_PKG\=y/g" $OPENWRT_CONFIG
sed -i "s/.*$AJ_NOTIFICATION_SAMPLES_PKG.*$/$AJ_NOTIFICATION_SAMPLES_PKG\=y/g" $OPENWRT_CONFIG

# Enable dependencies for onboarding
sed -i "s/.*$WPADMINI_DEFAULT.*$/$WPADMINI_DEFAULT\=n/g" $OPENWRT_CONFIG
sed -i "s/.*$WPADMINI_PKG.*$/$WPADMINI_PKG\=n/g" $OPENWRT_CONFIG
sed -i "s/.*$WPACLI_PKG.*$/$WPACLI_PKG\=y/g" $OPENWRT_CONFIG
sed -i "s/.*$WPASUPPLICANT_PKG.*$/$WPASUPPLICANT_PKG\=y/g" $OPENWRT_CONFIG

# ...finally enable aj-onboarding
make defconfig
sed -i "s/.*$AJ_ONBOARDING_SAMPLES_PKG.*$/$AJ_ONBOARDING_SAMPLES_PKG\=y/g" $OPENWRT_CONFIG

#
# AllJoyn tarball
#
echo "Creating Tarballs..."
mkdir $ALLJOYN_TARBALL_DIR

# AllJoyn core tarball
cd $ALLJOYN_CORE_PROJECT_DIR/..
AJ_CORE_DIR_NAME=$(basename $ALLJOYN_CORE_PROJECT_DIR)
mv $AJ_CORE_DIR_NAME $AJ_CORE_TARBALL_ROOT_DIRNAME
tar czf $ALLJOYN_TARBALL_DIR/$AJ_CORE_TARBALL $AJ_CORE_TARBALL_ROOT_DIRNAME/
mv $AJ_CORE_TARBALL_ROOT_DIRNAME $AJ_CORE_DIR_NAME

BASE_SERVICES_TARBALL_DIR=$WORKSPACE/$AJ_BASE_TARBALL_ROOT_DIRNAME

# Base services tarball
cd $ALLJOYN_PROJECT_DIR/..
AJ_DIR_NAME=$(basename $ALLJOYN_PROJECT_DIR)

mkdir -p $BASE_SERVICES_TARBALL_DIR
mkdir -p $BASE_SERVICES_TARBALL_DIR/core/alljoyn
mv $ALLJOYN_PROJECT_DIR/services $BASE_SERVICES_TARBALL_DIR/
cp -R $ALLJOYN_CORE_PROJECT_DIR/build_core $BASE_SERVICES_TARBALL_DIR/core/alljoyn

tar czf $ALLJOYN_TARBALL_DIR/$AJ_BASE_TARBALL $AJ_BASE_TARBALL_ROOT_DIRNAME/
mv $BASE_SERVICES_TARBALL_DIR/services $ALLJOYN_PROJECT_DIR
rm -rf $BASE_SERVICES_TARBALL_DIR/

# Copy tarball to  dl directory
mkdir -p $OPENWRT_PROJECT_DIR/dl
cp  $ALLJOYN_TARBALL_DIR/$AJ_CORE_TARBALL $OPENWRT_PROJECT_DIR/dl/
cp  $ALLJOYN_TARBALL_DIR/$AJ_BASE_TARBALL $OPENWRT_PROJECT_DIR/dl/

# Clean up AJ source
rm -rf $ALLJOYN_TARBALL_DIR


CONFIG_CCACHE="CONFIG_CCACHE"
CONFIG_LIBCAP="CONFIG_PACKAGE_libcap"

sed -i "s/.*$CONFIG_CCACHE.*$/$CONFIG_CCACHE\=y/g" $OPENWRT_CONFIG
sed -i "s/.*$CONFIG_LIBCAP.*$/$CONFIG_LIBCAP\=y/g" $OPENWRT_CONFIG

cd $OPENWRT_PROJECT_DIR
make defconfig
make -j1 V=s

