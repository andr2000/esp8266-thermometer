# use wifi settings from environment or hard code them here
WIFI_SSID ?= ""
WIFI_PWD  ?= ""

ESPTOOL2     ?= ./esptool2

FW_SECTS      = .text .data .rodata
FW_USER_ARGS  = -quiet -bin -boot2

BUILD_DIR = build
FIRMW_DIR = firmware

CC ?= xtensa-lx106-elf-gcc

LIBS    = c gcc main hal phy net80211 lwip wpa pp crypto
CFLAGS  = -g -O2 -Wpointer-arith -Wundef -Werror -Wno-implicit -Wl,-EL
CFLAGS += -fno-inline-functions -nostdlib -mlongcalls  -mtext-section-literals
CFLAGS += -D__ets__ -DICACHE_FLASH
CFLAGS += -Irboot
LDFLAGS = -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static

LIBS	:= $(addprefix -l,$(LIBS))

ifneq ($(WIFI_SSID), "")
	CFLAGS += -DWIFI_SSID=\"$(WIFI_SSID)\"
endif
ifneq ($(WIFI_PWD), "")
	CFLAGS += -DWIFI_PWD=\"$(WIFI_PWD)\"
endif

.SECONDARY:
.PHONY: all clean

C_FILES = $(wildcard *.c)
C_FILES += $(wildcard rboot/*.c)
O_FILES = $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_FILES))

all: $(BUILD_DIR) $(FIRMW_DIR) $(FIRMW_DIR)/rom0.bin $(FIRMW_DIR)/rom1.bin

$(BUILD_DIR)/%.o: %.c
	@echo "CC $<"
	@$(CC) -I. $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.elf: $(O_FILES)
	@echo "LD $(notdir $@)"
	@$(CC) -T$(notdir $(basename $@)).ld $(LDFLAGS) -Wl,--start-group $(LIBS) $^ -Wl,--end-group -o $@

$(FIRMW_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@echo "FW $(notdir $@)"
	@$(ESPTOOL2) $(FW_USER_ARGS) $^ $@ $(FW_SECTS)

$(BUILD_DIR):
	@mkdir -p $@
	@mkdir -p $@/rboot

$(FIRMW_DIR):
	@mkdir -p $@

clean:
	@echo "RM $(BUILD_DIR) $(FIRMW_DIR)"
	@rm -rf $(BUILD_DIR)
	@rm -rf $(FIRMW_DIR)
