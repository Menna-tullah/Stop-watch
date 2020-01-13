 /******************************************************************************
 *
 * Module: Micro - Configuration
 *
 * File Name: micro_config.h
 *
 * Description: File include all Microcontroller libraries
 *
 * Author: Menna Mamdouh
 *
 *******************************************************************************/

#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_


#ifndef F_CPU
#define F_CPU 1000000 /* 1MHZ clock frequency */
#endif

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>


#endif /* MICRO_CONFIG_H_ */
