/*

 * This file is part of the BRRTK source code.
 * 
 * Author: Bogdan Rosandic <bogdanrosandic@gmail.com>
 *
 */


#ifndef __BRRTK_APP_H__
#define __BRRTK_APP_H__

/*---------------------------------------
    This file should  include  code 
    or definitions that the application programmer 
    might need to modify
 ----------------------------------------*/

/* 
    Macro defines how much tasks user can create 
 */
#define MAXTASKS 6

/* 
    Defines size in corresponding memory pool for 
    aperiodic timing structures
 */
#define MAX_TIME_APER 1

/* 
    Defines size in corresponding memory pool for 
    periodic timing structures
 */
#define MAX_TIME_PER 5


#define SORT 0

/* 
    Defines minimum(or maximum priority)
    which defines wheter the task is in 
    medium list or not
 */
#define MED_BND_RM 15

/* 
    Defines  maximum nesting level
    Value of 1 and 0  does not allow nesting.
 */
#define ALLOWED_NESTING_LEVEL 0
/*typedef enum
{
    MSP430=0,
    LM3S8962_ARM_CORTEX_M3
} platform;*/

//#define MSP430
//#define LM3S8962_ARM_CORTEX_M3
#define STM32F107_ARM_CORTEX_M3
#endif /* __BRRTK_APP_H__ */


