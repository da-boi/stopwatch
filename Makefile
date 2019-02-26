TARGET = stopwatch

CFLAGS = -Wall -g
CMMCU = atmega328p

LFLAGS =

PROG = avrispv2
PPORT = -P /dev/ttyACM0
PBAUD = # -B 115200
PMMCU = m328p
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

run: ./target/$(TARGET).hex
		avrdude $(PFLAGS) -c $(PROG) $(PPORT) $(PBAUD) -p $(PMMCU) -U flash:w:$<

.PHONY: size clean run

#{{{ dependencies
./target/display.o:		display.c display.h
./target/main.o:		main.c
./target/time.o:		time.c time.h
#}}}