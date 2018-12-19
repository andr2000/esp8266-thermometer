obj-$(CONFIG_MQTT)		+= mqtt/
obj-$(CONFIG_RBOOT_OTA)		+= rboot/
obj-$(CONFIG_WIFI)		+= wifi/

# For simplicity link all into a single library, e.g. add built-in.o:
# by default each folder above will produce a built-in.o module which will
# not go into the lib/lib.a
lib-y += built-in.o
