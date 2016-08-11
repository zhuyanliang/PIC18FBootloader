
#include "common.h"
//============================================================================
// Function    : FlashErase64Bytes
// Description : This function erases the 64 bytes of flash program memory 
//               specified by FlashAddr (ignoring the 6 LSBs )
// Parameters  : unsigned short long - Address of block to erase
// Returns     : none
//============================================================================
void flashErase64Bytes(unsigned short long FlashAddr)
{
   TBLPTR = FlashAddr;                       // Setup flash program address
   EECON1bits.EEPGD = 1;                        
   EECON1bits.CFGS  = 0;                     // Point to program memory 
   EECON1bits.WREN  = 1;                     // Set write enable bit 
   EECON1bits.FREE  = 1;                     // Flash row erase enable bit     
   INTCONbits.GIE   = 0;                     // Disable interrupts           
   EECON2           = 0x55;                  // Interlock pattern 1          
   EECON2           = 0xAA;                  // Interlock pattern 2          
   EECON1bits.WR    = 1;                     // Start erasing           
   while (EECON1bits.WR);                 // Wait for erase to terminate                                  
   EECON1bits.WREN = 0;                      // Disable flash program writes    
   INTCONbits.GIE   = 1;                     // Reenable interrupts 
}

unsigned char flashRdOneBytes(unsigned short long FlashAddr)
{
    unsigned char ret;
    TBLPTR = FlashAddr;
    asm("tblrd");
    ret = TABLAT;
    
    return ret;
}

//============================================================================
// Function    : FlashWr64Bytes
// Description : This function writes 64 bytes to the flash program memory 
//               address specified by FlashAddr (ignoring the 6 LSBs)
// Parameters  : unsigned short long - Address of block write
//               unsigned char *BufPtr - Pointer to data written
// Returns     : none
//============================================================================
void flashWr64Bytes(unsigned short long FlashAddr, unsigned char *BufPtr)
{
    unsigned char i;
   unsigned char  NumByte;                    // Number of byte to write
   unsigned short long TblAddr;                   // Two block start address
   INTCONbits.GIE   = 0b0;                     // Disable interrupts   
   flashErase64Bytes(FlashAddr);       // Erase 64 bytes by rom addr
   
   TblAddr = FlashAddr  & 0x3FFFC0;    // 始终从保持寄存器0开始
   
   NumByte = 64;                       // Number of bytes written
   TBLPTR = TblAddr;                   // 32 bytes start rom address
   
   while (NumByte--)                   // If still have byte
   {
        TABLAT = *BufPtr++;              // assign the 32 data bytes to
        asm("tblwt*+");       
   }

   TBLPTR = TblAddr;                   // Setup start written addr
   EECON1bits.EEPGD = 0b1;                     // Point to program memory      
   EECON1bits.CFGS  = 0b0;                     // Set write enable bit         
   EECON1bits.WREN  = 0b1;                     // Flash row erase enable bit   
        
   EECON2           = 0x55;                  // Interlock pattern 1          
   EECON2           = 0x0AA;                  // Interlock pattern 2 
   
   EECON1bits.WR    = 0b1;                     // Start writing  
   Nop();Nop();Nop();Nop();Nop();Nop();
   while (EECON1bits.WR);                 // Wait for write to terminate  
   EECON1bits.WREN = 0b0;                      // Disable flash program writes
   INTCONbits.GIE   = 0b1;  
}  