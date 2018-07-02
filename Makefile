
# Put your stlink folder here so make burn will work.
STLINK=~/stlink/build/Release

# Put your source files here (or *.c, etc)
SRCS=main.c system_stm32f4xx.c stm32f4xx_it.c 
SRCS += stm32f4_discovery_lcd.c fonts.c stm32f4_discovery.c
SRCS += stm32f4xx_adc.c
SRCS += stm32f4xx_can.c
SRCS += stm32f4xx_crc.c
SRCS += stm32f4xx_cryp.c
SRCS += stm32f4xx_dac.c
SRCS += stm32f4xx_dbgmcu.c
SRCS += stm32f4xx_dcmi.c
SRCS += stm32f4xx_dma.c
SRCS += stm32f4xx_exti.c
SRCS += stm32f4xx_flash.c
SRCS += stm32f4xx_fsmc.c
SRCS += stm32f4xx_hash.c
SRCS += stm32f4xx_gpio.c
SRCS += stm32f4xx_i2c.c
SRCS += stm32f4xx_iwdg.c
SRCS += stm32f4xx_pwr.c
SRCS += stm32f4xx_rcc.c
SRCS += stm32f4xx_rng.c
SRCS += stm32f4xx_rtc.c
SRCS += stm32f4xx_sdio.c
SRCS += stm32f4xx_spi.c
SRCS += stm32f4xx_syscfg.c
SRCS += stm32f4xx_tim.c
SRCS += stm32f4xx_usart.c
SRCS += stm32f4xx_wwdg.c
SRCS += misc.c


# Binaries will be generated with this name (.elf, .bin, .hex, etc)
PROJ_NAME=screen

# Put your STM32F4 library code directory here
STM_COMMON=../STM32F4-Discovery_FW_V1.1.0

# Normally you shouldn't need to change anything below this line!
#######################################################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS  = -g -O2 -Wall -Tstm32_flash.ld
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -I.

# Include files from STM libraries
CFLAGS += -I$(STM_COMMON)/Utilities/STM32F4-Discovery
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Include 
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc

# add startup file to build
SRCS += $(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO/startup_stm32f4xx.s 

OBJS = $(SRCS:.c=.o)

vpath %.c $(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/src 

.PHONY: proj

all: proj

proj: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ 
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin

# Flash the STM32F4
burn: proj
	$(STLINK)/st-flash write $(PROJ_NAME).bin 0x8000000
