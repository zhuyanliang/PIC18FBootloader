//-------------------------------------------------------------------------------
// ecan.h head file,
//-------------------------------------------------------------------------------

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ECAN_H
#define __ECAN_H

#ifdef __cplusplus
   extern "C" {
#endif
       
#include "include.h"
       
// CAN��Ϣ֡���Ͷ���
#define CAN_ID_STD                  ((unsigned char)0x00)        // ��׼֡      
#define CAN_ID_EXT                  ((unsigned char)0x08)        // ��չ֡
#define CAN_RTR_DATA                ((unsigned char)0x00)        // ����֡        
#define CAN_RTR_REMOTE              ((unsigned char)0x40)        // Զ��֡

#define CAN_BAUDRATE                (125)         //ECAN bitrate define

//CAN message structure definition  
typedef struct
{
   unsigned long COB_ID;
   unsigned char  IDE;
   unsigned char  RTR; 
   unsigned char  DLC;
   unsigned char  Data[8];
} CanMsgTypedef;         
       
void ECAN_Init(void);
unsigned char ECAN_TransmitMsg(CanMsgTypedef* TxMessage);
unsigned char ECAN_ReceiveMsg(CanMsgTypedef* RxMessage);

#ifdef __cplusplus
}
#endif
#endif /*__ECAN_H */