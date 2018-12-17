#!/bin/bash

TARGET="slave"
MCU="atmega328p"
# clock not divided
CPUFREQ="8000000UL"
I2C_SLAVE_ADDR="100"
LFU="0xe2"
HFU="0xd9"
EFU="0xff"

SOURCES="slave.c
	avr_slave.c"

OBJDIR="obj"
CC="avr-gcc"
CFLAGS="-g -Wall -mcall-prologues -mmcu=${MCU} -Os -DF_CPU=${CPUFREQ} -DI2C_SLAVE_ADDR=${I2C_SLAVE_ADDR}"
LDFLAGS="-Wl,-gc-sections -Wl,-relax"

HEXFILE="${OBJDIR}/${TARGET}.hex"
ELFFILE="${OBJDIR}/${TARGET}.elf"

mkdir -p ${OBJDIR}

OBJS=""
for f in ${SOURCES}; do
	of="${OBJDIR}/`basename ${f} .c`.o"
	${CC} ${CFLAGS} -c ${f} -o ${of}
	OBJS="${OBJS} ${of}"
done

${CC} ${CFLAGS} ${LDFLAGS} ${OBJS} -o ${ELFFILE}

avr-objcopy -R .eeprom -O ihex ${ELFFILE} ${HEXFILE}

if [ "$1" == "program" ]; then
	PROGPORT=`find /dev/serial/by-id/ -name '*if01'`
	if [ -z "${PROGPORT}" ]; then
		echo "unable to find programming device"
		exit 1
	fi

	if [ ! -f "${HEXFILE}" ]; then
		echo "no hex file"
		exit 1
	fi

	FUSEOPTS="-U lfuse:w:${LFU}:m -U hfuse:w:${HFU}:m -U efuse:w:${EFU}:m"

	sudo avrdude -c stk500v2 -P ${PROGPORT} -p ${MCU} -U flash:w:${HEXFILE}:i ${FUSEOPTS}
fi
