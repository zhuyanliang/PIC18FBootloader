//-------------------------------------------------------------------------------
// ecan.h head file,
//-------------------------------------------------------------------------------

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ECAN_H
#define __ECAN_H

#ifdef __cplusplus
   extern "C" {
#endif
       
// CAN消息帧类型定义
#define CAN_ID_STD                  ((UINT8)0x00)        // 标准帧      
#define CAN_ID_EXT                  ((UINT8)0x08)        // 扩展帧
#define CAN_RTR_DATA                ((UINT8)0x00)        // 数据帧        
#define CAN_RTR_REMOTE              ((UINT8)0x40)        // 远程帧

#define CAN_BAUDRATE                (250)         //ECAN bitrate define

//CAN message structure definition  
typedef struct
{
   UINT32 COB_ID;
   UINT8  IDE;
   UINT8  RTR; 
   UINT8  DLC;
   UINT8  Data[8];
} CanMsgTypedef;         
       
void ECAN_Init(void);
UINT8 ECAN_TransmitMsg(CanMsgTypedef* TxMessage);
UINT8 ECAN_ReceiveMsg(CanMsgTypedef* RxMessage);


#ifdef __cplusplus
}
#endif
#endif /*__ECAN_H */