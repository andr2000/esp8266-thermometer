PREFIX              ?= .
# use wifi settings from environment or hard code them here
WIFI_SSID           ?= "ssid"
WIFI_PWD            ?= "pwd"

export ESPTOOL2     ?= $(PWD)/esptool2
export SPI_SIZE     ?= 1M
export SPI_MODE     ?= qio
export SPI_SPEED    ?= 40

FW_SECTS      = .text .data .rodata
FW_USER_ARGS  = -quiet -bin -boot2 -1024

BUILD_DIR = build
FIRMW_DIR = firmware
MAP_FILE  = output.map

CC ?= xtensa-lx106-elf-gcc

LIBS    = c gcc main hal phy net80211 lwip wpa pp crypto driver wpa2
CFLAGS  = -g -O2 -Wpointer-arith -Wundef -Werror -Wno-implicit -Wl,-EL
CFLAGS += -fno-inline-functions -nostdlib -mlongcalls  -mtext-section-literals
CFLAGS += -D__ets__ -DICACHE_FLASH -MMD
CFLAGS += -DSPI_SIZE_$(SPI_SIZE)
CFLAGS += -I$(SDK_BASE)/driver_lib/include -Irboot -Irboot/appcode
LDFLAGS = -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static
LDFLAGS += -Wl,-Map,$(MAP_FILE)

LIBS	:= $(addprefix -l,$(LIBS))

ifneq ($(WIFI_SSID), "")
	CFLAGS += -DWIFI_SSID=\"$(WIFI_SSID)\"
endif
ifneq ($(WIFI_PWD), "")
	CFLAGS += -DWIFI_PWD=\"$(WIFI_PWD)\"
endif

HEADERS = $(shell find . -name '*.h')
SOURCES = $(shell find . -not -path "./rboot/*" -name '*.c')
SOURCES += ./rboot/appcode/rboot-api.c
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)
DEPS = $(OBJECTS:%.o=%.d)
SUBDIRS = rboot

all: $(SUBDIRS) $(BUILD_DIR) $(FIRMW_DIR) $(FIRMW_DIR)/rom0.bin $(FIRMW_DIR)/rom1.bin

-include $(DEPS)

$(SUBDIRS):
	@echo "Making $@"
	$(MAKE) -C $@ $(MAKECMDGOALS)

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

clean: $(SUBDIRS)
	@echo "RM $(BUILD_DIR) $(FIRMW_DIR)"
	@rm -rf $(BUILD_DIR)
	@rm -rf $(FIRMW_DIR)
	@rm -f $(MAP_FILE)

.SECONDARY:
.PHONY: all clean $(SUBDIRS)
