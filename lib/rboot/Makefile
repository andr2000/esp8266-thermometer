ccflags-y += -I$(srctree)/external/rboot/
ccflags-y += -I$(srctree)/external/rboot/appcode/

# We are making rboot-api.c from the external project,
# so Kbuild won't create a build dir for it, so the build
# fails with
# "fatal error: opening dependency file lib/rboot/appcode/.rboot-api.o.d: No such file or directory"
# workaround this by creating the directory manually
$(obj)/../../external/rboot/appcode/rboot-api.o: \
	$(shell mkdir -p $(obj)/../../external/rboot/appcode)

obj-y += rboot-ota.o ../../external/rboot/appcode/rboot-api.o
