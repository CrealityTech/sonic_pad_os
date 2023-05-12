INCLUDES += -I ./include

TARGET = scr_test
TARGET_INOUT = scr_test_inout

install:
	-@mkdir -p $(CONFIG_PREFIX)/usr/bin

$(TARGET): scr_test.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDES) $(LDFLAGS)
	@cp -f $(TARGET) $(CONFIG_PREFIX)/usr/bin

$(TARGET_INOUT): scr_test_inout.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDES) $(LDFLAGS)
	@cp -f $(TARGET_INOUT) $(CONFIG_PREFIX)/usr/bin


######################################################
all: install $(TARGET) $(TARGET_INOUT)
clean:
	rm -rf $(TARGET) $(TARGET_INOUT)