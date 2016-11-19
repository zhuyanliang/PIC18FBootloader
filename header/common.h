/* 
 * File:   common.h
 * Author: Administrator
 *
 * Created on 2016��7��27��, ����2:25
 */

#ifndef COMMON_H
#define	COMMON_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define Fosc            64000000    //ϵͳʱ��Ƶ��64MHz     
#define APP_HEADER    	0x2000
#define APP_START		0x2040   


//=============================================================================================
// ����CAN�����ϸ��ڵ�ĵ�ַ
#define  BMS                             ((unsigned char)0xED) 
#define  GUI                             ((unsigned char)0xFD)
#define  DRIVE                           ((unsigned char)0x09)
#define  CHARGER                         ((unsigned char)0x0A)

// ���º궨�嶨���˸���CAN�ڵ�֮�����Ϣ֡������
// 1. BMS��GUI֮��ͨ�Ź�����
#define  CAN_MSG_BATTERY_STATE           ((unsigned char)0x01)
#define  CAN_MSG_CELL_STATE              ((unsigned char)0x02)
#define  CAN_MSG_CELLS_VOLTAGE_1         ((unsigned char)0x03)
#define  CAN_MSG_CELLS_VOLTAGE_2         ((unsigned char)0x04)
#define  CAN_MSG_CELLS_VOLTAGE_3         ((unsigned char)0x05)
#define  CAN_MSG_CELLS_VOLTAGE_4         ((unsigned char)0x06)
#define  CAN_MSG_BATTERY_TEMPERATURE     ((unsigned char)0x0E)
#define  CAN_MSG_BMS_VERSION             ((unsigned char)0x0F)
#define  CAN_MSG_IMAGE_ERASE             ((unsigned char)0xF0)
#define  CAN_MSG_IMAGE_REQUEST           ((unsigned char)0xF1)
#define  CAN_MSG_IMAGE_LOAD              ((unsigned char)0xF2) 
#define  CAN_MSG_OVER_LOAD               ((unsigned char)0xF3) // �������ݽ�����־
#define  CAN_MSG_REQ_NEXT                ((unsigned char)0xF4) // ������һ֡����
#define  CAN_MSG_REQ_AGAIN               ((unsigned char)0xF5) // �������·��͸�֡����


//���º궨����bootloader��GUI����֮���ͨ����


//Ӳ���汾��Ϣ
#define HARDWARE_MAJOR_VERSION  0
#define HARDWARE_MINOR_VERSION	1
#define FIRMWARE_MAJOR_VERSION  0
#define FIRMWARE_MINOR_VERSION  1
#define FIRMWARE_REVISE_VERSION 1  


extern CanMsgTypedef    g_CanTxBuf;
extern CanMsgTypedef    g_CanRxBuf;
extern unsigned char    g_Data[80];

extern unsigned short long  g_flashAddr;
extern unsigned char        g_FlashIndex;
extern unsigned long        g_totalBytes; 
unsigned char               isGetAppData;


#ifdef	__cplusplus
}
#endif

#endif	/* COMMON_H */

