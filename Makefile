CC = cl
CFLAGS = /c /I "include" /O2 /GS- /DNDEBUG
LDFLAGS = kernel32.lib user32.lib
SRC_DIR = src
BUILD_DIR = bin

$(BUILD_DIR)/%.exe: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(SRC_DIR)/$*.c /Fo$(BUILD_DIR)/$*.obj
	crinkler $(BUILD_DIR)/$*.obj /OUT:$@ $(LDFLAGS)


all: start $(BUILD_DIR)/a.exe $(BUILD_DIR)/c.exe $(BUILD_DIR)/d.exe clean

start:
	mkdir -p $(BUILD_DIR)

clean:
	rm bin/*.obj

