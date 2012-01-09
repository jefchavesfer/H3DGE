/*****************************************************************************
* Filename:          C:\git\h3dge\src\hdl\system/drivers/h3dge_coproc_v1_00_a/src/h3dge_coproc_selftest.c
* Version:           1.00.a
* Description:       Contains a diagnostic self-test function for the h3dge_coproc driver
* Date:              Wed Nov 30 20:33:52 2011 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "h3dge_coproc.h"

/************************** Constant Definitions ***************************/

#define H3DGE_COPROC_SELFTEST_BUFSIZE  512 /* Size of buffer (for transfer test) in bytes */

/************************** Variable Definitions ****************************/

static Xuint8 __attribute__((aligned (128))) SrcBuffer[H3DGE_COPROC_SELFTEST_BUFSIZE];   /* Source buffer      */
static Xuint8 __attribute__((aligned (128))) DstBuffer[H3DGE_COPROC_SELFTEST_BUFSIZE];   /* Destination buffer */

/************************** Function Definitions ***************************/

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the H3DGE_COPROC instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus H3DGE_COPROC_SelfTest(void * baseaddr_p)
{
  int     Index;
  Xuint32 baseaddr;
  Xuint8  Reg8Value;
  Xuint16 Reg16Value;
  Xuint32 Reg32Value;
  
  /*
   * Check and get the device address
   */
  /*
   * Base Address maybe 0. Up to developer to uncomment line below.
  XASSERT_NONVOID(baseaddr_p != XNULL);
   */
  baseaddr = (Xuint32) baseaddr_p;

  xil_printf("******************************\n\r");
  xil_printf("* User Peripheral Self Test\n\r");
  xil_printf("******************************\n\n\r");

  /*
   * Reset the device to get it back to the default state
   */
  xil_printf("Soft reset test...\n\r");
  H3DGE_COPROC_mReset(baseaddr);
  xil_printf("   - write 0x%08x to software reset register\n\r", SOFT_RESET);
  xil_printf("   - soft reset passed\n\n\r");

  /*
   * Write to user logic slave module register(s) and read back
   */
  xil_printf("User logic slave module test...\n\r");
  xil_printf("   - write 1 to slave register 0 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg0(baseaddr, 0, 1);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg0(baseaddr, 0);
  xil_printf("   - read %d from register 0 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 1 )
  {
    xil_printf("   - slave register 0 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 2 to slave register 1 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg1(baseaddr, 0, 2);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg1(baseaddr, 0);
  xil_printf("   - read %d from register 1 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 2 )
  {
    xil_printf("   - slave register 1 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 3 to slave register 2 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg2(baseaddr, 0, 3);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg2(baseaddr, 0);
  xil_printf("   - read %d from register 2 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 3 )
  {
    xil_printf("   - slave register 2 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 4 to slave register 3 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg3(baseaddr, 0, 4);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg3(baseaddr, 0);
  xil_printf("   - read %d from register 3 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 4 )
  {
    xil_printf("   - slave register 3 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 5 to slave register 4 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg4(baseaddr, 0, 5);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg4(baseaddr, 0);
  xil_printf("   - read %d from register 4 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 5 )
  {
    xil_printf("   - slave register 4 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 6 to slave register 5 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg5(baseaddr, 0, 6);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg5(baseaddr, 0);
  xil_printf("   - read %d from register 5 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 6 )
  {
    xil_printf("   - slave register 5 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 7 to slave register 6 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg6(baseaddr, 0, 7);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg6(baseaddr, 0);
  xil_printf("   - read %d from register 6 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 7 )
  {
    xil_printf("   - slave register 6 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 8 to slave register 7 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg7(baseaddr, 0, 8);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg7(baseaddr, 0);
  xil_printf("   - read %d from register 7 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 8 )
  {
    xil_printf("   - slave register 7 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 9 to slave register 8 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg8(baseaddr, 0, 9);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg8(baseaddr, 0);
  xil_printf("   - read %d from register 8 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 9 )
  {
    xil_printf("   - slave register 8 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 10 to slave register 9 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg9(baseaddr, 0, 10);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg9(baseaddr, 0);
  xil_printf("   - read %d from register 9 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 10 )
  {
    xil_printf("   - slave register 9 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 11 to slave register 10 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg10(baseaddr, 0, 11);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg10(baseaddr, 0);
  xil_printf("   - read %d from register 10 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 11 )
  {
    xil_printf("   - slave register 10 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 12 to slave register 11 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg11(baseaddr, 0, 12);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg11(baseaddr, 0);
  xil_printf("   - read %d from register 11 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 12 )
  {
    xil_printf("   - slave register 11 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 13 to slave register 12 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg12(baseaddr, 0, 13);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg12(baseaddr, 0);
  xil_printf("   - read %d from register 12 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 13 )
  {
    xil_printf("   - slave register 12 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 14 to slave register 13 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg13(baseaddr, 0, 14);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg13(baseaddr, 0);
  xil_printf("   - read %d from register 13 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 14 )
  {
    xil_printf("   - slave register 13 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 15 to slave register 14 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg14(baseaddr, 0, 15);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg14(baseaddr, 0);
  xil_printf("   - read %d from register 14 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 15 )
  {
    xil_printf("   - slave register 14 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 16 to slave register 15 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg15(baseaddr, 0, 16);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg15(baseaddr, 0);
  xil_printf("   - read %d from register 15 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 16 )
  {
    xil_printf("   - slave register 15 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 17 to slave register 16 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg16(baseaddr, 0, 17);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg16(baseaddr, 0);
  xil_printf("   - read %d from register 16 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 17 )
  {
    xil_printf("   - slave register 16 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 18 to slave register 17 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg17(baseaddr, 0, 18);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg17(baseaddr, 0);
  xil_printf("   - read %d from register 17 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 18 )
  {
    xil_printf("   - slave register 17 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 19 to slave register 18 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg18(baseaddr, 0, 19);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg18(baseaddr, 0);
  xil_printf("   - read %d from register 18 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 19 )
  {
    xil_printf("   - slave register 18 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 20 to slave register 19 word 0\n\r");
  H3DGE_COPROC_mWriteSlaveReg19(baseaddr, 0, 20);
  Reg32Value = H3DGE_COPROC_mReadSlaveReg19(baseaddr, 0);
  xil_printf("   - read %d from register 19 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 20 )
  {
    xil_printf("   - slave register 19 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - slave register write/read passed\n\n\r");

  /*
   * Setup user logic master module to receive/send data from/to remote memory
   */
  /* xil_printf("User logic master module test...\n\r"); */
  /* xil_printf("   - source buffer address is 0x%08x\n\r", SrcBuffer); */
  /* xil_printf("   - destination buffer address is 0x%08x\n\r", DstBuffer); */
  /* xil_printf("   - initialize the source buffer bytes with a pattern\n\r"); */
  /* xil_printf("   - initialize the destination buffer bytes to zero\n\r"); */
  /* for ( Index = 0; Index < H3DGE_COPROC_SELFTEST_BUFSIZE; Index++ ) */
  /* { */
  /*   SrcBuffer[Index] = Index; */
  /*   DstBuffer[Index] = 0; */
  /* } */
  /* xil_printf("   - start user logic master module to receive bytes from the source\n\r"); */
  /* H3DGE_COPROC_MasterRecvByte(baseaddr, (Xuint32) SrcBuffer, H3DGE_COPROC_SELFTEST_BUFSIZE); */
  /* while ( ! H3DGE_COPROC_mMasterDone(baseaddr) ) {} */
  /* xil_printf("   - transfer completed\n\r"); */
  /* xil_printf("   - start user logic master module to send bytes to the destination\n\r"); */
  /* H3DGE_COPROC_MasterSendByte(baseaddr, (Xuint32) DstBuffer, H3DGE_COPROC_SELFTEST_BUFSIZE); */
  /* while ( ! H3DGE_COPROC_mMasterDone(baseaddr) ) {} */
  /* xil_printf("   - transfer completed\n\r"); */
  /* for ( Index = 0; Index < H3DGE_COPROC_SELFTEST_BUFSIZE; Index++ ) */
  /* { */
  /*   if ( DstBuffer[Index] != SrcBuffer[Index] ) */
  /*   { */
  /*     xil_printf("   - destination buffer byte %d is different from the source buffer\n\r", Index); */
  /*     xil_printf("   - master module data transfer failed\n\r"); */
  /*     return XST_FAILURE; */
  /*   } */
  /* } */
  /* xil_printf("   - destination buffer's contents are same as the source buffer\n\r"); */
  /* xil_printf("   - master module data transfer passed\n\n\r"); */

  /*
   * Enable all possible interrupts and clear interrupt status register(s)
   */
  xil_printf("Interrupt controller test...\n\r");
  Reg32Value = H3DGE_COPROC_mReadReg(baseaddr, H3DGE_COPROC_INTR_IPISR_OFFSET);
  xil_printf("   - IP (user logic) interrupt status : 0x%08x\n\r", Reg32Value);
  xil_printf("   - clear IP (user logic) interrupt status register\n\r");
  H3DGE_COPROC_mWriteReg(baseaddr, H3DGE_COPROC_INTR_IPISR_OFFSET, Reg32Value);
  xil_printf("   - enable all possible interrupt(s)\n\r");
  H3DGE_COPROC_EnableInterrupt(baseaddr_p);
  xil_printf("   - write/read interrupt register passed\n\n\r");

  return XST_SUCCESS;
}
