CC=avr-gcc
MCU=atmega32u4
TARGET=blink
#CPUFREQ=16000000UL
CPUFREQ=2000000UL # clock divided by 8

CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -DF_CPU=$(CPUFREQ)
LDFLAGS=-Wl,-gc-sections -Wl,-relax
#FUSEOPTS=-U lfuse:w:0xde:m -U hfuse:w:0xd9:m # undivided clock
FUSEOPTS=-U lfuse:w:0x5e:m -U hfuse:w:0xd9:m # clock divided by 8

PROGPORT=/dev/ttyACM0

all: $(TARGET).hex

clean:
	rm -f *.o *.elf *.hex

%.hex: %.elf
	avr-objcopy -R .eeprom -O ihex $< $@

$(TARGET).elf: $(TARGET).o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

program: $(TARGET).hex
	avrdude -c stk500v2 -P "$(PROGPORT)" -p $(MCU) -U flash:w:$(TARGET).hex:i $(FUSEOPTS)
