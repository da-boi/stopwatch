TARGET = stopwatch

CFLAGS = -Wall -g
CMMCU = atmega8

LFLAGS =

PROG = avrispv2
PPORT = -P /dev/ttyACM0
PBAUD = # -B 115200
PMMCU = m8
PFLAGS = -v

OBJECTS = $(addprefix ./target/,$(patsubst %.c,%.o,$(wildcard *.c)))


all: hex eeprom

hex: ./target/$(TARGET).hex

eeprom: ./target/$(TARGET).eep.hex

./target/$(TARGET).hex: ./target/$(TARGET).elf
	avr-objcopy -j .text -j .data -O ihex $< $@

./target/$(TARGET).eep.hex: ./target/$(TARGET).elf
	avr-objcopy -j .eeprom --change-section-lma .eeprom=1 -O ihex $< $@

./target/$(TARGET).elf: $(OBJECTS)
	avr-gcc $(LFLAGS) -mmcu=$(CMMCU) -o $@ $^

./target/%.o: %.c
	mkdir -p ./target/
	avr-gcc -c $(CFLAGS) -mmcu=$(CMMCU) -o $@ $<

size: ./target/$(TARGET).elf
	avr-size --mcu=$(CMMCU) -C $<

clean:
	rm -R ./target

run: .target/$(TARGET).hex
	avrdude $(PFLAGS) -c $(PROG) $(PPORT) $(PBAUD) -p $(PMMCU) \
		-U lfuse:w:0x7F:m \
		-U flash:w:$<

.PHONY: \
	all \
	clean \
	eeprom \
	hex \
	run \
	size

#{{{ dependencies
./target/display.o:	display.c display.h
./target/input.o:	input.c input.h
./target/main.o:	main.c
./target/strutils.o:	strutils.c strutils.h
./target/time.o:	time.c time.h
#}}}
