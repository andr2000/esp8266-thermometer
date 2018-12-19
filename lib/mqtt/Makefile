ccflags-y += -I$(srctree)/$(src)/../config
ccflags-y += -Wno-switch -Wno-pointer-sign -Wno-format

mqtt_impl-objs := mqtt.o mqtt_msg.o proto.o queue.o ringbuf.o utils.o

obj-y += mqtt_impl.o $(mqtt_impl-objs)
