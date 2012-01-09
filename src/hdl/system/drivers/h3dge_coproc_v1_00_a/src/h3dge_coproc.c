/*****************************************************************************
* Filename:          C:\git\h3dge\src\hdl\system/drivers/h3dge_coproc_v1_00_a/src/h3dge_coproc.c
* Version:           1.00.a
* Description:       h3dge_coproc Driver Source File
* Date:              Wed Nov 30 20:33:52 2011 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "h3dge_coproc.h"

/************************** Function Definitions ***************************/


/**
 *
 * User logic master module to send/receive bytes to/from remote system memory.
 * While sending, the bytes are read from user logic local FIFO and write to remote system memory.
 * While receiving, the bytes are read from remote system memory and write to user logic local FIFO.
 *
 * @param   BaseAddress is the base address of the H3DGE_COPROC device.
 * @param   DstAddress is the destination system memory address from/to which the data will be fetched/stored.
 * @param   Size is the number of bytes to be sent.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void H3DGE_COPROC_MasterSendByte(Xuint32 BaseAddress, Xuint32 DstAddress, int Size)
{
  /*
   * Set user logic master control register for write transfer.
   */
  Xil_Out8(BaseAddress+H3DGE_COPROC_MST_CNTL_REG_OFFSET, MST_BRWR);

  /*
   * Set user logic master address register to drive IP2Bus_Mst_Addr signal.
   */
  Xil_Out32(BaseAddress+H3DGE_COPROC_MST_ADDR_REG_OFFSET, DstAddress);

  /*
   * Set user logic master byte enable register to drive IP2Bus_Mst_BE signal.
   */
  Xil_Out16(BaseAddress+H3DGE_COPROC_MST_BE_REG_OFFSET, 0xFFFF);

  /*
   * Set user logic master length register.
   */
  Xil_Out16(BaseAddress+H3DGE_COPROC_MST_LEN_REG_OFFSET, (Xuint16) Size);
  /*
   * Start user logic master write transfer by writting special pattern to its go port.
   */
  Xil_Out8(BaseAddress+H3DGE_COPROC_MST_GO_PORT_OFFSET, MST_START);
}

void H3DGE_COPROC_MasterRecvByte(Xuint32 BaseAddress, Xuint32 DstAddress, int Size)
{
  /*
   * Set user logic master control register for read transfer.
   */
  Xil_Out8(BaseAddress+H3DGE_COPROC_MST_CNTL_REG_OFFSET, MST_BRRD);

  /*
   * Set user logic master address register to drive IP2Bus_Mst_Addr signal.
   */
  Xil_Out32(BaseAddress+H3DGE_COPROC_MST_ADDR_REG_OFFSET, DstAddress);

  /*
   * Set user logic master byte enable register to drive IP2Bus_Mst_BE signal.
   */
  Xil_Out16(BaseAddress+H3DGE_COPROC_MST_BE_REG_OFFSET, 0xFFFF);

  /*
   * Set user logic master length register.
   */
  Xil_Out16(BaseAddress+H3DGE_COPROC_MST_LEN_REG_OFFSET, (Xuint16) Size);
  /*
   * Start user logic master read transfer by writting special pattern to its go port.
   */
  Xil_Out8(BaseAddress+H3DGE_COPROC_MST_GO_PORT_OFFSET, MST_START);
}

/**
 *
 * Enable all possible interrupts from H3DGE_COPROC device.
 *
 * @param   baseaddr_p is the base address of the H3DGE_COPROC device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void H3DGE_COPROC_EnableInterrupt(void * baseaddr_p)
{
  Xuint32 baseaddr;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Enable all interrupt source from user logic.
   */
  H3DGE_COPROC_mWriteReg(baseaddr, H3DGE_COPROC_INTR_IPIER_OFFSET, 0x00000001);

  /*
   * Set global interrupt enable.
   */
  H3DGE_COPROC_mWriteReg(baseaddr, H3DGE_COPROC_INTR_DGIER_OFFSET, INTR_GIE_MASK);
}

/**
 *
 * Example interrupt controller handler for H3DGE_COPROC device.
 * This is to show example of how to toggle write back ISR to clear interrupts.
 *
 * @param   baseaddr_p is the base address of the H3DGE_COPROC device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void H3DGE_COPROC_Intr_DefaultHandler(void * baseaddr_p)
{
  Xuint32 baseaddr;
  Xuint32 IntrStatus;
Xuint32 IpStatus;
  baseaddr = (Xuint32) baseaddr_p;

  {
    xil_printf("User logic interrupt! \n\r");
    IpStatus = H3DGE_COPROC_mReadReg(baseaddr, H3DGE_COPROC_INTR_IPISR_OFFSET);
    H3DGE_COPROC_mWriteReg(baseaddr, H3DGE_COPROC_INTR_IPISR_OFFSET, IpStatus);
  }

}

