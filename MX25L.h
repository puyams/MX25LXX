/*
* The MIT License (MIT)
*
* Copyright (c) 20014 Moukhlynin Ruslan <ruslan@khvmntk.ru>
*
* MX25L.h
* This file is part of the MX25L SPI memory library.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
 
#ifndef MX25L_H_
#define MX25L_H_

//***************************************************************
#define _MX25LXX_SPI                   hspi5
#define _MX25LXX_CS_GPIO               FLASH_CS_GPIO_Port
#define _MX25LXX_CS_PIN                FLASH_CS_Pin
//#define _MX25LXX_USE_FREERTOS          0
//#define _MX25LXX_DEBUG                 0
//***************************************************************
#include "stdint.h"
#include "stdbool.h"

#ifndef true
	#define true  1
#endif
#ifndef false
	#define false 0
#endif

void    MX25L_SPI_Init(void); // no need for stm32
uint8_t MX25L_SPI_Transfer(uint8_t data);
void    MX25L_Get_Identification(uint8_t data[3]);
uint8_t MX25L_GetStatus(void);
void    MX25L_SetWriteEnable(bool enable);
void    MX25L_SetStatus(uint8_t status);
void    MX25L_SectorErase(uint32_t address);
void    MX25L_BlockErase(uint32_t address);
void    MX25L_ChipErase(void);
void    MX25L_SetAddress(uint32_t address);
void    MX25L_Write(uint32_t address, const uint8_t * data, uint16_t length);
void    MX25L_Read(uint32_t address, uint8_t * data, uint16_t length);
void    MX25L_FastRead(uint32_t address, uint8_t * data, uint16_t length);
void    MX25L_ProtectBlock(bool bp0, bool bp1, bool bp2, bool bp3);

#endif /* MX25L_H_ */
