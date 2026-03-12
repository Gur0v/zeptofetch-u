.POSIX:
CC      = clang
CFLAGS  = -Os -flto=full -fno-stack-protector -fomit-frame-pointer \
          -fno-unwind-tables -fno-asynchronous-unwind-tables \
          -ffunction-sections -fdata-sections -fmerge-all-constants \
          -fno-ident -fvisibility=hidden -Wno-everything -DNDEBUG
LDFLAGS = -flto=full -Wl,--gc-sections -Wl,--strip-all \
          -Wl,--as-needed -Wl,-z,norelro -Wl,--hash-style=gnu \
          -Wl,--build-id=none -static -s
PREFIX  ?= /usr/local
BINDIR  = $(PREFIX)/bin
TARGET  = zeptofetch-u
SRC     = zeptofetch-u.c

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
	@ls -lh $@

clean:
	rm -f $(TARGET)

install: $(TARGET)
	mkdir -p $(DESTDIR)$(BINDIR)
	install -m755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)
