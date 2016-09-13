/* 
 * File:   common.h
 * Author: Administrator
 *
 * Created on 2016年7月27日, 下午2:25
 */

#ifndef COMMON_H
#define	COMMON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <plib.h>
#include "crc.h"
#include "ecan.h"
#include "taskcan.h"
#include "flash.h"
    
#define Fosc            64000000    //系统时钟频率64MHz     
#define APP_HEADER    	0x2000
#define APP_START		0x2040        

//=============================================================================================
// 定义CAN总线上各节点的地址
#define  BMS                             ((UINT8)0xED) 
#define  GUI                             ((UINT8)0xFD)
#define  DRIVE                           ((UINT8)0x09)
#define  CHARGER                         ((UINT8)0x0A)

// 以下宏定义定义了各个CAN节点之间的消息帧功能码
// 1. BMS与GUI之间通信功能码
#define  CAN_MSG_BATTERY_STATE           ((UINT8)0x01)
#define  CAN_MSG_CELL_STATE              ((UINT8)0x02)
#define  CAN_MSG_CELLS_VOLTAGE_1         ((UINT8)0x03)
#define  CAN_MSG_CELLS_VOLTAGE_2         ((UINT8)0x04)
#define  CAN_MSG_CELLS_VOLTAGE_3         ((UINT8)0x05)
#define  CAN_MSG_CELLS_VOLTAGE_4         ((UINT8)0x06)
#define  CAN_MSG_BATTERY_TEMPERATURE     ((UINT8)0x0E)
#define  CAN_MSG_BMS_VERSION             ((UINT8)0x0F)
#define  CAN_MSG_IMAGE_ERASE             ((UINT8)0xF0)
#define  CAN_MSG_IMAGE_REQUEST           ((UINT8)0xF1)
#define  CAN_MSG_IMAGE_LOAD              ((UINT8)0xF2) 
#define  CAN_MSG_OVER_LOAD               ((UINT8)0xF3) // 下载数据结束标志
#define  CAN_MSG_REQ_NEXT                ((UINT8)0xF4) // 请求下一帧数据
#define  CAN_MSG_REQ_AGAIN               ((UINT8)0xF5) // 请求重新发送该帧数据


//以下宏定义了bootloader与GUI下载之间的通信码


//硬件版本信息
#define HARDWARE_MAJOR_VERSION  0
#define HARDWARE_MINOR_VERSION	1
#define FIRMWARE_MAJOR_VERSION  0
#define FIRMWARE_MINOR_VERSION  1
#define FIRMWARE_REVISE_VERSION 1  


extern CanMsgTypedef 	 g_CanTxBuf;
extern CanMsgTypedef    g_CanRxBuf;
extern unsigned char   g_Data[80];

extern unsigned short long g_flashAddr;
extern unsigned char        g_FlashIndex;
extern unsigned long        g_totalBytes; 
unsigned char                isGetAppData;
#ifdef	__cplusplus
}
#endif

#endif	/* COMMON_H */

