GIT_VERSION = $(shell cd $(srctree) && git describe --long --dirty --always --tags)

EXTRA_CFLAGS += -DVERSION=\"$(GIT_VERSION)\"

obj-y := thing.o
