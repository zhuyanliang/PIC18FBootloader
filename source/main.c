/* 
 * File:   main.c
 * Author: Administrator
 *
 * Created on 2016年7月27日, 上午11:39
 */

#include "common.h"

// CONFIG1L
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = LOW // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = HIGH   // SOSC Power Selection and mode Configuration bits (High Power SOSC circuit selected)
#pragma config XINST = OFF       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config FOSC = INTIO2    // Oscillator (Internal RC oscillator)
#pragma config PLLCFG = OFF     // PLL x4 Enable bit (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power Up Timer (Disabled)
#pragma config BOREN = SBORDIS  // Brown Out Detect (Enabled in hardware, SBOREN disabled)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (1.8V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN = SWDTDIS  // Watchdog Timer (WDT enabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 1048576  // Watchdog Postscaler (1:1048576)

// CONFIG3H
#pragma config CANMX = PORTB    // ECAN Mux bit (ECAN TX and RX pins are located on RB2 and RB3, respectively)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR Enabled, RE3 Disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size (2K word Boot Block size)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protect 00800-03FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 04000-07FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 08000-0BFFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 0C000-0FFFF (Disabled)

// CONFIG5H
#pragma config CPB = OFF        // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protect 00800-03FFF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 04000-07FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 08000-0BFFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 0C000-0FFFF (Disabled)

// CONFIG6H
#pragma config WRTC = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB = OFF       // Table Write Protect Boot (Disabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protect 00800-03FFF (Disabled)
#pragma config EBTR1 = OFF      // Table Read Protect 04000-07FFF (Disabled)
#pragma config EBTR2 = OFF      // Table Read Protect 08000-0BFFF (Disabled)
#pragma config EBTR3 = OFF      // Table Read Protect 0C000-0FFFF (Disabled)

// CONFIG7H
#pragma config EBTRB = OFF      // Table Read Protect Boot (Disabled)

#define LedRed                LATCbits.LATC7
#define LedGre                LATCbits.LATC6 

#asm
PSECT intcode
goto APP_START + 0x08
PSECT intcodelo
goto APP_START + 0x18
#endasm

        
CanMsgTypedef 	g_CanTxBuf;
CanMsgTypedef 	g_CanRxBuf;

void SystemInit(void);
UINT8 Image_Verify_crc16(void);
void Timer_Init(void);
void OSC_Init(void);
void ISR_Init(void);

unsigned short long g_flashAddr;
unsigned char g_Data[80] = {0x0};
unsigned char g_FlashIndex = 0;
unsigned char isGetAppData = 0;
unsigned long g_totalBytes = 0; // 记录一共多少字节

int crc = 0;

void Led_Init(void)
{
	TRISCbits.TRISC6 = 0;
	TRISCbits.TRISC7 = 0;

	/*添加SOC指示灯，板上指示灯*/
	TRISAbits.TRISA6 = 0;
	TRISCbits.TRISC1 = 0;

	//...
    LedRed = 1;
}

/*
 * 
 */
void main(void)
{   
	UINT16 totalNums = 0;
    UINT16 totalCrc = 0;
    UINT16 calcCRC = 0;
    UINT16 needReadTimes = 0;
    UINT8  readLastNum = 0;
    
    unsigned char goApp = 1;
    OSC_Init();  /* 初始化系统时钟 */
    SystemInit();  
    Led_Init();
    
    g_flashAddr = APP_HEADER;
    
    //取出App头64字节
    for(int i=0;i<64;i++)
    {
        g_Data[i] = flashRdOneBytes(g_flashAddr++);
    }

	totalNums =  (g_Data[1]<<8) + g_Data[0];
	totalCrc = (g_Data[3]<<8) + g_Data[2];
 #if 0   
	for(int i=4;i<64;i++)
    {	
        if(0xAA == g_Data[i])
        { 
            goApp = 1;
            continue;
        }
        else
        {
            goApp = 0;
            break;
        }
    }
#endif
	needReadTimes = totalNums/64;
	readLastNum = totalNums%64;
    g_flashAddr = APP_START;
	for(int i=0;i<needReadTimes;i++)
	{
		for(int j=0;j<64;j++)
	    {
	        g_Data[j] = flashRdOneBytes(g_flashAddr++);
	    }
	    calcCRC = ComputeCrc16(g_Data, calcCRC, 64);
	}
	for(int j=0;j<readLastNum;j++)
    {
        g_Data[j] = flashRdOneBytes(g_flashAddr++);
    }
    calcCRC = ComputeCrc16(g_Data, calcCRC, readLastNum);

    if(calcCRC == totalCrc)
    {
		goApp = 1;
    }
    else
    	goApp = 0;
	
    //红灯点亮，没有进入App
    if(goApp == 1)
    { 
       LedRed = 1;
       LedGre = 0;
	   asm("goto " ___mkstr(APP_START));
    }
    else
    {
        LedRed = 0;
        LedGre = 1;
        isGetAppData = 0;
    }
    g_flashAddr = APP_START;
    
	for (;;)
	{
        if(!isGetAppData)
            SendResponse(CAN_MSG_IMAGE_REQUEST);// 发送请求APP下载
		CAN_CompressData();		
	} 
    
}

void OSC_Init(void)
{   
	OSCCONbits.SCS 		= 0b00;     //Select INOSC as system clock source
    OSCCONbits.IRCF 	= 0b111;    //设置所选HF_INTOSC输出频率(16MHz)
    OSCCONbits.IDLEN 	= 0b0;      //when implement sleep function,mcu goto sleep 
    OSCTUNEbits.TUN 	= 0b0;		//快速RC振荡器(INTOSC)频率调节 暂时不调节，使用默认校验好的
    OSCTUNEbits.PLLEN 	= 0b1;      //enable PLL  系统时钟运行在16MHz*4 
}

void SystemInit()
{
	ISR_Init();  
	ECAN_Init();
    Led_Init();
    
#ifndef __DEBUG
	WDTCONbits.SWDTE = 1;
#endif
} 


void ISR_Init(void)
{
   INTCONbits.GIE   = 0;  // 关闭所有中断
}
