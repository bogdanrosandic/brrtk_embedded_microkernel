#!/bin/bash  
export PATH=/home/renesas/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_EABI/bin:$PATH

make brrtk.o
#make platform.o
make my_program
