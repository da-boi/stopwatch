TARGET = stopwatch

CFLAGS = -Wall -Os
CMMCU = atmega8

LFLAGS =

PROG = avrispv2
PPORT = -P /dev/ttyACM0
PBAUD = # -B 115200
PMMCU = m8
PFLAGS = -v

OBJECTS = $(addprefix ./target/,$(patsubst %.c,%.o,$(wildcard *.c)))


all: hex fuses run

hex: ./target/$(TARGET).hex

./target/$(TARGET).hex: ./target/$(TARGET).elf
	avr-objcopy -j .text -j .data -O ihex $< $@

./target/$(TARGET).elf: $(OBJECTS)
	avr-gcc $(LFLAGS) -mmcu=$(CMMCU) -o $@ $^

./target/%.o: %.c
	mkdir -p ./target/
	avr-gcc -c $(CFLAGS) -mmcu=$(CMMCU) -o $@ $<

size: ./target/$(TARGET).elf
	avr-size --mcu=$(CMMCU) -C $<

clean:
	rm -R ./target/

run: ./target/$(TARGET).hex
	avrdude $(PFLAGS) -c $(PROG) $(PPORT) $(PBAUD) -p $(PMMCU) -U flash:w:$<

fuses:
	avrdude $(PFLAGS) -c $(PROG) $(PPORT) $(PBAUD) -p $(PMMCU) \
		-U lfuse:w:0xEF:m \
		-U hfuse:w:0xD9:m


.PHONY: \
	all \
	clean \
	eeprom \
	fuses \
	hex \
	run \
	size

#{{{ dependencies
./target/display.o:	display.c display.h
./target/input.o:	input.c input.h
./target/highscore.o:	highscore.c highscore.h
./target/main.o:	main.c display.h input.h strutils.h time.h
./target/strutils.o:	strutils.c strutils.h
./target/time.o:	time.c time.h
#}}}
