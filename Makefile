
ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -std=c11 -lgcc
LOPS = -L/home/sundeep/lin/lib/gcc/arm-none-eabi/4.9.2/ -lgcc

gcc : app.hex app.bin

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc

crt0.o : crt0.s
	$(ARMGNU)-as crt0.s -o crt0.o

gpio.o : gpio.c
	$(ARMGNU)-gcc $(COPS) -c gpio.c -o gpio.o

printf.o : printf.c
	$(ARMGNU)-gcc $(COPS) -c printf.c -o printf.o

uart.o : uart.c
	$(ARMGNU)-gcc $(COPS) -c uart.c -o uart.o

startup.o : startup.c
	$(ARMGNU)-gcc $(COPS) -c startup.c -o startup.o

app.o : app.c
	$(ARMGNU)-gcc $(COPS) -c app.c -o app.o

string.o : string.c
	$(ARMGNU)-gcc $(COPS) -c string.c -o string.o

app.elf : lscript crt0.o gpio.o printf.o uart.o startup.o app.o string.o
	$(ARMGNU)-ld crt0.o startup.o gpio.o printf.o uart.o app.o string.o -T lscript -o app.elf $(LOPS)
	$(ARMGNU)-objdump -D app.elf > app.list

app.bin : app.elf
	$(ARMGNU)-objcopy app.elf -O binary app.bin

app.hex : app.elf
	$(ARMGNU)-objcopy app.elf -O ihex app.hex
