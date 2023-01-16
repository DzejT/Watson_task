include $(TOPDIR)/rules.mk

PKG_NAME:=daemon
PKG_RELEASE:=1
PKG_VERSION:=1.3.67

include $(INCLUDE_DIR)/package.mk

define Package/daemon
	CATEGORY:=Base system
	TITLE:=example_prog
	DEPENDS:=+IoT_Watson +libubox +libubus
endef

define Package/daemon/description
	This is an example app package
endef


define Package/daemon/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/daemon $(1)/usr/bin
	$(INSTALL_BIN) ./files/daemon.init $(1)/etc/init.d/daemon
	$(INSTALL_DATA) ./files/config.yaml $(1)/usr/bin
	$(INSTALL_CONF) ./files/daemon.config $(1)/etc/config/daemon
endef

$(eval $(call BuildPackage,daemon))
