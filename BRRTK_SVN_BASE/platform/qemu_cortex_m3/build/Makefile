BRRTK_ROOT_DIR =/home/qemu_code_sourcery/brrtk_qemu/brrtk_embedded_microkernel/trunk/BRRTK_SVN_BASE/
 # Define the symbols we might want to change:
    CC	:= arm-none-linux-gnueabi-gcc -mcpu=cortex-m3
    AS	:= arm-none-linux-gnueabi-as -mcpu=cortex-m3
    LD	:= arm-none-linux-gnueabi-ld
    CFLAGS := -g 
    LFLAGS := -Ttext=0x0 
    SOURCE_BRRTK    := 
    SOURCE_PLATFORM := 
    OBJECTS	:= brrtk.o platform.o
    
    my_program: $(OBJECTS)
    	$(LD) $(CFLAGS) -o my_program.elf $(OBJECTS)
    
    brrtk.o: $(SOURCE_BRRTK)
    	$(CC) $(INCLUDES) $(CFLAGS) -c $(SOURCE_BRRTK) -o brrtk.o
    
    platform.o: $(SOURCE_PLATFORM)
    	$(AS) $(SOURCE_PLATFORM) -o platform.o


#$ 
#$ 
#$ arm-none-eabi-objcopy -O binary add.elf add.bin
