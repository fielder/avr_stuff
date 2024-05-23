CC=avr-gcc
MCU=atmega32u4
TARGET=caak
CPUFREQ=16000000UL

CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -DF_CPU=$(CPUFREQ)
LDFLAGS=-Wl,-gc-sections -Wl,-relax
FUSEOPTS=-U lfuse:w:0xde:m -U hfuse:w:0xd9:m # undivided clock

PROGPORT=/dev/ttyACM0

OBJS =	./main.o \
	./usb.o \
	./matrix.o

all: $(TARGET).hex

clean:
	rm -f *.o *.elf *.hex

%.hex: %.elf
	avr-objcopy -R .eeprom -O ihex $< $@

$(TARGET).elf: main.o usb.o matrix.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

./main.o: main.c
	$(CC) -c $(CFLAGS) $? -o $@
./usb.o: usb.c
	$(CC) -c $(CFLAGS) $? -o $@
./matrix.o: matrix.c
	$(CC) -c $(CFLAGS) $? -o $@

program: $(TARGET).hex
	avrdude -v -C /etc/avrdude.conf -c stk500v2 -P "$(PROGPORT)" -p $(MCU) -U flash:w:$(TARGET).hex:i $(FUSEOPTS)
