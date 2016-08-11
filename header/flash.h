/* 
 * File:   flash.h
 * Author: Administrator
 *
 * Created on 2016年8月4日, 下午2:39
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

