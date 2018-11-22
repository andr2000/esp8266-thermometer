CC ?= xtensa-lx106-elf-gcc
CFLAGS ?= -I. -mlongcalls
LDLIBS ?= -nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc
LDFLAGS ?= -Teagle.app.v6.ld

thermometer-0x00000.bin: thermometer
	esptool.py elf2image $^

thermometer: thermometer.o

thermometer.o: thermometer.c

flash: thermometer-0x00000.bin
	esptool.py write_flash 0 thermometer-0x00000.bin 0x10000 thermometer-0x10000.bin

clean:
	rm -f thermometer thermometer.o thermometer-0x00000.bin thermometer-0x10000.bin
