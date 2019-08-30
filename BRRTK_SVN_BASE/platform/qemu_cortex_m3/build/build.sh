#!/bin/bash  
export PATH=/home/bogdan/Downloads/gcc-arm-none-eabi-8-2019-q3-update-linux/gcc-arm-none-eabi-8-2019-q3-update/bin:$PATH

#make brrtk.o
#make platform.o
make clean
make my_program

