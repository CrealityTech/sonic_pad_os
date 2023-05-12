SHARED_TARGET := lib*.so
STATIC_TARGET := lib*.a

SUBLIB_DIR = ./..

all:
	mkdir -p $(INSTALL_PREFIX)/usr/lib
	install -m0755 ./$(C_LIB_TYPE)/$(CPU_VARIANT)/$(SHARED_TARGET) $(INSTALL_PREFIX)/usr/lib
	install -m0755 ./$(C_LIB_TYPE)/$(CPU_VARIANT)/$(SHARED_TARGET) $(SUBLIB_DIR)

