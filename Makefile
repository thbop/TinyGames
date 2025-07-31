CC = cl
CFLAGS = /c /I "include" /O1 /GS- /DNDEBUG
LDFLAGS = kernel32.lib user32.lib
SRC_DIR = src
BUILD_DIR = bin

%.exe:
	$(CC) $(CFLAGS) $(SRC_DIR)/$*.c /Fo$(BUILD_DIR)/$*.obj
	crinkler $(BUILD_DIR)/$*.obj /OUT:$(BUILD_DIR)/$@ $(LDFLAGS)


all: start a.exe c.exe

start:
	mkdir -p $(BUILD_DIR)

