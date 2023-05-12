INCLUDES += -I./include -I$(CONFIG_PREFIX)/usr/include
LIB_PATH := $(CONFIG_PREFIX)/usr/lib
LIBS += -L$(LIB_PATH) -lbluetooth -lglib-2.0 -lsbc -ljson-c -lgio-2.0 -lgobject-2.0 -lasound -lbtmg -ldl -lm -lresolv
LIBS+=-lshared-mainloop -lshared-glib -lbluetooth-internal
#-lbluealsa-hfp

target	= bt_test
C_SRCS = bt_test.c bt_cmd.c
C_OBJS := $(C_SRCS:.c=.o)
LIB_OBJS = $(C_OBJS)

install:
	-@mkdir -p $(CONFIG_PREFIX)/usr/bin

#aw_bt_test
$(C_OBJS): %.o: %.c
	$(CXX) $(CFLAGS) $(INCLUDES) -g -c -o $@ $^

$(target): $(LIB_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDES) -g $(LIBS)  $(LDFLAGS)
	@cp $(target)  $(CONFIG_PREFIX)/usr/bin/

bt_gatt_server_test: bt_gatt_server_test.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDES)  $(GATT_LIBS) $(LIBS)  $(LDFLAGS)
	@cp bt_gatt_server_test  $(CONFIG_PREFIX)/usr/bin/

bt_gatt_client_test: bt_gatt_client_test.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDES) $(GATT_LIBS) $(LIBS)  $(LDFLAGS)
	@cp bt_gatt_client_test  $(CONFIG_PREFIX)/usr/bin/
####################################################################
all: install $(target) bt_gatt_server_test bt_gatt_client_test
clean:
	rm -rf $(target) $(LIB_OBJS)
