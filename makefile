PNAME = stopwatch

CFLAGS = -Os
CMMCU = atmega328p

LFLAGS =

PROG = avrispv2
PPORT = /dev/ttyACM0
PMMCU = m328p
PFLAGS = -v

OBJ = main.o display.o time.o


prog: $(PNAME).hex
	avrdude -c $(PROG) -P $(PPORT) -p $(PMMCU) -U flash:w:$< $(PFLAGS)

$(PNAME).hex: $(PNAME).elf
	avr-objcopy -j .text -j .data -O ihex $< $@

$(PNAME).elf: $(OBJ)
	avr-gcc -mmcu=$(CMMCU) -o $@ $^ $(LFLAGS)

%.o: %.c %.h
	avr-gcc -mmcu=$(CMMCU) -c -o $@ $< $(CFLAGS)

