# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# pkg-config setup
PKG_CONFIG = pkg-config

# Music 
LIBNAME = gstreamer-1.0 # Main audio handler
LIBNAME += taglib_c libsixel ncurses# Metadata parser + display
LIBNAME += glib-2.0 gio-2.0 # Mpris integration

CFLAGS += $(shell $(PKG_CONFIG) --cflags $(LIBNAME))
LDLIBS = $(shell $(PKG_CONFIG) --libs $(LIBNAME))

# Targets
TARGET = lyrica
SRC = $(wildcard src/*.c)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
