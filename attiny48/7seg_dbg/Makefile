CC=avr-gcc
MCU=attiny48
TARGET=7segdebug
CPUFREQ=8000000UL

CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -DF_CPU=$(CPUFREQ)
LDFLAGS=-Wl,-gc-sections -Wl,-relax
# clock not divided
FUSEOPTS=-U lfuse:w:0xee:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

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
