/* 
 * File:   crc16.h
 * Author: Administrator
 *
 * Created on 2016年7月27日, 下午2:38
 */

#ifndef CRC16_H
#define	CRC16_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "include.h"   
    
    
unsigned char calculate_crc8(unsigned char* ptr, unsigned char len);
unsigned char calculate_char_crc8(unsigned char crc, unsigned char nbyte);
unsigned short ComputeCrc16(unsigned char *buffer, unsigned short lastcrc,unsigned short long len);


#ifdef	__cplusplus
}
#endif

#endif	/* CRC16_H */

