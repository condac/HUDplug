# This makefile has been modified to accept
# external environment variables

OS ?= LINUX
#OS = MACOSX
#OS = WINDOWS
#OS = WINDOWS64



ifeq ($(OS), LINUX)
#  sudo apt-get install libudev-dev
CC ?= gcc
INC += -ISDK/CHeaders/XPLM -ISDK/CHeaders/Widgets
#CFLAGS = -Wall -O0 -g -D$(OS) $(INC)
CFLAGS += -Wall -O2 -fPIC -D$(OS) $(INC) -fvisibility=hidden
LDFLAGS += -Wl,--version-script=exports.txt
LDDYNFLAGS += -shared
LIBS +=  -lm -ludev -lpthread -lrt -lGLEW -lGLU
TARGET ?= lin.xpl

else ifeq ($(OS), MACOSX)
CC ?= gcc-4.2
SDK ?= /Developer/SDKs/MacOSX10.5.sdk
#ARCH = -arch i386 -mmacosx-version-min=10.5
ARCH ?= -arch i386 -arch x86_64 -mmacosx-version-min=10.5
INC += -ISDK/CHeaders/XPLM -ISDK/CHeaders/Widgets
CFLAGS += -Wall -O2 $(ARCH) -D$(OS) -isysroot $(SDK) $(INC) -fvisibility=hidden
LDFLAGS += $(ARCH) -isysroot $(SDK)
LDDYNFLAGS += -bundle -undefined dynamic_lookup -single_module
LIBS += -framework IOKit -framework CoreFoundation
TARGET ?= mac.xpl

else ifeq ($(OS), WINDOWS)
#  sudo apt-get install mingw32 mingw32-binutils mingw32-runtime
#  sudo apt-get install mono-devel wine
#  sudo apt install gcc-mingw-w64-i686


CC ?= i686-w64-mingw32-gcc
INC += -ISDK2/CHeaders/XPLM -ISDK2/CHeaders/Widgets -I/mingw32/include -I./glew/glew-2.2.0/include
CFLAGS += -Wall -O2 -D$(OS) $(INC) -g -DNO_FRAMEBUFFER -DNO_GLEW
LDFLAGS += -Wl,--version-script=exports.txt -g
LDDYNFLAGS += -shared -static-libgcc -static-libstdc++
LIBS += -L/mingw32/lib SDK2/Libraries/Win/XPLM.lib lib32/libglew32.a -L./lib32/ -lhid -lsetupapi -lwsock32 -lopengl32 -lglu32 -lglew32
TARGET ?= win.xpl

else ifeq ($(OS), WINDOWS64)
#  sudo apt-get install  gcc-mingw-w64-x86-64 binutils-mingw-w64-x86-64
#  x86_64-w64-mingw32-gcc
CC ?= x86_64-w64-mingw32-gcc
INC += -ISDK/CHeaders/XPLM -ISDK/CHeaders/Widgets -I/mingw64/include -I./glew/glew-2.2.0/include
CFLAGS += -Wall -O2 -D$(OS) $(INC)
LDFLAGS += -Wl,--version-script=exports.txt
LDDYNFLAGS += -shared
LIBS += -L/mingw64/lib SDK/Libraries/Win/XPLM_64.lib -L./lib64/ -lhid -lsetupapi -lwsock32 -lopengl32 -lglu32 -lglew32
TARGET ?= win.xpl
endif

SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)



all: HUDplug.xpl 

OBJS = HUDplug.o display.o  statusDisplay.o  config.o

HUDplug.xpl: $(OBJ)
	$(CC) $(LDFLAGS) $(LDDYNFLAGS) $(OBJ) -o HUDplug.xpl $(LIBS)
	cp HUDplug.xpl $(TARGET)



# %.o: ./src/%.c HUDplug.h Makefile
# 	$(CC) $(CFLAGS) -c $<
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -f *.o $(OBJ_DIR)/*.o HUDplug.xpl linktest
