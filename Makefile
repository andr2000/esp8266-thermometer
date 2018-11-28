# use wifi settings from environment or hard code them here
WIFI_SSID ?= "ssid"
WIFI_PWD  ?= "pwd"

ESPTOOL2     ?= ./esptool2

FW_SECTS      = .text .data .rodata
FW_USER_ARGS  = -quiet -bin -boot2 -1024

BUILD_DIR = build
FIRMW_DIR = firmware
MAP_FILE  = output.map

CC ?= xtensa-lx106-elf-gcc

LIBS    = c gcc main hal phy net80211 lwip wpa pp crypto driver
CFLAGS  = -g -O2 -Wpointer-arith -Wundef -Werror -Wno-implicit -Wl,-EL
CFLAGS += -fno-inline-functions -nostdlib -mlongcalls  -mtext-section-literals
CFLAGS += -D__ets__ -DICACHE_FLASH -MMD
CFLAGS += -I$(SDK_BASE)/driver_lib/include -Irboot
LDFLAGS = -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static
LDFLAGS += -Wl,-Map,$(MAP_FILE)

LIBS	:= $(addprefix -l,$(LIBS))

ifneq ($(WIFI_SSID), "")
	CFLAGS += -DWIFI_SSID=\"$(WIFI_SSID)\"
endif
ifneq ($(WIFI_PWD), "")
	CFLAGS += -DWIFI_PWD=\"$(WIFI_PWD)\"
endif

.SECONDARY:
.PHONY: all clean

HEADERS = $(shell find . -name '*.h')
SOURCES = $(shell find . -name '*.c')
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)
DEPS = $(OBJECTS:%.o=%.d)

all: $(BUILD_DIR) $(FIRMW_DIR) $(FIRMW_DIR)/rom0.bin $(FIRMW_DIR)/rom1.bin

-include $(DEPS)

$(BUILD_DIR)/%.o: %.c $(HEADERS)
	@echo "CC $<"
	mkdir -p $(@D)
	@$(CC) -I. $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.elf: $(OBJECTS)
	@echo "LD $(notdir $@)"
	@$(CC) -T$(notdir $(basename $@)).ld $(LDFLAGS) -Wl,--start-group $(LIBS) $^ -Wl,--end-group -o $@

$(FIRMW_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@echo "FW $(notdir $@)"
	@$(ESPTOOL2) $(FW_USER_ARGS) $^ $@ $(FW_SECTS)

$(DEPS_DIR)/%.d: ;
.PRECIOUS: $(DEPS_DIR)/%.d

include $(wildcard $(patsubst %,$(DEPS_DIR)/%.d,$(basename $(SOURCES))))

$(BUILD_DIR):
	@mkdir -p $@

$(FIRMW_DIR):
	@mkdir -p $@

clean:
	@echo "RM $(BUILD_DIR) $(FIRMW_DIR)"
	@rm -rf $(BUILD_DIR)
	@rm -rf $(FIRMW_DIR)
	@rm -f $(MAP_FILE)
