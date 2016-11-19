#include "include.h"

//----------------------------------------------------------------------------
// Function    ：CAN_ExtractFunctionCode
// Description ：从CAN ID中提取功能码.
// Parameters  ：
// Returns     ：
//----------------------------------------------------------------------------
unsigned char CAN_ProtAnal_FC(unsigned long can_id)
{
	unsigned char temp;

	temp = (unsigned char)(can_id >> 16);
	return(temp);
}


//----------------------------------------------------------------------------
// Function    ：CAN_Generate_ID
// Description ：
// Parameters  ：
// Returns     ：
//----------------------------------------------------------------------------
unsigned long CAN_Generate_ID(unsigned char msg_fc, unsigned char crc)
{
	unsigned long temp = 0;

	temp = ((unsigned long)msg_fc << 16) + ((unsigned long)crc << 8) + BMS;
	return(temp);
}

void SendResponse(unsigned char dat)
{
    g_CanTxBuf.COB_ID = 0;
    g_CanTxBuf.DLC = 0;
    g_CanTxBuf.RTR = 1; //远程帧
    g_CanTxBuf.IDE = 1; //扩展帧
    
    g_CanTxBuf.COB_ID += CAN_Generate_ID(dat,'C');
    
    ECAN_TransmitMsg(&g_CanTxBuf);
}

void CAN_ReceiveImage(void)
{
    static unsigned char i = 0;
    static unsigned char len = 0;
    static unsigned short crc = 0;
    static unsigned char getNextFrame = 0;
    static unsigned char getCrc = 0;
    
    getCrc = (unsigned char)((g_CanRxBuf.COB_ID >>8)&0xFF);
    len = g_CanRxBuf.DLC;
    if(len>0)
        crc = calculate_crc8((unsigned char*)&g_CanRxBuf.Data[0],len);
    
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
        for(i=0;i<lastLen;i++)
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
	unsigned short crc = 0;
    unsigned char getCrc = 0;
    unsigned char len = 0;
    
    while(g_FlashIndex < 64)
    {
        g_Data[g_FlashIndex++] = 0xff;
    }
    flashWr64Bytes(g_flashAddr,g_Data);
    
    g_FlashIndex = 0;

	getCrc = (unsigned char)((g_CanRxBuf.COB_ID >>8)&0xFF);
    len = g_CanRxBuf.DLC;
    if(len>0)
        crc = calculate_crc8((unsigned char*)&g_CanRxBuf.Data[0],len);
    
    if(getCrc == crc)
    {
    	g_Data[0] = g_CanRxBuf.Data[0];
	    g_Data[1] = g_CanRxBuf.Data[1];
	    g_Data[2] = g_CanRxBuf.Data[2];
	    g_Data[3] = g_CanRxBuf.Data[3];
    }

    for(int i =4;i<64;i++)
    {
        g_Data[i] = 0xAA; 
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


