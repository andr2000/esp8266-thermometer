hostprogs-y := esptool2

esptool2-objs := esptool2src/esptool2.o esptool2src/esptool2_elf.o

HOSTCFLAGS += -Wno-missing-prototypes

always := $(hostprogs-y)

extra-$(CONFIG_RBOOT_OTA) += rboot

external/rboot: external/esptool2 FORCE
	$(Q)export ESPTOOL2=$(CURDIR)/external/esptool2 ; \
	export RBOOT_BUILD_BASE=$(CURDIR)/external/rboot/build ; \
	export RBOOT_FW_BASE=$(CURDIR)/external/rboot/firmware ; \
	$(MAKE) -C $(srctree)/external/rboot

clean-dirs := $(CURDIR)/external/rboot/build
clean-dirs += $(CURDIR)/external/rboot/firmware
