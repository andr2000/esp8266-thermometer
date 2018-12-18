ccflags-y +=-Wno-switch -Wno-pointer-sign

ifdef CONFIG_MQTT
ccflags-y += -I$(srctree)/$(src)
endif

mqtt_impl-objs := mqtt.o mqtt_msg.o proto.o queue.o ringbuf.o utils.o

obj-$(CONFIG_MQTT) += mqtt_impl.o $(mqtt_impl-objs)
