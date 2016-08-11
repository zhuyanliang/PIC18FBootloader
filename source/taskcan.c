#include "common.h"

//----------------------------------------------------------------------------
// Function    ：CAN_ExtractSourceAddress
// Description ：从CAN ID中提取发送节点的源地址.
// Parameters  ：
// Returns     ：
//----------------------------------------------------------------------------
UINT8 CAN_ProtAnal_SA(UINT32 can_id)
{
   return((UINT8)can_id);
}

//----------------------------------------------------------------------------
// Function    ：CAN_ExtractFunctionCode
// Description ：从CAN ID中提取功能码.
// Parameters  ：
// Returns     ：
//----------------------------------------------------------------------------
UINT8 CAN_ProtAnal_FC(UINT32 can_id)
{
	UINT8 temp;

	temp = (UINT8)(can_id >> 16);
	return(temp);
}


//----------------------------------------------------------------------------
// Function    ：CAN_Generate_ID
// Description ：
// Parameters  ：
// Returns     ：
//----------------------------------------------------------------------------
UINT32 CAN_Generate_ID(UINT8 msg_fc, UINT8 crc)
{
	UINT32 temp = 0;

	temp = ((UINT32)msg_fc << 16) + ((UINT32)crc << 8) + BMS;
	return(temp);
}

void SendResponse(UINT8 dat)
{
    g_CanTxBuf.COB_ID = 0;
    g_CanTxBuf.DLC = 0;
    g_CanTxBuf.RTR = 1; //远程帧
    g_CanTxBuf.IDE = 1; //扩展帧
    
    g_CanTxBuf.COB_ID += CAN_Generate_ID(dat,'C');
    
    ECAN_TransmitMsg(&g_CanTxBuf);
}

//----------------------------------------------------------------------------
// Function    : CAN_Request_Image
// Description : 发送远程帧向上位机请求用户app
// Parameters  : none
// Returns     : none
//----------------------------------------------------------------------------
void CAN_Request_Image(void)
{
	g_CanTxBuf.COB_ID = CAN_Generate_ID(GUI, CAN_MSG_IMAGE_REQUEST);
	g_CanTxBuf.IDE = CAN_ID_EXT;
	g_CanTxBuf.RTR = CAN_RTR_DATA;
	g_CanTxBuf.DLC = 0x08;

	g_CanTxBuf.Data[0] = 0x01;
	g_CanTxBuf.Data[1] = 0x23;
	g_CanTxBuf.Data[2] = 0x45;
	g_CanTxBuf.Data[3] = 0x67;
	g_CanTxBuf.Data[4] = 0x89;
	g_CanTxBuf.Data[5] = 0xAB;
	g_CanTxBuf.Data[6] = 0xCD;
	g_CanTxBuf.Data[7] = 0xEF;

	ECAN_TransmitMsg(&g_CanTxBuf);
}

void CAN_ReceiveImage(void)
{
    static UINT8 i = 0;
    static UINT8 len = 0;
    static UINT16 crc = 0;
    static UINT8 getNextFrame = 0;
    static UINT8 getCrc = 0;
    
    getCrc = (unsigned char)((g_CanRxBuf.COB_ID >>8)&0xFF);
    len = g_CanRxBuf.DLC;
    if(len>0)
        crc = calculate_crc8((UINT8*)&g_CanRxBuf.Data[0],len);
    
    if(getCrc == crc)
    {
        g_totalBytes += len;
        for(i=0;i<g_CanRxBuf.DLC;i++)
        {
            g_Data[g_FlashIndex++] = g_CanRxBuf.Data[i];
        }
        getNextFrame = 1;
    }
    else
    {
        getNextFrame = 0;
    }
    
    if(g_FlashIndex >= 64)
    {
        flashWr64Bytes(g_flashAddr,g_Data);
        int lastLen = (g_FlashIndex - 64);
        g_FlashIndex = 0;
        for(int i=0;i<lastLen;i++)
        {
            g_Data[g_FlashIndex++] = g_Data[64+i];
        }
        
        g_flashAddr += 64;
    }
    
    if(1 == getNextFrame)
    {
        SendResponse(CAN_MSG_REQ_NEXT);
    }
    else
    {
        SendResponse(CAN_MSG_REQ_AGAIN);
    }
}

void CAN_ReceiveOver(void)
{
    while(g_FlashIndex < 64)
    {
        g_Data[g_FlashIndex++] = 0xff;
    }
    flashWr64Bytes(g_flashAddr,g_Data);
    
    g_FlashIndex = 0;
    
    for(int i =0;i<64;i++)
    {
        g_Data[i] = 0xAA; // 前面11字节保存系统信息
    }
    flashWr64Bytes(APP_HEADER,g_Data);
    
    asm("goto " ___mkstr(APP_START));
}

//----------------------------------------------------------------------------
// Function    ：CAN_RxTask_Update
// Description ：处理接收到的CAN数据帧
// Parameters  ：none
// Returns     ：none
//----------------------------------------------------------------------------
void CAN_CompressData(void)
{
    unsigned char can_fc;
    while (ECAN_ReceiveMsg(&g_CanRxBuf))
    { 
        
        can_fc = CAN_ProtAnal_FC(g_CanRxBuf.COB_ID);
        
        switch(can_fc)
        {
            case CAN_MSG_IMAGE_ERASE:
            {
                flashErase64Bytes(APP_HEADER);
                for(int i=0;i<64;i++)
                {
                    if(0xFF == flashRdOneBytes(APP_HEADER+i))
                        continue;
                    else 
                    {
                        SendResponse(CAN_MSG_REQ_AGAIN);  
                        break;
                    }
                }
                SendResponse(CAN_MSG_REQ_NEXT);
                isGetAppData = 1;
            }
            break;
            case CAN_MSG_IMAGE_LOAD:
            CAN_ReceiveImage();
            break;
            case CAN_MSG_OVER_LOAD:
            CAN_ReceiveOver();
            break;
            default:
            break;
        }

       
    }
    
    
}


