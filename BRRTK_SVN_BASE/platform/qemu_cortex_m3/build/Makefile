#MODIFY PATHS
BRRTK_ROOT_DIR =/home/bogdan/brrtk_svn/brrtk_embedded_microkernel/trunk/BRRTK_SVN_BASE
CODE_SOURCERY =/home/bogdan/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_EABI

# Define the symbols we might want to change:
    CC	:= arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -mthumb
    AS	:= arm-none-eabi-as 
    LD	:= arm-none-eabi-ld
    CFLAGS := -g -c 
    LFLAGS := -T brrtk.ld  
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
    LINCLUDES := -L/home/bogdan/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_EABI/lib/gcc/arm-none-eabi/4.6.3/ \
	-L$(CODE_SOURCERY)/lib/gcc/arm-none-eabi/4.6.3/thumb2/ \
	-L$(CODE_SOURCERY)/arm-none-eabi/libc/thumb2/usr/lib/ \
	-L$(CODE_SOURCERY)/arm-none-eabi/lib/thumb2/
	
      
    OBJECTS	:= *.o $(CODE_SOURCERY)/lib/gcc/arm-none-eabi/4.6.3/thumb2/crti.o
    
    my_program: $(OBJECTS)
	$(LD) $(LINCLUDES) $(LFLAGS) -o  my_program.elf $(OBJECTS) 
    
    brrtk.o: $(SOURCE_BRRTK)
	$(CC) $(INCLUDES) $(CFLAGS)  $(SOURCE_BRRTK) 
    
#platform.o: $(SOURCE_PLATFORM)
#	$(AS) $(SOURCE_PLATFORM) -o platform.o


#$ 
#$ 
#$ arm-none-eabi-objcopy -O binary add.elf add.bin

