Package/rtl8733bs-firmware = $(call Package/firmware-default,RealTek RTL8733BS firmware)

define Package/rtl8733bs-firmware/install
	$(INSTALL_DIR) $(1)/$(FIRMWARE_PATH)/rtlbt
	$(INSTALL_DATA) \
		$(TOPDIR)/package/firmware/linux-firmware/rtl8733bs/rtl8723fs_fw \
		$(1)/$(FIRMWARE_PATH)/rtlbt/rtl8723fs_fw
	$(INSTALL_DATA) \
		$(TOPDIR)/package/firmware/linux-firmware/rtl8733bs/rtl8723fs_config \
		$(1)/$(FIRMWARE_PATH)/rtlbt/rtl8723fs_config
endef

$(eval $(call BuildPackage,rtl8733bs-firmware))
