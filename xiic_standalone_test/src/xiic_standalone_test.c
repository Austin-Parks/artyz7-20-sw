/***************************** Include Files *********************************/
#include <stdio.h>
#include <unistd.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xstatus.h"
#include "xiicps.h"

/************************** Constant Definitions *****************************/
/**
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define IIC_DEVICE_ID	XPAR_XIICPS_0_DEVICE_ID
XIicPs Iic;

#define IIC_SCLK_RATE 100000
#define SLV_MON_LOOP_COUNT 0x00FFFFF

#define IIC_ADDR_TMP006 0x20

/*****************************************************************************/
/**
* This function perform the initial configuration for the IICPS Device.
*
* @param	DeviceId instance.
*
* @return	XST_SUCCESS if pass, otherwise XST_FAILURE.
*
* @note		None.
*
****************************************************************************/
static s32 IicPsConfig(u16 DeviceId)
{
	s32 Status;
	XIicPs_Config *ConfigPtr;	/* Pointer to configuration data */

	/*
	 * Initialize the IIC driver so that it is ready to use.
	 */
	ConfigPtr = XIicPs_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(&Iic, ConfigPtr,
					ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	/*
	 * Set the IIC serial clock rate.
	 */
	XIicPs_SetSClk(&Iic, IIC_SCLK_RATE);
	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function checks the availability of a slave using slave monitor mode.
*
* @param	DeviceId is the Device ID of the IicPs Device and is the
*		XPAR_<IICPS_instance>_DEVICE_ID value from xparameters.h
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note 	None.
*
*******************************************************************************/
static s32 IicPsSlaveMonitor(u16 Address, u16 DeviceId)
{
	u32 Index,IntrStatusReg;
	s32 Status;
	XIicPs *IicPtr;

	/*
	 * Initialize the IIC driver so that it is ready to use.
	 */
	Status = IicPsConfig(DeviceId);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	IicPtr = &Iic;
	XIicPs_EnableSlaveMonitor(&Iic, Address);

	Index = 0;
	/*
	 * Wait for the Slave Monitor Status
	 */
	while (Index < SLV_MON_LOOP_COUNT) {
		Index++;
		/*
		 * Read the Interrupt status register.
		 */
		IntrStatusReg = XIicPs_ReadReg(IicPtr->Config.BaseAddress,
						 (u32)XIICPS_ISR_OFFSET);
		if (0U != (IntrStatusReg & XIICPS_IXR_SLV_RDY_MASK)) {
			XIicPs_DisableSlaveMonitor(&Iic);
			XIicPs_WriteReg(IicPtr->Config.BaseAddress,
					(u32)XIICPS_ISR_OFFSET, IntrStatusReg);
			return XST_SUCCESS;
		}
	}
	XIicPs_DisableSlaveMonitor(&Iic);
	return XST_FAILURE;
}


/*****************************************************************************/
/**
*
* This function is use to figure out the slave device is alive or not.
*
* @param        slave address and Device ID .
*
* @return       XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note         None.
*
*******************************************************************************/
static s32 IicPsFindDevice(u16 addr, u16 DeviceId)
{
	s32 Status;

	Status = IicPsSlaveMonitor(addr, DeviceId);
	if (Status == XST_SUCCESS) {
		return XST_SUCCESS;
	}
	return XST_FAILURE;
}

int main(void)
{
	init_platform();
	printf("Hello World\n\r");

	s32 status;

	XIicPs_Config *Config;
	Config = XIicPs_LookupConfig(IIC_DEVICE_ID);
	if(Config == NULL)
	{
		printf("IIC_DEVICE_ID - LookupConfig - failed\n\r");
		return XST_FAILURE;
	}
	else
	{
		printf("IIC_DEVICE_ID - LookupConfig - passed\n\r");
	}

	status = XIicPs_CfgInitialize(&Iic, Config, Config->BaseAddress);
	if(status != XST_SUCCESS)
	{
		printf("IIC_DEVICE_ID - CfgInitialize - failed\n\r");
		return XST_FAILURE;
	}
	else
	{
		printf("IIC_DEVICE_ID - CfgInitialize - passed\n\r");
	}

	status = XIicPs_SelfTest(&Iic);
	if(status != XST_SUCCESS)
	{
		printf("IIC_DEVICE_ID - SelfTest - failed\n\r");
		return XST_FAILURE;
	}
	else
	{
		printf("IIC_DEVICE_ID - SelfTest - passed\n\r");
	}

	status = XIicPs_SetSClk(&Iic, IIC_SCLK_RATE);
	if(status != XST_SUCCESS)
	{
		printf("IIC_DEVICE_ID - SetSClk - failed\n\r");
		return XST_FAILURE;
	}
	else
	{
		printf("IIC_DEVICE_ID - SetSClk - passed\n\r");
	}

	u32 cnt = 0;
	while(cnt < 64)
	{
		status = IicPsFindDevice(IIC_ADDR_TMP006, IIC_DEVICE_ID);
		if(status == XST_SUCCESS)
		{
			printf("Found Slave device @ 0x%02x\n\r", IIC_ADDR_TMP006);
		}
		else
		{
			printf("Failed to find slave device @ 0x%02x\n\r", IIC_ADDR_TMP006);
		}
		cnt++;
	}

	return 0;
}
