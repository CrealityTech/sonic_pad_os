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
PKG_NAME:=rt_media
PKG_VERSION:=1.0
PKG_RELEASE:=1

# This specifies the directory where we're going to build the program.
# The root build directory, $(BUILD_DIR), is by default the build_mipsel
# directory in your OpenWrt SDK directory
PKG_BUILD_DIR := $(COMPILE_DIR)/$(PKG_NAME)
SRC_COMPILE_DIR := $(PKG_BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
SRC_CODE_DIR := $(TOPDIR)/external/fast-user-adapter/rt_media

# This specifies the directory where file depends. when files in it update, increasing compile will be enable.
PKG_FILE_DEPENDS:= $(SRC_CODE_DIR)

include $(BUILD_DIR)/package.mk

# Specify package information for this program.
# The variables defined here should be self explanatory.
# If you are running Kamikaze, delete the DESCRIPTION
# variable below and uncomment the Kamikaze define
# directive for the description below
define Package/$(PKG_NAME)
  SECTION:=rt_media
  CATEGORY:=Allwinner
  TITLE:=rt_media for Tina Linux
  DEPENDS:=+libstdcpp
endef

# define Package/$(PKG_NAME)/config
# source "$(SOURCE)/Config.in"
# endef

# Uncomment portion below for Kamikaze and delete DESCRIPTION variable above
define Package/$(PKG_NAME)/description
  fast user adapter
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
	$(MAKE) -C $(SRC_COMPILE_DIR)/tinyalsa \
		CC="$(TARGET_CC)" AR="$(TARGET_AR)"

	$(MAKE) -C $(SRC_COMPILE_DIR)/api_adapter \
		CC="$(TARGET_CC)" AR="$(TARGET_AR)"


	$(MAKE) -C $(SRC_COMPILE_DIR)/demo \
		CC="$(TARGET_CC)"
endef

define Build/Clean
	if [ -d $(SRC_COMPILE_DIR) ]; then \
		$(MAKE) -C $(SRC_COMPILE_DIR)/tinyalsa clean; \
		$(MAKE) -C $(SRC_COMPILE_DIR)/api_adapter clean; \
		$(MAKE) -C $(SRC_COMPILE_DIR)/demo clean; \
	else \
		echo "$(SRC_COMPILE_DIR) is not exist!"; \
	fi
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
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_DIR) $(1)/lib
	$(CP) $(SRC_COMPILE_DIR)/audio_encoder/lib/*.so $(1)/lib
	$(CP) $(SRC_COMPILE_DIR)/audio_decoder/lib/*.so $(1)/lib
	$(CP) $(SRC_COMPILE_DIR)/audio_effect/lib/*.so $(1)/lib
	$(CP) $(SRC_COMPILE_DIR)/tinyalsa/*.so $(1)/lib
	$(CP) $(SRC_COMPILE_DIR)/api_adapter/*.so $(1)/lib
	$(CP) $(SRC_COMPILE_DIR)/demo/demo_audio_in $(1)/bin
	$(CP) $(SRC_COMPILE_DIR)/demo/demo_audio_out $(1)/bin
	$(CP) $(SRC_COMPILE_DIR)/demo/demo_video_in $(1)/bin
endef

# This line executes the necessary commands to compile our program.
# The above define directives specify all the information needed, but this
# line calls BuildPackage which in turn actually uses this information to
# build a package.

$(eval $(call BuildPackage,$(PKG_NAME)))
