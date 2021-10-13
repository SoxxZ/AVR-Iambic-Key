MCU=atmega328P
FREQ=8000000
PORT=/dev/ttyUSB0

C_SOURCE=$(shell find src/ -type f -name '*.c')
CPP_SOURCE=$(shell find src/ -type f -name '*.cpp')
S_SOURCE=$(shell find src/ -type f -name '*.s')
H_SOURCE=$(shell find src/ -type f -name '*.h')

C_OBJ=$(C_SOURCE:.c=.o)
CPP_OBJ=$(CPP_SOURCE:.cpp=.o)
S_OBJ=$(S_SOURCE:.s=.o)

# -g = debug info
CC_FLAGS=-Wall -Werror -Os -g
CC_DEFINES=-DF_CPU=$(FREQ)UL 
SC_FLAGS=-Wall


all: image.hex

image.hex: $(C_OBJ) $(CPP_OBJ) $(S_OBJ)
	avr-gcc -mmcu=$(MCU) -o image.elf $^
	avr-objcopy -j .text -j .data -O ihex image.elf image.hex

%.o: %.c $(H_SOURCE)
	avr-gcc $(CC_FLAGS) -mmcu=$(MCU) $(CC_DEFINES) -c -o $@ $<

%.o: %.s
	avr-as $(SC_FLAGS) -mmcu=$(MCU) -c -o $@ $^

clean:
	rm -rf image.elf image.hex $(C_OBJ) $(S_OBJ) *.hex *~

upload:
	avrdude -c usbasp -p $(MCU) -b 115200 -u -U flash:w:image.hex
