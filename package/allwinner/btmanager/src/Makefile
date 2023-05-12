
install:
	-@mkdir -p $(CONFIG_PREFIX)/usr/lib
	-@mkdir -p $(CONFIG_PREFIX)/usr/include
	@cp include/bt_manager.h $(CONFIG_PREFIX)/usr/include
	@cp include/bt_log.h $(CONFIG_PREFIX)/usr/include
	@cp include/bt_dev_list.h $(CONFIG_PREFIX)/usr/include
	@cp lib/$(CPU)/$(CONFIG_LIBC)/*.so $(CONFIG_PREFIX)/usr/lib

all: install

.PHONY:clean
clean:
	rm -rf $(target)
