CC = cl
CFLAGS = /c /I "include" /O1 /GS- /DNDEBUG
GCFLAGS = -nostdlib -ffreestanding -Wl,-e,main -Iinclude -O3
LDFLAGS = kernel32.lib user32.lib
GLDFLAGS = -lkernel32 -luser32
SRC_DIR = src
BUILD_DIR = bin

$(BUILD_DIR)/%.exe: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(SRC_DIR)/$*.c /Fo$(BUILD_DIR)/$*.obj
	crinkler $(BUILD_DIR)/$*.obj /OUT:$@ $(LDFLAGS)

$(BUILD_DIR)/d.exe: $(SRC_DIR)/d.c
	gcc $(GCFLAGS) src/d.c -o bin/d.exe $(GLDFLAGS)

all: start $(BUILD_DIR)/a.exe $(BUILD_DIR)/c.exe $(BUILD_DIR)/d.exe clean

start:
	mkdir -p $(BUILD_DIR)

clean:
	rm bin/*.obj

