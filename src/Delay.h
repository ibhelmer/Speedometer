/* 
 * File:   Delay.h
 * Author: IHN
 *
 * Created on 11. april 2018, 10:23
 */

#ifndef DELAY_H
#define	DELAY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdint.h>
#define MY_SYS_CLK_FREQ 80000000

#define READ_CORE_TIMER()    _CP0_GET_COUNT()       // Read the MIPS Core Timer

void DelayMs(uint16_t  milliseconds);
void DelayUs(uint16_t microseconds);
void DelayNs(uint16_t nanoseconds);

#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_H */

