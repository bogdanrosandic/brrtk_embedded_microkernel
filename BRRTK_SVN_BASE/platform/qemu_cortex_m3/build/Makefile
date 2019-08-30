#MODIFY PATHS
BRRTK_ROOT_DIR =/home/bogdan/BOGDAN/Projects/brrtk_embedded_microkernel/BRRTK_SVN_BASE
CODE_SOURCERY =/home/bogdan/Downloads/gcc-arm-none-eabi-8-2019-q3-update-linux/gcc-arm-none-eabi-8-2019-q3-update

# Define the symbols we might want to change:
    CC	:= arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -mthumb
    AS	:= arm-none-eabi-as 
    LD	:= arm-none-eabi-ld
    CFLAGS :=  -g -D__STARTUP_CLEAR_BSS -D__START=main -Os  -flto -ffunction-sections -fdata-sections -fno-builtin 
    LFLAGS := --specs=nano.specs --specs=nosys.specs 
    SOURCE_BRRTK    := $(BRRTK_ROOT_DIR)/brrtk/brrtk_utilities.c \
	$(BRRTK_ROOT_DIR)/brrtk/brrtk_task.c \
	$(BRRTK_ROOT_DIR)/brrtk/mailbox.c \
	$(BRRTK_ROOT_DIR)/brrtk/scheduler.c \
	$(BRRTK_ROOT_DIR)/brrtk/semaphore.c \
	$(BRRTK_ROOT_DIR)/platform/qemu_cortex_m3/brrtk_platform.c \
	$(BRRTK_ROOT_DIR)/examples/qemu_cortex_m3/periodic_task_example/main.c \
	$(BRRTK_ROOT_DIR)/examples/qemu_cortex_m3/periodic_task_example/startup_keil.s \
	$(BRRTK_ROOT_DIR)/platform/qemu_cortex_m3/brrtk_platform_asm_keil.s \
	
	
    INCLUDES := -I$(BRRTK_ROOT_DIR)/app \
	-I$(BRRTK_ROOT_DIR)/brrtk \
	-I$(BRRTK_ROOT_DIR)/platform 
    LINCLUDES := -L$(CODE_SOURCERY)/share/gcc-arm-none-eabi/samples/ldscripts -T gcc.ld -Wl,--gc-sections -Wl,-Map=my_program.map
	
      
    #OBJECTS	:= *.o #$(CODE_SOURCERY)/arm-none-eabi/lib/gcc/arm-none-eabi/8.3.1/thumb/v7-m/nofp/libgcc.a
    #my_program: $(OBJECTS)
	#$(LD) $(LINCLUDES) $(LFLAGS) -o  my_program.elf $(OBJECTS) 
    
    my_program: $(SOURCE_BRRTK) 
	$(CC) $(INCLUDES) $^   $(CFLAGS) $(LINCLUDES) $(LFLAGS)  -o  $@
    
#platform.o: $(SOURCE_PLATFORM)
#	$(AS) $(SOURCE_PLATFORM) -o platform.o


#$ 
#$ 
#$ arm-none-eabi-objcopy -O binary add.elf add.bin

