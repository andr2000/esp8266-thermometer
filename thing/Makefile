GIT_VERSION = $(shell cd $(srctree) && git describe --long --dirty --always --tags)

ccflags-y += -DVERSION=\"$(GIT_VERSION)\"

obj-y := thing.o
