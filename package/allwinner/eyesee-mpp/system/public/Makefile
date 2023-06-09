############################################## # OpenWrt Makefile for eyesee-mpp-system
#
#
# Most of the variables used here are defined in
# the include directives below. We just need to
# specify a basic description of the package,
# where to build our program, where to find
# the source files, and where to install the
# compiled program on the router.
#
# Be very careful of spacing in this file.
# Indents should be tabs, not spaces, and
# there should be no trailing whitespace in
# lines that are not commented.
#
##############################################
include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/kernel.mk

# Name and release number of this package
PKG_NAME:=eyesee-mpp-system
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_MAINTAINER:=AW-PDC-PD5
# This specifies the directory where we're going to build the program.
# The root build directory, $(BUILD_DIR), is by default the build_mipsel
# directory in your OpenWrt SDK directory
PKG_BUILD_DIR := $(COMPILE_DIR)/$(PKG_NAME)
SRC_COMPILE_DIR := $(PKG_BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
SRC_CODE_DIR := $(TOPDIR)/external/eyesee-mpp/system/public

# This specifies the directory where file depends. when files in it update, increasing compile will be enable.
PKG_FILE_DEPENDS:= $(SRC_CODE_DIR)

include $(BUILD_DIR)/package.mk

# Specify package information for this program.
# The variables defined here should be self explanatory.
# If you are running Kamikaze, delete the DESCRIPTION
# variable below and uncomment the Kamikaze define
# directive for the description below
define Package/$(PKG_NAME)
  SECTION:=eyesee-mpp-system
  CATEGORY:=Allwinner
  SUBMENU:=eyesee-mpp
  TITLE:=eyesee-mpp-system for Tina Linux
  DEPENDS:= +glog +liblua +eyesee-mpp-external
endef

define Package/$(PKG_NAME)/config
source "$(SOURCE)/Config.in"
endef

# Uncomment portion below for Kamikaze and delete DESCRIPTION variable above
define Package/$(PKG_NAME)/description
  eyesee-mpp-system is as eyesee-mpp low level libraries.
endef

# Specify what needs to be done to prepare for building the package.
# In our case, we need to copy the source files to the build directory.
# This is NOT the default.  The default uses the PKG_SOURCE_URL and the
# PKG_SOURCE which is not defined here to download the source from the web.
# In order to just build a simple program that we have just written, it is
# much easier to do it this way.

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	if [ -d $(SRC_COMPILE_DIR) ]; then \
		rm -rf $(SRC_COMPILE_DIR); \
	fi
	ln -s $(SRC_CODE_DIR) $(SRC_COMPILE_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(SRC_COMPILE_DIR) -f tina.mk \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CXX="$(TARGET_CXX)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		CXXFLAGS="$(TARGET_CXXFLAGS)" \
		CPPFLAGS="$(TARGET_CPPFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		LINUX_USER_HEADERS="$(LINUX_DIR)/user_headers" \
		all
endef

define Build/Clean
	if [ -d $(SRC_COMPILE_DIR) ]; then \
		$(MAKE) -C $(SRC_COMPILE_DIR) -f tina.mk \
			ARCH="$(TARGET_ARCH)" \
			AR="$(TARGET_AR)" \
			CC="$(TARGET_CC)" \
			CXX="$(TARGET_CXX)" \
			CFLAGS="$(TARGET_CFLAGS)" \
			CXXFLAGS="$(TARGET_CXXFLAGS)" \
			CPPFLAGS="$(TARGET_CPPFLAGS)" \
			LDFLAGS="$(TARGET_LDFLAGS)" \
			clean; \
	else \
		echo "$(SRC_COMPILE_DIR) is not exist!"; \
	fi
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib/eyesee-mpp/
	$(INSTALL_DIR) $(1)/usr/include/eyesee-mpp/system/public
	$(CP) -r $(SRC_COMPILE_DIR)/include $(1)/usr/include/eyesee-mpp/system/public
	$(CP) $(SRC_COMPILE_DIR)/liblog/liblog.a $(1)/usr/lib/eyesee-mpp
	$(CP) $(SRC_COMPILE_DIR)/liblog/liblog.so $(1)/usr/lib/eyesee-mpp
	$(INSTALL_DIR) $(1)/usr/include/eyesee-mpp/system/public/libion
	$(CP) -r $(SRC_COMPILE_DIR)/libion/include $(1)/usr/include/eyesee-mpp/system/public/libion
	$(CP) $(SRC_COMPILE_DIR)/libion/libion.a $(1)/usr/lib/eyesee-mpp
	$(CP) $(SRC_COMPILE_DIR)/libion/libion.so $(1)/usr/lib/eyesee-mpp
#	$(CP) $(SRC_COMPILE_DIR)/libcutils/libcutils.a $(1)/usr/lib/eyesee-mpp
#	$(CP) $(SRC_COMPILE_DIR)/libcutils/libcutils.so $(1)/usr/lib/eyesee-mpp
	find $(SRC_COMPILE_DIR)/display \( -name "libhwdisplay.a" -o -name "libhwdisplay.so" \) -exec $(CP) {} $(1)/usr/lib/eyesee-mpp \;
	$(CP) $(SRC_COMPILE_DIR)/luaconfig/libluaconfig.a $(1)/usr/lib/eyesee-mpp
	$(CP) $(SRC_COMPILE_DIR)/luaconfig/libluaconfig.so $(1)/usr/lib/eyesee-mpp
	$(CP) $(SRC_COMPILE_DIR)/wifi/wpa_supplicant/libwpa_ctl.a $(1)/usr/lib/eyesee-mpp
	$(CP) $(SRC_COMPILE_DIR)/wifi/libwifi_sta.a $(1)/usr/lib/eyesee-mpp
	$(CP) $(SRC_COMPILE_DIR)/wifi/libwifi_ap.a $(1)/usr/lib/eyesee-mpp
	$(INSTALL_DIR) $(1)/usr/include/eyesee-mpp/system/public/smartlink
	$(CP) $(SRC_COMPILE_DIR)/smartlink/*.h $(1)/usr/include/eyesee-mpp/system/public/smartlink
	$(CP) $(SRC_COMPILE_DIR)/smartlink/libsmartlink.a $(1)/usr/lib/eyesee-mpp
	$(INSTALL_DIR) $(1)/usr/include/eyesee-mpp/system/public/rgb_ctrl
	$(CP) $(SRC_COMPILE_DIR)/rgb_ctrl/*.h $(1)/usr/include/eyesee-mpp/system/public/rgb_ctrl
	$(CP) $(SRC_COMPILE_DIR)/rgb_ctrl/librgb_ctrl.a $(1)/usr/lib/eyesee-mpp
	$(CP) $(SRC_COMPILE_DIR)/rgb_ctrl/librgb_ctrl.so $(1)/usr/lib/eyesee-mpp
#	$(CP) $(SRC_COMPILE_DIR)/rtsp/libTinyServer.so $(1)/usr/lib/eyesee-mpp
	$(CP) $(SRC_COMPILE_DIR)/rtsp/libTinyServer.a $(1)/usr/lib/eyesee-mpp
endef

# We do not need to define Build/Configure or Build/Compile directives
# The defaults are appropriate for compiling a simple program such as this one


# Specify where and how to install the program. Since we only have one file,
# the helloworld executable, install it by copying it to the /bin directory on
# the router. The $(1) variable represents the root directory on the router running
# OpenWrt. The $(INSTALL_DIR) variable contains a command to prepare the install
# directory if it does not already exist.  Likewise $(INSTALL_BIN) contains the
# command to copy the binary file from its current location (in our case the build
# directory) to the install directory.
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib/eyesee-mpp
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/share/lua/5.1
	$(INSTALL_DIR) $(1)/usr/share/osd/fonts/
	$(CP) $(SRC_COMPILE_DIR)/liblog/liblog.so $(1)/usr/lib/eyesee-mpp
#	$(CP) $(SRC_COMPILE_DIR)/logcat/alogcat.sh $(1)/usr/bin/alogcat
#	$(INSTALL_BIN) $(SRC_COMPILE_DIR)/logcat/alogcat.sh $(1)/bin/alogcat
	$(CP) $(SRC_COMPILE_DIR)/libion/libion.so $(1)/usr/lib/eyesee-mpp
#	$(CP) $(SRC_COMPILE_DIR)/libcutils/libcutils.so $(1)/usr/lib/eyesee-mpp
	find $(SRC_COMPILE_DIR)/display -name "libhwdisplay.so" -exec $(CP) {} $(1)/usr/lib/eyesee-mpp \;
ifeq ($(CONFIG_PACKAGE_newfs_msdos),)
	$(INSTALL_BIN) $(SRC_COMPILE_DIR)/newfs_msdos/newfs_msdos $(1)/usr/bin
endif
	$(INSTALL_BIN) $(SRC_COMPILE_DIR)/reboot_efex/reboot_efex $(1)/usr/bin
	$(CP) $(SRC_COMPILE_DIR)/luaconfig/libluaconfig.so $(1)/usr/lib/
	$(CP) $(SRC_COMPILE_DIR)/luaconfig/persistence.lua $(1)/usr/share/lua/5.1
	$(CP) $(SRC_COMPILE_DIR)/luaconfig/persistence.lua $(1)/usr/share/lua/
ifeq ($(CONFIG_wifi_8189ftv),y)
	@echo "The wifi is 8189ftv"
	$(INSTALL_DIR) $(1)/etc/firmware
	$(CP) -v --remove-destination $(SRC_COMPILE_DIR)/wifi/firmware/8189ftv $(1)/etc/firmware 1>&2
	$(CP) -v --remove-destination $(SRC_COMPILE_DIR)/wifi/firmware/*.sh $(1)/etc/firmware 1>&2
else ifeq ($(CONFIG_wifi_ap6181),y)
	@echo "The wifi is ap6181"
	$(INSTALL_DIR) $(1)/etc/firmware
	$(CP) -v --remove-destination $(SRC_COMPILE_DIR)/wifi/firmware/ap6181 $(1)/etc/firmware 1>&2
	$(CP) -v --remove-destination $(SRC_COMPILE_DIR)/wifi/firmware/*.sh $(1)/etc/firmware 1>&2
else ifeq ($(CONFIG_wifi_ap6255),y)
	@echo "The wifi is ap6255"
	$(INSTALL_DIR) $(1)/etc/firmware
	$(CP) -v --remove-destination $(SRC_COMPILE_DIR)/wifi/firmware/ap6255 $(1)/etc/firmware 1>&2
	$(CP) -v --remove-destination $(SRC_COMPILE_DIR)/wifi/firmware/*.sh $(1)/etc/firmware 1>&2
else ifeq ($(CONFIG_wifi_ap6355),y)
	@echo "The wifi is ap6355"
	$(INSTALL_DIR) $(1)/etc/firmware
	$(CP) -v --remove-destination $(SRC_COMPILE_DIR)/wifi/firmware/ap6355 $(1)/etc/firmware 1>&2
	$(CP) -v --remove-destination $(SRC_COMPILE_DIR)/wifi/firmware/*.sh $(1)/etc/firmware 1>&2
else ifeq ($(CONFIG_wifi_xr819),y)
	@echo "The wifi is xr819"
	$(INSTALL_DIR) $(1)/etc/firmware/xr819
	$(CP) -v --remove-destination $(SRC_COMPILE_DIR)/wifi/firmware/xr819/*.bin $(1)/etc/firmware 1>&2
	$(CP) -v --remove-destination $(SRC_COMPILE_DIR)/wifi/firmware/*.sh $(1)/etc/firmware 1>&2
else
	@echo "no wifi"
endif
	$(CP) $(SRC_COMPILE_DIR)/smartlink/libsmartlink.so $(1)/usr/lib/eyesee-mpp
	$(CP) $(SRC_COMPILE_DIR)/rgb_ctrl/librgb_ctrl.so $(1)/usr/lib/eyesee-mpp
	$(CP) $(SRC_COMPILE_DIR)/rgb_ctrl/fonts/asc*.lz4 $(1)/usr/share/osd/fonts/
	#$(INSTALL_BIN) $(SRC_COMPILE_DIR)/ntpclient/ntpclient $(1)/usr/bin
endef

#define Package/$(PKG_NAME)/InstallDev
#endef

# This line executes the necessary commands to compile our program.
# The above define directives specify all the information needed, but this
# line calls BuildPackage which in turn actually uses this information to
# build a package.

$(eval $(call BuildPackage,$(PKG_NAME)))
