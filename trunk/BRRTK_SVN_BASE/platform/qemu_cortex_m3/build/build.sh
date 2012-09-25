#!/bin/bash  
export PATH=/home/bogdan/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_GNU_Linux/bin:$PATH

make brrtk.o
#make platform.o
make my_program
