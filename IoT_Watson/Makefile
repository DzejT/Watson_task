include $(TOPDIR)/rules.mk

PKG_NAME:=IoT_Watson
PKG_RELEASE:=1
PKG_VERSION:=1.0.0
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/ibm-watson-iot/iot-c
PKG_SOURCE_VERSION:=f921dce616af815da2236954c608a2f377c928c5
PKG_BUILD_DEPENDS:=libpaho-mqtt3as


include $(INCLUDE_DIR)/package.mk

define Package/IoT_Watson
	CATEGORY:=Examples
	TITLE:=IoT_Watson
	DEPENDS:=+libopenssl
endef

define Build/Configure
	cp Makefile_change.patch $(PKG_BUILD_DIR)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/wiotp/sdk/iotp_device.h $(STAGING_DIR)/usr/include
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/wiotp/sdk/iotp_config.h $(STAGING_DIR)/usr/include
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/build/libiotp-as-device.so $(STAGING_DIR)/usr/lib/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3as.so* $(STAGING_DIR)/usr/lib/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/paho.mqtt.c/src/MQTTProperties.h $(STAGING_DIR)/usr/include/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/paho.mqtt.c/src/MQTTExportDeclarations.h $(STAGING_DIR)/usr/include/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/wiotp/sdk/iotp_rc.h $(STAGING_DIR)/usr/include/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/wiotp/sdk/iotp_utils.h $(STAGING_DIR)/usr/include/
endef

define Package/IoT_Watson/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3as.so* $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-device.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,IoT_Watson))
