/* 
 * File:   taskcan.h
 * Author: Administrator
 *
 * Created on 2016��7��19��, ����10:44
 */

#ifndef TASKCAN_H
#define	TASKCAN_H

#ifdef	__cplusplus
extern "C" {
#endif

void SendResponse(unsigned char dat);

void CAN_ReceiveImage(void); 

void CAN_CompressData(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* TASKCAN_H */

