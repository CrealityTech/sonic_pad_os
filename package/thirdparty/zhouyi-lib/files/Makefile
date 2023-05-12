TARGET = libaipu.so
TARGET_SO_NAME = libaipu.so.$(MAJOR)
TARGET_REAL_NAME = libaipu.so.$(MAJOR).$(MINOR)

SRCS = $(wildcard ./src/*.cpp) $(wildcard ./src/*/*.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

#override CXXFLAGS += -O0 -g -DRTDEBUG=1	# debug
override CXXFLAGS += -O2 -DRTDEBUG=0		# release

override CXXFLAGS += -DARM_LINUX=1
override CXXFLAGS += -std=c++11 -fPIC -Wall -Werror
override LDFLAGS += -shared -Wl,-soname,$(TARGET_SO_NAME)
override INCLUDES += -I./include -I./src

all: $(TARGET) install

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET_REAL_NAME): $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(TARGET_SO_NAME): $(TARGET_REAL_NAME)
	ln -s $(TARGET_REAL_NAME) $(TARGET_SO_NAME)

$(TARGET): $(TARGET_SO_NAME)
	ln -s $(TARGET_SO_NAME) $(TARGET)

install:
	mkdir -p $(INSTALL_PREFIX)/usr/include
	cp ./include/* $(INSTALL_PREFIX)/usr/include
	mkdir -p $(INSTALL_PREFIX)/usr/lib
	cp -d $(TARGET) $(TARGET_SO_NAME) $(TARGET_REAL_NAME) $(INSTALL_PREFIX)/usr/lib

clean:
	rm -rf $(TARGET) $(TARGET_SO_NAME) $(TARGET_REAL_NAME) $(OBJS)

.PHONY: all clean install
