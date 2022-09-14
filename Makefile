SOURCE_DIR ?= src
INCLUDE_DIR ?= include
BUILD_DIR ?= build

Hook:
	clear
	mkdir -p $(BUILD_DIR)
	g++ $(INCLUDE_DIR)/*.c $(SOURCE_DIR)/*.cpp -I $(INCLUDE_DIR) -W -Wall -DMG_ENABLE_IPV6=1 -DMG_ENABLE_LINES=1 -D_POSIX_C_SOURCE=200000L -lws2_32 -o $(BUILD_DIR)/Hook.exe