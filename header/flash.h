/* 
 * File:   flash.h
 * Author: Administrator
 *
 * Created on 2016��8��4��, ����2:39
 */

#ifndef FLASH_H
#define	FLASH_H

#ifdef	__cplusplus
extern "C" {
#endif

unsigned char flashRdOneBytes(unsigned short long FlashAddr);

void flashWr64Bytes(unsigned short long FlashAddr, unsigned char *BufPtr);

void flashErase64Bytes(unsigned short long FlashAddr);


#ifdef	__cplusplus
}
#endif

#endif	/* FLASH_H */

