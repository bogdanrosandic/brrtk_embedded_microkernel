BRRTK_ROOT_DIR =/home/qemu_code_sourcery/brrtk_qemu/brrtk_embedded_microkernel/trunk/BRRTK_SVN_BASE
 # Define the symbols we might want to change:
    CC	:= arm-none-linux-gnueabi-gcc -mthumb -mcpu=cortex-m3 -mthumb
#-mcpu=cortex-m3 -mthumb
    AS	:= arm-none-linux-gnueabi-as 
    LD	:= arm-none-linux-gnueabi-ld
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
	$(BRRTK_ROOT_DIR)/platform/qemu_cortex_m3/brrtk_platform_asm_keil.s 

    INCLUDES := -I$(BRRTK_ROOT_DIR)/app \
	-I$(BRRTK_ROOT_DIR)/brrtk \
	-I$(BRRTK_ROOT_DIR)/platform 
    LINCLUDES := -L/home/renesas/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_GNU_Linux/lib/gcc/arm-none-linux-gnueabi/4.6.3/ \
	-L/home/renesas/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_GNU_Linux/lib/gcc/arm-none-linux-gnueabi/4.6.3/thumb2/ \
	-L/home/renesas/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_GNU_Linux/arm-none-linux-gnueabi/libc/thumb2/usr/lib/
    OBJECTS	:= *.o
    
    my_program: $(OBJECTS)
	$(LD) $(LINCLUDES) $(LFLAGS) -o  my_program.elf $(OBJECTS) -lgcc -lc -lgcc_eh
    
    brrtk.o: $(SOURCE_BRRTK)
	$(CC) $(INCLUDES) $(CFLAGS)  $(SOURCE_BRRTK) 
    
#platform.o: $(SOURCE_PLATFORM)
#	$(AS) $(SOURCE_PLATFORM) -o platform.o


#$ 
#$ 
#$ arm-none-eabi-objcopy -O binary add.elf add.bin
