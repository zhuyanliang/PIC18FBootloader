/* 
 * File:   taskcan.h
 * Author: Administrator
 *
 * Created on 2016年7月19日, 上午10:44
 */

#ifndef TASKCAN_H
#define	TASKCAN_H

#ifdef	__cplusplus
extern "C" {
#endif

void SendResponse(UINT8 dat);

void CAN_ReceiveImage(void); 

void CAN_CompressData(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* TASKCAN_H */

