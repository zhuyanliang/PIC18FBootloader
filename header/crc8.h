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
    
UINT8 calculate_crc8(UINT8* ptr, UINT8 len);
UINT8 calculate_char_crc8(UINT8 crc, UINT8 nbyte);


#ifdef	__cplusplus
}
#endif

#endif	/* CRC16_H */

