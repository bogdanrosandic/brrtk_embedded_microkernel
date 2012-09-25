BRRTK_ROOT_DIR =/home/qemu_code_sourcery/brrtk_qemu/brrtk_embedded_microkernel/trunk/BRRTK_SVN_BASE/
 # Define the symbols we might want to change:
    CC	:= arm-none-linux-gnueabi-gcc -mthumb -mcpu=cortex-m3
#-mcpu=cortex-m3 -mthumb
    AS	:= arm-none-linux-gnueabi-as -mcpu=cortex-m3
    LD	:= arm-none-linux-gnueabi-ld
    CFLAGS := -g 
    LFLAGS := -Ttext=0x0 
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
	-I$(BRRTK_ROOT_DIR)/platform \
     
	
    OBJECTS	:= brrtk.o platform.o
    
#    my_program: $(OBJECTS)
#	$(LD) $(CFLAGS) -o my_program.elf $(OBJECTS)
    
    brrtk.o: $(SOURCE_BRRTK)
	$(CC) $(INCLUDES) $(CFLAGS)  $(SOURCE_BRRTK) -o brrtk.o 
    
#platform.o: $(SOURCE_PLATFORM)
#	$(AS) $(SOURCE_PLATFORM) -o platform.o


#$ 
#$ 
#$ arm-none-eabi-objcopy -O binary add.elf add.bin
