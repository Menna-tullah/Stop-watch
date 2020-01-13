 /******************************************************************************
 *
 * Module: common - macros
 *
 * File Name: common_macros.h
 *
 * Description: Commonly used Macros
 *
 * Author: Menna Mamdouh
 *
 *******************************************************************************/

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

/* set a certain bit in any register */
#define SET_BIT(REG,BIT_NUM) (REG|=(1<<BIT_NUM))

/* clear a certain bit in any register */
#define CLEAR_BIT(REG,BIT_NUM) (REG&=(~(1<<BIT_NUM)))

/* toggle a certain bit in any register */
#define TOGGLE_BIT(REG,BIT_NUM) (REG^=(1<<BIT_NUM))

/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG,BIT_NUM) (REG&(1<<BIT_NUM))

/* Check if a specific bit is clear in any register and return true if yes */
#define BIT_IS_CLEAR(REG,BIT_NUM) (!(REG&(1<<BIT_NUM)))

/* Rotate right the register value with specific number of rotates */
#define ROR(REG,num) (REG=(REG>>num)|(REG<<(8-num)))

/* Rotate left the register value with specific number of rotates */
#define ROL(REG,num) (REG=(REG<<num)|(REG>>(8-num)))

#endif /* COMMON_MACROS_H_ */
