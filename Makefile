TARGET := app
C_SRCS := $(wildcard *.c)
A_SRCS := $(wildcard *.s)
C_OBJS := ${C_SRCS:.c=.o}
C_OBJS += ${A_SRCS:.s=.o}

INCLUDE_DIRS := ./include
CC := arm-xilinx-eabi-gcc
AS := arm-xilinx-eabi-as
OBJCOPY := arm-xilinx-eabi-objcopy
OBJDUMP := arm-xilinx-eabi-objdump

CPPFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))
CFLAGS += -Wall -O2 -nostdlib -nostartfiles -ffreestanding -fno-delete-null-pointer-checks -std=c11 -lgcc
ASFLAGS += --warn --fatal-warnings

all: $(TARGET).elf $(TARGET).bin $(TARGET).hex $(TARGET).list

$(TARGET).elf: $(C_OBJS)
	$(CC) $(C_OBJS) -o $(TARGET).elf -T lscript

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
