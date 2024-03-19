CC=avr-gcc
MCU=attiny85
TARGET=pwm
CPUFREQ=8000000UL

CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -DF_CPU=$(CPUFREQ)
LDFLAGS=-Wl,-gc-sections -Wl,-relax
# clock not divided
FUSEOPTS=-U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

PROGPORT=/dev/ttyACM0

all: $(TARGET).hex

clean:
	rm -f *.o *.elf *.hex

%.hex: %.elf
	avr-objcopy -R .eeprom -O ihex $< $@

$(TARGET).elf: $(TARGET).o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

program: $(TARGET).hex
	avrdude -v -C /etc/avrdude.conf -c stk500v2 -P "$(PROGPORT)" -p $(MCU) -U flash:w:$(TARGET).hex:i $(FUSEOPTS)
