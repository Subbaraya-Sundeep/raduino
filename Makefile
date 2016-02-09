TARGET := app
C_SRCS := $(wildcard *.c)
A_SRCS := $(wildcard *.S)
C_OBJS := ${C_SRCS:.c=.o}
C_OBJS += ${A_SRCS:.S=.o}

INCLUDE_DIRS := ./include
CC := arm-none-eabi-gcc
AS := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump

CPPFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))
CFLAGS += -Wall -O2 -ffreestanding -fno-delete-null-pointer-checks -std=c11 -march=armv7-a -mtune=cortex-a7
ASFLAGS += -c

all: $(TARGET).elf $(TARGET).bin $(TARGET).hex $(TARGET).list

$(TARGET).elf: $(C_OBJS)
	$(CC) $(C_OBJS) -o $(TARGET).elf -T lscript -nostdlib -nostartfiles -lgcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc

$(TARGET).list : $(TARGET).elf
	$(OBJDUMP) -D $(TARGET).elf > $(TARGET).list

$(TARGET).bin : $(TARGET).elf
	$(OBJCOPY) $(TARGET).elf -O binary $(TARGET).bin

$(TARGET).hex : $(TARGET).elf
	$(OBJCOPY) $(TARGET).elf -O ihex $(TARGET).hex
