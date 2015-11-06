
# Put your stlink folder here so make burn will work.
STLINK=~/stlink.git

SRCS=main.c system_stm32f4xx.c stm32f4xx_it.c

# Library modules
SRCS += stm32f4xx_gpio.c stm32f4xx_rcc.c stm32f4xx_tim.c stm32f4xx_usart.c 
SRCS += misc.c stm32f4xx_flash.c stm32f4xx_exti.c stm32f4xx_dma.c stm32f4xx_adc.c 
SRCS += stm32f4xx_syscfg.c adc_dma.c common.c pid.c
# User Defined modules 
SRCS += timer.c pwm.c motor.c usart.c strategy.c adc.c infrared.c
#SRCS += usart_dma.c

# Binaries will be generated with this name (.elf, .bin, .hex, etc)
PROJ_NAME=PaperCut

#######################################################################################

STM_COMMON=../../..

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS  = -g -O2 -Wall -Tstm32_flash.ld 
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -I.
#CFLAGS += -l"/usr/local/gcc-arm-none-eabi-4_9-2015q3/arm-none-eabi/lib/armv7-m/libc.a"
#CFLAGS += -l/usr/local/gcc-arm-none-eabi-4_9-2015q3/arm-none-eabi/lib/armv7-m/librdimon.a
#CFLAGS += --specs=rdimon.specs -lgcc -lc -lm -lrdimon -Wl,--end-group

# Include files from STM libraries
CFLAGS += -I$(STM_COMMON)/Utilities/STM32F4-Discovery
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Include 
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc
#此条语句用于添加某些优化库以使printf能够使用
CFLAGS +=  -specs=nosys.specs 

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
	#$(STLINK)/st-flash write $(PROJ_NAME).bin 0x8000000
	st-flash --reset write $(PROJ_NAME).bin 0x8000000
