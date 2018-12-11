ifdef CONFIG_RBOOT_OTA
EXTRA_CFLAGS += -I$(srctree)/lib/rboot/ -I$(srctree)/lib/rboot/appcode/

# We are making rboot-api.c from the external project,
# so Kbuild won't create a build dir for it, so the build
# fails with
# "fatal error: opening dependency file lib/rboot/appcode/.rboot-api.o.d: No such file or directory"
# workaround this by creating the directory manually
$(obj)/rboot/appcode/rboot-api.o: \
	$(shell mkdir -p $(obj)/rboot/appcode)
endif

lib-$(CONFIG_RBOOT_OTA)		+= rboot-ota.o rboot/appcode/rboot-api.o

lib-$(CONFIG_WIFI)		+= wifi.o
