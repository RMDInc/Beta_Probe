/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"/******************************************************************************
*
* Main Dev Kit Application
* Revision 1.0
*
* Notes:
* Initial Software package that is developed for the Dev Kit.  07/06/16 - EBJ
* mkaffine 10/10/18: changes added such that anytime the code is
* waiting for a command to be entered, if a q is entered, it will
* quit the submenu and return to the main menu.
* mkaffine 1/3/19: changed a few sleeps to usleeps (case 0, case 1)
******************************************************************************/

#include "LApp.h"

//struct definitions
struct adc_event{
	unsigned int total_events;
	unsigned int event_number;
	unsigned int baseline_int;
	unsigned int short_int;
	unsigned int long_int;
	unsigned int full_int;
	unsigned int footer;
	unsigned int adc_ch_0;
	unsigned int adc_ch_1;
	unsigned int adc_ch_2;
	unsigned int adc_ch_3;
	unsigned int adc_ch_4;
	unsigned int adc_ch_5;
	unsigned int adc_ch_6;
	unsigned int adc_ch_7;
	unsigned int adc_ch_8;
	unsigned int adc_ch_9;
	unsigned int adc_ch_10;
	unsigned int adc_ch_11;
	unsigned int adc_ch_12;
	unsigned int adc_ch_13;
	unsigned int adc_ch_14;
	unsigned int adc_ch_15;
};

static XSpiPs SpiInstance;
//////////////////////////// MAIN //////////////////// MAIN //////////////
int main()
{
	// Initialize System
    init_platform();  		// This initializes the platform, which is ...
	ps7_post_config();
	Xil_DCacheDisable();	//
	InitializeAXIDma();		// Initialize the AXI DMA Transfer Interface
	int flag = 0;
	InitializeInterruptSystem(XPAR_PS7_SCUGIC_0_DEVICE_ID);

	// Initialize buffers
	memset(RecvBuffer, '0', 32);	// Clear RecvBuffer Variable
	memset(SendBuffer, '0', 32);	// Clear SendBuffer Variable
	u8 SendBuf[3];
	u8 RecvBuf[3];
	XSpiPs_Config *SpiConfig;
	u32 ByteCount = 3;
	u8 pdwn_opt;
	u8 pdwn;
	u8 pdwnMode_opt;
	u8 pdwnMode;
	u8 submenu;
	u8 ain_opt;
	u8 ain;
	u8 clockDelay;
	u8 clockDelay_opt;
	u8 cml;
	u8 cml_opt;
	u8 coarse;
	u8 coarse_opt;
	u8 fine;
	u8 fine_opt;
	u8 dataFormat;
	u8 ddrEn;
	u8 ddrEn_opt;
	u8 delayEn;
	u8 delayEn_opt;
	u8 dutyCycle;
	u8 dutyCycle_opt;
	u8 flexVref;
	u8 orEn_opt;
	u8 orEn;
	u8 orPos;
	u8 orPos_opt;
	u8 outEn_opt;
	u8 outEn;
	u8 outInv;
	u8 outPhase_opt;
	u8 outPhase;
	u8 pin23_opt;
	u8 pin23;
	u8 pin9;
	u8 pin9_opt;
	u8 testMode_opt;
	u8 testMode;

	//*******************Setup the UART **********************//
	XUartPs_Config *Config = XUartPs_LookupConfig(UART_DEVICEID);
	if (NULL == Config) { return 1;}
	Status = XUartPs_CfgInitialize(&Uart_PS, Config, Config->BaseAddress);
	if (Status != 0){ xil_printf("XUartPS did not CfgInit properly.\n");	}

//	/* Conduct a Selftest for the UART */
//	Status = XUartPs_SelfTest(&Uart_PS);
//	if (Status != 0) { xil_printf("XUartPS failed self test.\n"); }			//handle error checks here better

	/* Set to normal mode. */
	XUartPs_SetOperMode(&Uart_PS, XUARTPS_OPER_MODE_NORMAL);
	//*******************Setup the UART **********************//
	Xil_Out32 (XPAR_AXI_GPIO_16_BASEADDR, 16384);
	Xil_Out32 (XPAR_AXI_GPIO_17_BASEADDR , 1);
	//*******************Receive and Process Packets **********************//
	Xil_Out32 (XPAR_AXI_GPIO_0_BASEADDR, 38);
	Xil_Out32 (XPAR_AXI_GPIO_1_BASEADDR, 73);
	Xil_Out32 (XPAR_AXI_GPIO_2_BASEADDR, 169);
	Xil_Out32 (XPAR_AXI_GPIO_3_BASEADDR, 1050);
	Xil_Out32 (XPAR_AXI_GPIO_10_BASEADDR, 8989);
	//*******************enable the system **********************//
	Xil_Out32(XPAR_AXI_GPIO_18_BASEADDR, 1);
	//*******************Receive and Process Packets **********************//

	// *********** Setup the Hardware Reset GPIO ****************//
	GPIOConfigPtr = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(&Gpio, GPIOConfigPtr, GPIOConfigPtr ->BaseAddr);
	if (Status != XST_SUCCESS) { return XST_FAILURE; }
	XGpioPs_SetDirectionPin(&Gpio, SW_BREAK_GPIO, 1);
	// *********** Setup the Hardware Reset MIO ****************//

	// *********** Mount SD Card and Initialize Variables ****************//
	if( doMount == 0 ){			//Initialize the SD card here
		ffs_res = f_mount(0, "", 0);
		ffs_res = f_mount(&fatfs, "", 0);
		doMount = 1;
	}

	// *********** Mount SD Card and Initialize Variables ****************//

	//set variables

	// *********** SPI SET UP ******************************

	unsigned int optionBits = XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION;

	SpiConfig = XSpiPs_LookupConfig(SPI_DEVICE_ID);
	if (NULL == SpiConfig) {
		return XST_FAILURE;
	}

	Status = XSpiPs_CfgInitialize(&SpiInstance, SpiConfig, SpiConfig->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Status = XSpiPs_SelfTest(&SpiInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XSpiPs_SetOptions(&SpiInstance, optionBits);
	Status = XSpiPs_SetClkPrescaler(&SpiInstance, XSPIPS_CLK_PRESCALE_64);
	Status = XSpiPs_SetSlaveSelect(&SpiInstance, 0);

	// ******************* POLLING LOOP *******************//
	mode = 99;
	loopDelay = 100000;
	enable_state = 0;
	while(1){
		//XUartPs_SetOptions(&Uart_PS,XUARTPS_OPTION_RESET_RX);	// Clear UART Read Buffer
		memset(RecvBuffer, '0', 32);							// Clear RecvBuffer Variable

		//sleep(0.5);  // Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 0.5 s
		xil_printf("\n\rDevkit version 5.10.j*e*pi*k*alpha.b.q (17 JAN 2019) \n\r");
		xil_printf("MAIN MENU \n\r");
		xil_printf("Meg is the Unquestioned Supreme Overlord and the Divine Ruler of all she surveys\n\r");
		xil_printf("And all she does not survey\n\r");
		xil_printf("Meg is just the best\n\r");
		xil_printf("Meg is the most powerful being around.\n\r");
		xil_printf("Meg is a god among mortals. \n\r");
		xil_printf("Bow before Meg's excellence.\n\r");
		xil_printf("******\n\r");
		xil_printf(" 0) Set Mode of Operation\n\r");
		xil_printf(" 1) Enable or Disable the system\n\r");
		xil_printf(" 2) Stream Processed Data\n\r");
		xil_printf("\n\r***Setup Parameters *** \n\r");
		xil_printf(" 3) Set Trigger Threshold\n\r");
		xil_printf(" 4) Set Integration Times (Number of Clock Cycles * 4ns) \n\r");
		xil_printf("******\n\r");
		xil_printf(" 5) Stream Processed Data Transposed\n\r");
		xil_printf(" 6) Stream Processed Data - BINARY\n\r");
		xil_printf(" 7) SPI communication\n\r");
		xil_printf("******\n\r");

		ReadCommandPoll();
		u8 menusel = 0xff;

		sscanf((char *)RecvBuffer,"%02u",&menusel);
		if ( menusel < 0 || menusel > 7 ) {
			xil_printf(" Invalid Command: Enter 0-7 \n\r");
//			sleep(1);
		}

		switch (menusel) { // Switch-Case Menu Select

		case 0: //Set Mode of Operation
			mode = 99; //Detector->GetMode();
			xil_printf("\n\r AA Waveform Data: Enter 0 <return>\n\r");
			xil_printf(" Raw Waveform Data: Enter 1 <return>\n\r");
			//xil_printf(" NOT A THING: \t Enter 2 <return>\n\r");
			//xil_printf(" TRG Waveform Data: \t Enter 3 <return>\n\r");
			xil_printf(" Processed Data: Enter 4 <return>\n\r");
			flag = ReadCommandPoll();
			if(flag == 1)
			{
				break;
			} else {

			sscanf((char *)RecvBuffer,"%01u",&mode);

			if (mode != 0 && mode != 1 && mode != 4 ) { xil_printf("Invalid Command\n\r"); break; }
			Xil_Out32 (XPAR_AXI_GPIO_14_BASEADDR, ((u32)mode));
			// Register 14
			if ( mode == 0 ) { xil_printf("Transfer AA Waveforms\n\r"); }
			if ( mode == 1 ) { xil_printf("Transfer Raw Waveforms\n\r"); }
			if ( mode == 2 ) { xil_printf("Transfer DFF Waveforms\n\r"); }
			if ( mode == 3 ) { xil_printf("Transfer TRG Waveforms\n\r"); }
			if ( mode == 4 ) { xil_printf("Transfer Processed Data\n\r"); }
			//usleep(1);

			break;}

		case 1: //Enable or disable the system
			xil_printf("\n\r Disable: Enter 0 <return>\n\r");
			xil_printf(" Enable: Enter 1 <return>\n\r");
			if (ReadCommandPoll()) {
				break;
			} else {
			sscanf((char *)RecvBuffer,"%01u",&enable_state);
			if (enable_state != 0 && enable_state != 1) { xil_printf("Invalid Command\n\r"); break; }
			Xil_Out32(XPAR_AXI_GPIO_18_BASEADDR, ((u32)enable_state));
			// Register 18 Out enabled, In Disabled
			if ( enable_state == 1 ) { xil_printf("DAQ Enabled\n\r"); }
			if ( enable_state == 0 ) { xil_printf("DAQ Disabled\n\r"); }
			//usleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s

//			dTime += 1;
//			iSprintfReturn = snprintf(cWriteToLogFile, LOG_FILE_BUFF_SIZE, "Enable system %d %f ", enable_state, dTime);
//
//			ffs_res = f_open(&logFile, cLogFile, FA_OPEN_ALWAYS | FA_WRITE);
//			if(ffs_res){
//				xil_printf("Could not open file %d", ffs_res);
//				break;
//			}
//			ffs_res = f_lseek(&logFile, filptr_clogFile);
//			ffs_res = f_write(&logFile, cWriteToLogFile, iSprintfReturn, &numBytesWritten);
//			filptr_clogFile += numBytesWritten;												// Add the number of bytes written to the file pointer
//			snprintf(cWriteToLogFile, 10, "%d", filptr_clogFile);							// Write that to a string for saving
//			ffs_res = f_lseek(&logFile, (10 - LNumDigits(filptr_clogFile)));				// Seek to the beginning of the file skipping the leading zeroes
//			ffs_res = f_write(&logFile, cWriteToLogFile, LNumDigits(filptr_clogFile), &numBytesWritten); // Write the new file pointer
//			ffs_res = f_close(&logFile);

			break;}

		case 2: //Continuously Read of Processed Data
		case 5: //Continuously Read of Processed Data
		case 6: //Continuously Read of Processed Data
			// the user needs to set the mode before coming here; see case 0
			if (!(enable_state)) {
				xil_printf("You must enable the system for data\n\r");
				break;
			}

			ipollReturn = 0;

			xil_printf("Mode = %d",mode);
			switch(mode)
			{
			case 0:
				//AA Waveforms
				while(ipollReturn != 113)	//loop until getting a 'q' in the RecvBuffer
				{
					usleep(loopDelay);
					ipollReturn = PollUart((char *)RecvBuffer, &Uart_PS);	//return value of 97(a), 113(q), or 0(else)
					if(ipollReturn == 113)
						break;

					if(Xil_In32(XPAR_AXI_GPIO_11_BASEADDR) > 0)	//if the buffer is full, read it to the screen
					{
						DAQ();
						ipollReturn = PrintWFData();
					}
				}
				break;
		    case 1:
				//Raw Waveforms
				while(ipollReturn != 113)	//loop until getting a 'q' in the RecvBuffer
				{
					usleep(loopDelay);
					ipollReturn = PollUart((char *)RecvBuffer, &Uart_PS);	//return value of 97(a), 113(q), or 0(else)
					if(ipollReturn == 113)
						break;

					if(Xil_In32(XPAR_AXI_GPIO_11_BASEADDR) > 0)	//if the buffer is full, read it to the screen
					{
						DAQ();
						ipollReturn = PrintWFData();
					}
				}
				break;
		    case 3:
		    	break;
			case 4:
				ClearBuffers();
				//Processed Data
				while(ipollReturn != 113)	//loop until getting a 'q' in the RecvBuffer
				{
//					usleep(loopDelay);
					ipollReturn = PollUart((char *)RecvBuffer, &Uart_PS);	//return value of 97(a), 113(q), or 0(else)
					if(ipollReturn == 113)
						break;

					if(Xil_In32(XPAR_AXI_GPIO_11_BASEADDR) > 0)	//if the buffer is full, read it to the screen
					{
						DAQ();
						ipollReturn = PrintData(menusel);
					}
				}
				break;
			default:
				xil_printf("Invalid mode\n\r");
				break;
			}

			//do this regardless of mode, before exiting
			ipollReturn = 0;
			break;
		case 3: //Set Threshold
			iThreshold0 = Xil_In32(XPAR_AXI_GPIO_10_BASEADDR);
			xil_printf("\n\r Existing Trigger = %d \n\r",Xil_In32(XPAR_AXI_GPIO_10_BASEADDR));
			xil_printf("In summary: This is delta between 2 adc values sampled 16ns apart.\n\r");
			xil_printf("I will probably change the value you will give this in a future update.\n\r");
			xil_printf("Just giving you a heads up.\n\r");
			xil_printf(" Enter New Trigger (0 to 10240) <return> \n\r");
			if (ReadCommandPoll()) {
				break;
			} else {
			sscanf((char *)RecvBuffer,"%05u",&iThreshold1);
			//if((iThreshold1 < 0) || (iThreshold1 > 10240)) { xil_printf("Invalid command\r\n"); break; }
			Xil_Out32(XPAR_AXI_GPIO_10_BASEADDR, ((u32)iThreshold1));
			xil_printf("New Threshold = %d \n\r",Xil_In32(XPAR_AXI_GPIO_10_BASEADDR));
			//usleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;}
		case 4: //Set Integration Times
			setIntsArray[4] = -52 + ((int)Xil_In32(XPAR_AXI_GPIO_0_BASEADDR))*4;
			setIntsArray[5] = -52 + ((int)Xil_In32(XPAR_AXI_GPIO_1_BASEADDR))*4;
			setIntsArray[6] = -52 + ((int)Xil_In32(XPAR_AXI_GPIO_2_BASEADDR))*4;
			setIntsArray[7] = -52 + ((int)Xil_In32(XPAR_AXI_GPIO_3_BASEADDR))*4;

			xil_printf("\n\r Existing Integration Times \n\r");
			xil_printf(" Time = 0 ns is when the Pulse Crossed Threshold \n\r");
			xil_printf(" Baseline Integral Window  [-200ns,%dns] \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_0_BASEADDR))*4 );
			xil_printf(" Short Integral Window  [-200ns,%dns] \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_1_BASEADDR))*4 );
			xil_printf(" Long Integral Window   [-200ns,%dns] \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_2_BASEADDR))*4 );
			xil_printf(" Full Integral Window   [-200ns,%dns] \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_3_BASEADDR))*4 );
			xil_printf(" Change: (Y)es (N)o <return>\n\r");
			if(ReadCommandPoll()) {
				break;
			} else {
			sscanf((char *)RecvBuffer,"%c",&updateint);

			if (updateint == 'N' || updateint == 'n')
				break;
			if (updateint == 'Y' || updateint == 'y')
				SetIntegrationTimes(&setIntsArray, &setSamples);
			//usleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;}
/*
		case 5: //Perform a DMA transfer
			xil_printf("\n\r Perform DMA Transfer of Waveform Data\n\r");
			xil_printf(" Press 'q' to Exit Transfer  \n\r");
			Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x48, 0xa000000);
			Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x58 , 65536);
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			PrintData(mode);		// Display data to console.
			sw = 0;   // broke out of the read loop, stop swith reset to 0
			break;

		case 6: //Perform a DMA transfer of Processed data
			xil_printf("\n\r ********Perform DMA Transfer of Processed Data \n\r");
			xil_printf(" Press 'q' to Exit Transfer  \n\r");
			//Xil_Out32 (XPAR_AXI_GPIO_18_BASEADDR, 0);	// Disable : GPIO Reg Capture Module
			Xil_Out32 (XPAR_AXI_GPIO_15_BASEADDR, 1);	// Enable: GPIO Reg to Readout Data MUX
			//sleep(1);				// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x48, 0xa000000); // Transfer from BRAM to DRAM, start address 0xa000000, 16-bit length
			Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x58 , 65536);
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			Xil_Out32 (XPAR_AXI_GPIO_15_BASEADDR, 0); 	// Disable: GPIO Reg turn off Readout Data MUX
			ClearBuffers();
			PrintData(mode);								// Display data to console.
			//Xil_Out32 (XPAR_AXI_GPIO_18_BASEADDR, 1);	// Enable : GPIO Reg Capture Module
			sw = 0;   // broke out of the read loop, stop swith reset to 0
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;

		case 7: //Check the Size of the Data Buffers
			databuff = Xil_In32 (XPAR_AXI_GPIO_11_BASEADDR);
			xil_printf("\n\r BRAM Data Buffer Size = %d \n\r",databuff);
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;
*/
		case 7: //SPI Communication
			if (!enable_state) {
				xil_printf("You can't communicate with the ADC if it's off. Try again.\n");
				break;
			}
			xil_printf("*****SPI COMMUNICATION OPTIONS*****\n\r");
			xil_printf("***DISCLAIMER: MEG IS NOT RESPONSIBLE FOR USER'S INPUTS\n\r");
			xil_printf("MEG DOES NOT CLAIM TO KNOW WHAT THE DIFFERENT COMBINATIONS WILL DO\n\r");
			xil_printf("0) Chip port configuration \n\r");
			xil_printf("1) Chip ID : Reads back the Identity of the ADC (expect 0x0C)\n\r");
			xil_printf("2) Chip Speed Grade - Reads back the speed grade of the device\n\r");
			xil_printf("3) Select mode of chip operation *External PDWN pin overrides this setting\n\r");
			xil_printf("4) Duty Cycle Stabilizer enable or disable\n\r");
			xil_printf("5) Enable test data on output pins\n\r");
			xil_printf("6) VCM and Analog input disable controls\n\r");
			xil_printf("7) Output mode\n\r");
			xil_printf("8) Output adjust\n\r");
			xil_printf("9) Output phase\n\r");
			xil_printf("10) flex output delay\n\r");
			xil_printf("11) flex Voltage reference\n\r");
			xil_printf("12) Overrange configuration\n\r");

			ReadCommandPoll();
			submenu = 15;
			sscanf(RecvBuffer,"%u",&submenu);
			if (submenu < 0 || submenu > 12) {
				xil_printf("Invalid Command\n\r");
				break; }

			switch(submenu) {
			case 0: // Chip port Configuration
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				xil_printf("You are not allowed to change how the bytes are sent\n\r");
				break;
			case 1: //Read back of chip id
				SendBuf[0] = 0x80;	//Read 1 byte
				SendBuf[1] = 0x01;	//from register 0x01
				SendBuf[2] = 0x00;	//Dummy byte for read cycle
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, RecvBuf, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				printf("The ADC chip ID is:  0x%x\r\n", RecvBuf[2]);
				break;
			case 2: //Read back of chip grade
				xil_printf("Speed Grade Options\n\r");
				xil_printf("250 MSPS -> Code: 0x00\n\r");
				xil_printf("210 MSPS -> Code: 0x08\n\r");
				xil_printf("170 MSPS -> Code: 0x10\n\r");
				SendBuf[0] = 0x80;	//Read 1 byte
				SendBuf[1] = 0x02;	//from register 0x01
				SendBuf[2] = 0x00;	//Dummy byte for read cycle
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, RecvBuf, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				printf("The ADC speed grade is:  0x%x\r\n", RecvBuf[2]);
				break;
			case 3: //Modes
				SendBuf[0] = 0x80;	//Read 1 byte
				SendBuf[1] = 0x08;	//from register 0x01
				SendBuf[2] = 0x00;	//Dummy byte for read cycle
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, RecvBuf, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				pdwn = (RecvBuf[2] >> 5);
				pdwnMode = (0x07 & RecvBuf[2]);
				if (pdwn == 1) {
					xil_printf("PDWN is currently set to Standby.\n\r");
				} else {
					xil_printf("PDWN is currently set to Full.\n\r");
				}
				if (pdwnMode == 0){
					xil_printf("Power Down Mode is Normal (Power up).\n\r");
				} else if (pdwnMode == 1) {
					xil_printf("Power Down Mode is Full Power Down\n\r");
				} else if (pdwnMode == 2) {
					xil_printf("Power Down Mode is Standby\n\r");
				} else if (pdwnMode == 3) {
					xil_printf("Power Down Mode is Normal\n\r");
				} else {
					xil_printf("So, this isn't an option? Something has gone wrong\n\r");
				}
				pdwn_opt = 3;
				xil_printf("OPTIONS - PDWN\n\r");
				xil_printf("0) Full\n\r");
				xil_printf("1) Standby\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&pdwn_opt);
				pdwnMode_opt = 7;
				xil_printf("OPTIONS - MODE\n\r");
				xil_printf("0) Normal Power Up\n\r");
				xil_printf("1) full power down\n\r");
				xil_printf("2) standby\n\r");
				xil_printf("3) Normal Power UP\n\r");
				xil_printf("I know that 0 and 3 look the same, but that's what the table in the datasheet said\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer, "%01u",&pdwnMode_opt);
				if (pdwn_opt == 1){
					SendBuf[2] = 0x20;
				} else {
					SendBuf[2] = 0x00;
				}
				if (pdwnMode_opt == 0) {
					SendBuf[2] = SendBuf[2] | 0x00;
				} else if (pdwnMode_opt == 1) {
					SendBuf[2] = SendBuf[2] | 0x01;
				} else if (pdwnMode_opt == 2) {
					SendBuf[2] = SendBuf[2] | 0x02;
				} else if (pdwnMode_opt == 3) {
					SendBuf[2] = SendBuf[2] | 0x03;
				} else {
					xil_printf("That's not actually an option. Try again\n\r");
					break;
				}
				SendBuf[0] = 0x00; //Writing 1 byte
				SendBuf[1] = 0x08; //Reading register 8
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				//Transfering to hardware register
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				SendBuf[0] = 0x00;
				SendBuf[1] = 0xFF;
				SendBuf[2] = 0x01;
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				break;
			case 4: //Duty cycle stabilizer enable/disable
				SendBuf[0] = 0x80;	//Read 1 byte
				SendBuf[1] = 0x09;	//from register 0x01
				SendBuf[2] = 0x00;	//Dummy byte for read cycle
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, RecvBuf, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				dutyCycle = RecvBuf[2];
				if (dutyCycle == 1) {
					xil_printf("Duty Cycle Stabilizer is currently enabled\n\r");
				} else {
					xil_printf("Duty Cycle Stabilizer is currently disenabled\n\r");
				}
				xil_printf("OPTIONS - Duty Cycle Stabilizer\n\r");
				xil_printf("0) Disabled\n\r");
				xil_printf("1) Enabled\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&dutyCycle_opt);
				if (dutyCycle_opt == 1){
					SendBuf[2] = 0x01;
				} else {
					SendBuf[2] = 0x00;
				}
				SendBuf[0] = 0x00; //Writing 1 byte
				SendBuf[1] = 0x09; //Reading register 8
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				//Transfering to hardware register
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				SendBuf[0] = 0x00;
				SendBuf[1] = 0xFF;
				SendBuf[2] = 0x01;
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				break;
			case 5: //Test output
				SendBuf[0] = 0x80;	//Read 1 byte
				SendBuf[1] = 0x0D;	//from register 0x01
				SendBuf[2] = 0x00;	//Dummy byte for read cycle
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, RecvBuf, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				testMode = RecvBuf[2] & 0x0F;
				pin23 = RecvBuf[2] >> 5;
				pin9 = (RecvBuf[2] >> 4) & 0x01;
				if (pin23 == 1) {
					xil_printf("Reset PN 23 gen is currently set to on.\n\r");
				} else {
					xil_printf("Reset PN 23 gen is currently off\n\r");
				}
				if (pin9 == 1) {
					xil_printf("Reset PN 9 gen is currently set to on.\n\r");
				} else {
					xil_printf("Reset PN 9 gen is currently off\n\r");
				}
				if (testMode == 0){
					xil_printf("Test Mode is off.\n\r");
				} else if (testMode == 1) {
					xil_printf("Test Mode is Midscale Short\n\r");
				} else if (testMode == 2) {
					xil_printf("Test Mode is +FS short\n\r");
				} else if (testMode == 3) {
					xil_printf("Test Mode is -FS short\n\r");
				} else if (testMode == 4) {
					xil_printf("Test Mode is checker board output\n\r");
				} else if (testMode == 5) {
					xil_printf("Test Mode is PN 23 Sequence\n\r");
				} else if (testMode == 6) {
					xil_printf("Test Mode is PN 9\n\r");
				} else if (testMode == 7) {
					xil_printf("Test Mode is one/zero word toggle\n\r");
				}	else {
					xil_printf("So, this isn't an option? Something has gone wrong\n\r");
				}
				pin9_opt = 3;
				pin23_opt = 3;
				testMode_opt = 8;
				xil_printf("OPTIONS - PN9\n\r");
				xil_printf("0) Off\n\r");
				xil_printf("1) On\n\r");
				xil_printf("If you select any other number, it's going to be OFF\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&pin9_opt);
				xil_printf("OPTIONS - PN23\n\r");
				xil_printf("0) Off\n\r");
				xil_printf("1) On\n\r");
				xil_printf("If you select any other number, it's going to be OFF\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&pin23_opt);
				xil_printf("OPTIONS - Test MODE\n\r");
				xil_printf("0) OFF \n\r");
				xil_printf("1) Midscale short\n\r");
				xil_printf("2) +FS short\n\r");
				xil_printf("3) -FS short\n\r");
				xil_printf("4) Checker Board Output\n\r");
				xil_printf("5) PN 23 Sequence\n\r");
				xil_printf("6) PN 9\n\r");
				xil_printf("7) one/zero word toggle\n\r");
				xil_printf("Any other number is going to go to the default OFF\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer, "%01u",&pdwnMode_opt);
				if (pin9 == 1){
					SendBuf[2] = 0x10;
				} else {
					SendBuf[2] = 0x00;
				}
				if (pin23 == 1){
					SendBuf[2] = SendBuf[2] | 0x20;
				} else {
					SendBuf[2] = SendBuf[2] | 0x00;
				}
				if (testMode_opt == 1) {
					SendBuf[2] = SendBuf[2] | 0x01;
				} else if (testMode_opt == 2) {
					SendBuf[2] = SendBuf[2] | 0x02;
				} else if (testMode_opt == 3) {
					SendBuf[2] = SendBuf[2] | 0x03;
				} else if (testMode_opt == 4) {
					SendBuf[2] = SendBuf[2] | 0x04;
				} else if (testMode_opt == 5) {
					SendBuf[2] = SendBuf[2] | 0x05;
				} else if (testMode_opt == 6) {
					SendBuf[2] = SendBuf[2] | 0x06;
				} else if (testMode_opt == 7) {
					SendBuf[2] = SendBuf[2] | 0x07;
				} else if (testMode_opt == 0) {
					SendBuf[2] = SendBuf[2] | 0x00;
				}else {
					xil_printf("As that is not a valid option, you have decided it should be off\n\r");
					SendBuf[2] = SendBuf[2] | 0x00;
				}
				SendBuf[0] = 0x00; //Writing 1 byte
				SendBuf[1] = 0x0D; //Reading register 8
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				//Transfering to hardware register
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				SendBuf[0] = 0x00;
				SendBuf[1] = 0xFF;
				SendBuf[2] = 0x01;
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				break;
			case 6: //analog input disable/VCM enable
				SendBuf[0] = 0x80;	//Read 1 byte
				SendBuf[1] = 0x0F;	//from register 0x01
				SendBuf[2] = 0x00;	//Dummy byte for read cycle
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, RecvBuf, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}cml = ((RecvBuf[2] >> 1) & 0x01);
				ain = RecvBuf[2] >> 2;
				if (cml == 1) {
					xil_printf("CML is currently set to on.\n\r");
				} else {
					xil_printf("CML is currently set to OFF.\n\r");
				}
				if (ain == 1){
					xil_printf("Analog Input Disable is on.\n\r");
				} else {
					xil_printf("Analog Input Disable is Disabled\n\r");
				}
				cml_opt = 3;
				ain_opt = 3;
				xil_printf("OPTIONS - CML\n\r");
				xil_printf("0) OFF\n\r");
				xil_printf("1) ON\n\r");
				xil_printf("If you select any other number, it's going to be OFF\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&cml_opt);
				xil_printf("OPTIONS - Analog Input Disable\n\r");
				xil_printf("0) OFF\n\r");
				xil_printf("1) ON\n\r");
				xil_printf("If you select any other number, it's going to be OFF\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&ain_opt);
				if (cml_opt == 1){
					xil_printf("Hello. You are indeed attempting to turn this on\n\r");
					SendBuf[2] = 0x02;
				} else {
					SendBuf[2] = 0x00;
				}
				if (ain_opt == 1) {
					SendBuf[2] = SendBuf[2] | 0x04;
				} else {
					SendBuf[2] = SendBuf[2] | 0x00;
				}
				xil_printf("I'm just going to hardcode this to turn VCM on");
				SendBuf[0] = 0x00; //Writing 1 byte
				SendBuf[1] = 0x0F; //Writing register 0x0f
				SendBuf[2] = 0x02;
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				//Transfering to hardware register
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				SendBuf[0] = 0x00;
				SendBuf[1] = 0xFF;
				SendBuf[2] = 0x01;
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				break;
			case 7: //output mode
				SendBuf[0] = 0x80;	//Read 1 byte
				SendBuf[1] = 0x14;	//from register 0x01
				SendBuf[2] = 0x00;	//Dummy byte for read cycle
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, RecvBuf, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				outEn = (RecvBuf[2] >> 4);
				ddrEn = (RecvBuf[2] >> 3) & 0x01;
				outInv = (RecvBuf[2] >> 2) & 0x01;
				dataFormat = RecvBuf[2] & 0x03;
				if (outEn == 1) {
					xil_printf("Output is currently disabled\n\r");
				} else {
					xil_printf("Output is currently enabled\n\r");
				}
				if (ddrEn == 1) {
					xil_printf("DDR is currently enabled\n\r");
				} else {
					xil_printf("DDR is currently disabled\n\r");
				}
				if (outInv == 1) {
					xil_printf("Output is currently inverted\n\r");
				} else {
					xil_printf("Output is currently not inverted\n\r");
				}
				if (dataFormat == 0){
					xil_printf("Data Format is Offset Binary\n\r");
				} else if (dataFormat == 1) {
					xil_printf("Data Format is two's complement\n\r");
					xil_printf("With the current programming on the FPGA, you're getting garbage\n\r");
				} else if (dataFormat == 2) {
					xil_printf("Power Down Mode is Standby\n\r");
					xil_printf("With the current programming on the FPGA, you're getting garbage\n\r");
				} else {
					xil_printf("So, this isn't an option? Something has gone wrong\n\r");
				}
				outEn_opt = 3;
				ddrEn_opt = 3;
				xil_printf("OPTIONS - Output Enable\n\r");
				xil_printf("0) Enable\n\r");
				xil_printf("1) Disable\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&outEn_opt);
				xil_printf("OPTIONS - DDR Enable\n\r");
				xil_printf("0) Disable\n\r");
				xil_printf("1) Enable\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&ddrEn_opt);
				xil_printf("As per order of Meg, you do not have permission to change anything else\n\r");
				xil_printf("This includes inverting the output and what data format we get\n\r");
				xil_printf("You'll have to live with the defaults\n\r");
				if (outEn_opt == 1){
					SendBuf[2] = 0x10;
				} else {
					SendBuf[2] = 0x00;
				}
				if (ddrEn_opt == 1){
					SendBuf[2] = SendBuf[2] | 0x08;
				} else {
					SendBuf[2] = SendBuf[2] | 0x00;
				}
				SendBuf[2] = SendBuf[2] & 0xF8;
				SendBuf[0] = 0x00; //Writing 1 byte
				SendBuf[1] = 0x14; //Reading register 8
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				//Transfering to hardware register
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				SendBuf[0] = 0x00;
				SendBuf[1] = 0xFF;
				SendBuf[2] = 0x01;
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				break;
			case 8: //output adjust
				SendBuf[0] = 0x80;	//Read 1 byte
				SendBuf[1] = 0x15;	//from register 0x01
				SendBuf[2] = 0x00;	//Dummy byte for read cycle
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, RecvBuf, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				coarse = (RecvBuf[2] >> 3) & 0x01;
				fine = RecvBuf[2] & 0x7;
				if (coarse == 1) {
					xil_printf("Course Adjust: 2.0mA\n\r");
				} else {
					xil_printf("Course Adjust Default: 3.5mA\n\r");
				}
				if (fine == 1){
					xil_printf("Fine adjust: 3.50mA\n\r");
				} else if (fine == 2) {
					xil_printf("Fine Adjust: 3.25mA\n\r");
				} else if (fine == 3) {
					xil_printf("Fine Adjust: 3.00mA\n\r");
				} else if (fine == 4) {
					xil_printf("Fine Adjust: 2.75mA\n\r");
				} else if (fine == 5) {
					xil_printf("Fine Adjust: 2.50mA\n\r");
				} else if (fine == 6) {
					xil_printf("Fine Adjust: 2.25mA\n\r");
				} else if (fine == 7) {
					xil_printf("Fine Adjust: 2.00mA\n\r");
				} else {
					xil_printf("Fine Adjust: Default\n\r");
				}
				coarse_opt = 3;
				fine_opt = 0;
				xil_printf("OPTIONS - Course\n\r");
				xil_printf("0) 3.5mA (Default)\n\r");
				xil_printf("1) 2.0mA\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&coarse_opt);
				fine_opt = 0;
				xil_printf("OPTIONS - MODE\n\r");
				xil_printf("0) Default\n\r");
				xil_printf("1) 3.5mA\n\r");
				xil_printf("2) 3.25mA\n\r");
				xil_printf("3) 3.00mA\n\r");
				xil_printf("4) 2.75mA\n\r");
				xil_printf("5) 2.50mA\n\r");
				xil_printf("6) 2.25mA\n\r");
				xil_printf("7) 2.00mA\n\r");
				xil_printf("Anything higher than 7 or not a number results in default\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer, "%01u",&fine_opt);
				if (coarse_opt == 1){
					SendBuf[2] = 0x08;
				} else {
					SendBuf[2] = 0x00;
				}
				if (fine_opt == 0) {
					SendBuf[2] = SendBuf[2] | 0x00;
				} else if (fine_opt == 1) {
					SendBuf[2] = SendBuf[2] | 0x01;
				} else if (fine_opt == 2) {
					SendBuf[2] = SendBuf[2] | 0x02;
				} else if (fine_opt == 3) {
					SendBuf[2] = SendBuf[2] | 0x03;
				} else if (fine_opt == 4) {
					SendBuf[2] = SendBuf[2] | 0x04;
				} else if (fine_opt == 5) {
					SendBuf[2] = SendBuf[2] | 0x05;
				} else if (fine_opt == 6) {
					SendBuf[2] = SendBuf[2] | 0x06;
				} else if (fine_opt == 7) {
					SendBuf[2] = SendBuf[2] | 0x07;
				} else {
					xil_printf("That's not actually an option. You get default\n\r");
					SendBuf[2] = SendBuf[2] | 0x00;
				}
				SendBuf[0] = 0x00; //Writing 1 byte
				SendBuf[1] = 0x15; //Reading register 8
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				//Transfering to hardware register
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				SendBuf[0] = 0x00;
				SendBuf[1] = 0xFF;
				SendBuf[2] = 0x01;
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				break;
			case 9: // output phase
				SendBuf[0] = 0x80;	//Read 1 byte
				SendBuf[1] = 0x16;	//from register 0x01
				SendBuf[2] = 0x00;	//Dummy byte for read cycle
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, RecvBuf, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				outPhase = (RecvBuf[2] >> 7);
				if (outPhase == 1) {
					xil_printf("Output clock polarity is currently inverted\n\r");
				} else {
					xil_printf("Output clock polarity is currently normal\n\r");
				}
				outPhase_opt = 3;
				xil_printf("OPTIONS - Clock Polarity\n\r");
				xil_printf("0) Normal\n\r");
				xil_printf("1) Inverted\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&outPhase_opt);
				if (outPhase_opt == 1){
					SendBuf[2] = 0x83;
				} else {
					SendBuf[2] = 0x03;
				}
				SendBuf[0] = 0x00; //Writing 1 byte
				SendBuf[1] = 0x16; //Reading register 8
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				//Transfering to hardware register
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				SendBuf[0] = 0x00;
				SendBuf[1] = 0xFF;
				SendBuf[2] = 0x01;
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				break;
			case 10: //Flex Output delay
				SendBuf[0] = 0x80;	//Read 1 byte
				SendBuf[1] = 0x17;	//from register 0x01
				SendBuf[2] = 0x00;	//Dummy byte for read cycle
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, RecvBuf, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				clockDelay = RecvBuf[2] & 0x1F;
				delayEn = (RecvBuf[2] >> 7) & 0x01;
				if (delayEn == 1) {
					xil_printf("Output delay enable is disabled.\n\r");
				} else {
					xil_printf("Output delay enable is enabled\n\r");
				}
				if (clockDelay == 0){
					xil_printf("Output clock delay is 0.1ns\n\r");
				} else if (clockDelay == 1) {
					xil_printf("Output clock delay is 0.2ns\n\r");
				} else if (clockDelay == 2) {
					xil_printf("Output clock delay is 0.3ns\n\r");
				} else if (clockDelay == 3) {
					xil_printf("Output clock delay is 0.4ns\n\r");
				} else if (clockDelay == 4) {
					xil_printf("Output clock delay is 0.5ns\n\r");
				} else if (clockDelay == 5) {
					xil_printf("Output clock delay is 0.6ns\n\r");
				} else if (clockDelay == 6) {
					xil_printf("Output clock delay is 0.7ns\n\r");
				} else if (clockDelay == 7) {
					xil_printf("Output clock delay is 0.8ns\n\r");
				} else if (clockDelay == 8) {
					xil_printf("Output clock delay is 0.9ns\n\r");
				} else if (clockDelay == 9) {
					xil_printf("Output clock delay is 1.0ns\n\r");
				} else if (clockDelay == 10) {
					xil_printf("Output clock delay is 1.1ns\n\r");
				} else if (clockDelay == 11) {
					xil_printf("Output clock delay is 1.2ns\n\r");
				} else if (clockDelay == 12) {
					xil_printf("Output clock delay is 1.3ns\n\r");
				} else if (clockDelay == 13) {
					xil_printf("Output clock delay is 1.4ns\n\r");
				} else if (clockDelay == 14) {
					xil_printf("Output clock delay is 1.5ns\n\r");
				} else if (clockDelay == 15) {
					xil_printf("Output clock delay is 1.6ns\n\r");
				} else if (clockDelay == 16) {
					xil_printf("Output clock delay is 1.7ns\n\r");
				} else if (clockDelay == 17) {
					xil_printf("Output clock delay is 1.8ns\n\r");
				} else if (clockDelay == 18) {
					xil_printf("Output clock delay is 1.9ns\n\r");
				} else if (clockDelay == 19) {
					xil_printf("Output clock delay is 2.0ns\n\r");
				} else if (clockDelay == 20) {
					xil_printf("Output clock delay is 2.1ns\n\r");
				} else if (clockDelay == 21) {
					xil_printf("Output clock delay is 2.2ns\n\r");
				} else if (clockDelay == 22) {
					xil_printf("Output clock delay is 2.3ns\n\r");
				} else if (clockDelay == 23) {
					xil_printf("Output clock delay is 2.4ns\n\r");
				} else if (clockDelay == 24) {
					xil_printf("Output clock delay is 2.5ns\n\r");
				} else if (clockDelay == 25) {
					xil_printf("Output clock delay is 2.6ns\n\r");
				} else if (clockDelay == 26) {
					xil_printf("Output clock delay is 2.7ns\n\r");
				} else if (clockDelay == 27) {
					xil_printf("Output clock delay is 2.8ns\n\r");
				} else if (clockDelay == 28) {
					xil_printf("Output clock delay is 2.9ns\n\r");
				} else if (clockDelay == 29) {
					xil_printf("Output clock delay is 3.0ns\n\r");
				} else if (clockDelay == 30) {
					xil_printf("Output clock delay is 3.1ns\n\r");
				} else if (clockDelay == 31) {
					xil_printf("Output clock delay is 3.2ns\n\r");
				} else {
					xil_printf("So, this isn't an option? Something has gone wrong\n\r");
				}
				delayEn_opt = 3;
				xil_printf("OPTIONS - Delay Enable\n\r");
				xil_printf("0) Enable\n\r");
				xil_printf("1) Disable\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&delayEn_opt);
				clockDelay_opt = 7;
				xil_printf("OPTIONS - clock delay\n\r");
				xil_printf("Shortest Clock delay is 0.1ns (option 0) \n\r");
				xil_printf("Longest Clock delays is 3.2ns (option 31)\n\r");
				xil_printf("Each option increments by .1ns\n\r");
				xil_printf("Ex. Option 1 is .2ns, .1ns more than Option 0\n\r");
				xil_printf("Type in a number from 0 to 31\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer, "%u",&clockDelay_opt);
				if (delayEn_opt == 1){
					SendBuf[2] = 0x80;
				} else {
					SendBuf[2] = 0x00;
				}
				if (clockDelay_opt == 0){
					SendBuf[2] = SendBuf[2] | 0x00;
				} else if (clockDelay_opt == 1) {
					SendBuf[2] = SendBuf[2] | 0x01;
				} else if (clockDelay_opt == 2) {
					SendBuf[2] = SendBuf[2] | 0x02;
				} else if (clockDelay_opt == 3) {
					SendBuf[2] = SendBuf[2] | 0x03;
				} else if (clockDelay_opt == 4) {
					SendBuf[2] = SendBuf[2] | 0x04;
				} else if (clockDelay_opt == 5) {
					SendBuf[2] = SendBuf[2] | 0x05;
				} else if (clockDelay_opt == 6) {
					SendBuf[2] = SendBuf[2] | 0x06;
				} else if (clockDelay_opt == 7) {
					SendBuf[2] = SendBuf[2] | 0x07;
				} else if (clockDelay_opt == 8) {
					SendBuf[2] = SendBuf[2] | 0x08;
				} else if (clockDelay_opt == 9) {
					SendBuf[2] = SendBuf[2] | 0x09;
				} else if (clockDelay_opt == 10) {
					SendBuf[2] = SendBuf[2] | 0x0A;
				} else if (clockDelay_opt == 11) {
					SendBuf[2] = SendBuf[2] | 0x0B;
				} else if (clockDelay_opt == 12) {
					SendBuf[2] = SendBuf[2] | 0x0C;
				} else if (clockDelay_opt == 13) {
					SendBuf[2] = SendBuf[2] | 0x0D;
				} else if (clockDelay_opt == 14) {
					SendBuf[2] = SendBuf[2] | 0x0E;
				} else if (clockDelay_opt == 15) {
					SendBuf[2] = SendBuf[2] | 0x0F;
				} else if (clockDelay_opt == 16) {
					SendBuf[2] = SendBuf[2] | 0x10;
				} else if (clockDelay_opt == 17) {
					SendBuf[2] = SendBuf[2] | 0x11;
				} else if (clockDelay_opt == 18) {
					SendBuf[2] = SendBuf[2] | 0x12;
				} else if (clockDelay_opt == 19) {
					SendBuf[2] = SendBuf[2] | 0x13;
				} else if (clockDelay_opt == 20) {
					SendBuf[2] = SendBuf[2] | 0x14;
				} else if (clockDelay_opt == 21) {
					SendBuf[2] = SendBuf[2] | 0x15;
				} else if (clockDelay_opt == 22) {
					SendBuf[2] = SendBuf[2] | 0x16;
				} else if (clockDelay_opt == 23) {
					SendBuf[2] = SendBuf[2] | 0x17;
				} else if (clockDelay_opt == 24) {
					SendBuf[2] = SendBuf[2] | 0x18;
				} else if (clockDelay_opt == 25) {
					SendBuf[2] = SendBuf[2] | 0x19;
				} else if (clockDelay_opt == 26) {
					SendBuf[2] = SendBuf[2] | 0x1A;
				} else if (clockDelay_opt == 27) {
					SendBuf[2] = SendBuf[2] | 0x1B;
				} else if (clockDelay_opt == 28) {
					SendBuf[2] = SendBuf[2] | 0x1C;
				} else if (clockDelay_opt == 29) {
					SendBuf[2] = SendBuf[2] | 0x1D;
				} else if (clockDelay_opt == 30) {
					SendBuf[2] = SendBuf[2] | 0x1E;
				} else if (clockDelay_opt == 31) {
					SendBuf[2] = SendBuf[2] | 0x1F;
				} else {
					xil_printf("I gave you 32 options, why did you not pick one of those? Take Default\n\r");
					SendBuf[2] = SendBuf[2] | 0x00;
				}
				SendBuf[0] = 0x00; //Writing 1 byte
				SendBuf[1] = 0x17; //Reading register 8
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				//Transfering to hardware register
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				SendBuf[0] = 0x00;
				SendBuf[1] = 0xFF;
				SendBuf[2] = 0x01;
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				break;
			case 11: //flex vref
				SendBuf[0] = 0x80;	//Read 1 byte
				SendBuf[1] = 0x18;	//from register 0x01
				SendBuf[2] = 0x00;	//Dummy byte for read cycle
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, RecvBuf, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				flexVref = RecvBuf[2];
				xil_printf("Input Voltage range setting is 0x%x\n\r",flexVref);
				xil_printf("Please check the datasheet for what that means\n\r");
				xil_printf("If you would like to be able to change this number, please reach out to Meg\n\r");
				xil_printf("The datasheet is currently too confusing for Meg to implement immediately\n\r");
				break;
			case 12: //overrange config
				SendBuf[0] = 0x80;	//Read 1 byte
				SendBuf[1] = 0x2A;	//from register 0x01
				SendBuf[2] = 0x00;	//Dummy byte for read cycle
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, RecvBuf, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				orPos = (RecvBuf[2] >> 1) & 0x01;
				orEn = (RecvBuf[2] & 0x01);
				xil_printf("OR position is for DDR mode only\n\r");
				if (orPos == 1) {
					xil_printf("OR position is Pin 21, Pin 22\n\r");
				} else {
					xil_printf("OR position is Pin 9, pin 10\n\r");
				}
				if (orPos == 1) {
					xil_printf("OR enable is on\n\r");
				} else {
					xil_printf("OR enable is off\n\r");
				}
				orEn_opt = 3;
				orPos_opt = 3;
				xil_printf("OPTIONS - OR Enable\n\r");
				xil_printf("0) OFF\n\r");
				xil_printf("1) ON\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&orEn_opt);
				xil_printf("OPTIONS - OR Position (DDR MODE ONLY)\n\r");
				xil_printf("0) Pin 9, Pin 10\n\r");
				xil_printf("1) Pin 21, Pin22\n\r");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%01u",&orPos_opt);
				if (orEn_opt == 0){
					SendBuf[2] = 0x00;
				} else {
					SendBuf[2] = 0x01;
				}
				if (orPos_opt == 1) {
					SendBuf[2] = SendBuf[2] | 0x02;
				} else {
					SendBuf[2] = SendBuf[2] | 0x00;
				}
				SendBuf[0] = 0x00; //Writing 1 byte
				SendBuf[1] = 0x2A; //Reading register 8
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				//Transfering to hardware register
				SendBuf[0] = 0x00;
				SendBuf[1] = 0xFF;
				SendBuf[2] = 0x01;
				Status = XSpiPs_PolledTransfer(&SpiInstance, SendBuf, NULL, ByteCount);
				while(SpiInstance.IsBusy == TRUE)
				{
						//hold the system here
				}
				break;
			}
			break;
		/*case 7: //Set DMA loop delay time
			xil_printf("\n\rEnter Loop Delay Time in uS: 1 to 1,000,000 <return> \n\r");
			if(ReadCommandPoll()) {
				break;
			} else {
			sscanf((char *)RecvBuffer,"%u",&loopDelay);
			xil_printf("New Delay Time = %u \n\r",loopDelay);
			//sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;}*/
//****HEY MEG, DOUBLECHECK THAT OLD CASE 8 ISN'T USED
//BUT ALSO CHECK IF THE OTHER ONES ARE NEEDED TOO

		/**** OLD CASE, CAN BE ADDED IN
		 * case 8: //Clear the processed data buffers
			xil_printf("\n\r Clear the Data Buffers\n\r");
			ClearBuffers();
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;*/

		case 8: //Event Rate
			xil_printf("Calculating Event Rate");
			t0 = Xil_In32(XPAR_AXI_GPIO_5_BASEADDR);
			t1 = t0;
			TC0 = Xil_In32(XPAR_AXI_GPIO_4_BASEADDR);
			TC1 = TC0;
			while(1) {
				if (t1 > t0) {
					eventRate = ((TC1-TC0)/((t1 - t0)*4));
					xil_printf("Event Rate (counts/s): %u", eventRate);
					break;
				}
				else {
					t1 = t0;
					t0 = Xil_In32(XPAR_AXI_GPIO_5_BASEADDR);
					TC1 = TC0;
					TC0 = Xil_In32(XPAR_AXI_GPIO_4_BASEADDR);
				}
			}


		case 9: //Print DMA Data
			xil_printf("\n\r Print Data\n\r");
			PrintData(mode);
			break;
		/*case 10: //Transfer data over serial port	// Skips setting integrals, threshold	//Difference is this doesn't save to sd, it prints out the data for us
			//xil_printf("321");	//echo back to the gui that the request was received
			mode = 0;
			Xil_Out32 (XPAR_AXI_GPIO_14_BASEADDR, ((u32)mode));	//sets mode to transfer AA waveforms
			sleep(1);
			enable_state = 1;
			Xil_Out32(XPAR_AXI_GPIO_18_BASEADDR, ((u32)enable_state));	//enables the system
			sleep(1);
			getWFDAQ();
			break;
		case 11: //change threshold over the serial connection
			xil_printf("Enter the new threshold: <enter>\n");
			if(ReadCommandPoll()){
				break;
			} else {
			sscanf((char *)RecvBuffer,"%04u",&iThreshold1);
			Xil_Out32(XPAR_AXI_GPIO_10_BASEADDR, ((u32)iThreshold1));
			xil_printf("New Threshold = %d \n\r",Xil_In32(XPAR_AXI_GPIO_10_BASEADDR));
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;}
		case 12: //change integrals over the serial connection
			xil_printf("Enter each integral time followed by <enter>");
			if(ReadCommandPoll()){
				break;
			}
			else {
			sscanf((char *)RecvBuffer,"%04u",&setBL);
			if(ReadCommandPoll()){
				break;
			} else {
			sscanf((char *)RecvBuffer,"%04u",&setSI);
			if(ReadCommandPoll()){
				break;
			} else {
			sscanf((char *)RecvBuffer,"%04u",&setLI);
			if(ReadCommandPoll()){
				break;
			} else {
			sscanf((char *)RecvBuffer,"%04u",&setFI);

			Xil_Out32 (XPAR_AXI_GPIO_0_BASEADDR, ((u32)((setBL+52)/4)));	//set baseline int time
			Xil_Out32 (XPAR_AXI_GPIO_1_BASEADDR, ((u32)((setSI+52)/4)));	//set short int time
			Xil_Out32 (XPAR_AXI_GPIO_2_BASEADDR, ((u32)((setLI+52)/4)));	//set long int time
			Xil_Out32 (XPAR_AXI_GPIO_3_BASEADDR, ((u32)((setFI+52)/4)));	//set full int time

			//echo back the changed values //for checking
			xil_printf("  Inputs Rounded to the Nearest 4 ns : Number of Samples\n\r");
			xil_printf("  Baseline Integral Window  [-200ns,%dns]: %d \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_0_BASEADDR))*4, 38+Xil_In32(XPAR_AXI_GPIO_0_BASEADDR) );
			xil_printf("  Short Integral Window 	  [-200ns,%dns]: %d \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_1_BASEADDR))*4, 38+Xil_In32(XPAR_AXI_GPIO_1_BASEADDR));
			xil_printf("  Long Integral Window      [-200ns,%dns]: %d \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_2_BASEADDR))*4, 38+Xil_In32(XPAR_AXI_GPIO_2_BASEADDR));
			xil_printf("  Full Integral Window      [-200ns,%dns]: %d \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_3_BASEADDR))*4, 38+Xil_In32(XPAR_AXI_GPIO_3_BASEADDR));
			break;}}}}
		case 13: //Transfer processed data over the serial port
			mode = 4;
			Xil_Out32 (XPAR_AXI_GPIO_14_BASEADDR, ((u32)mode));	//sets mode to processed data
			sleep(1);
			enable_state = 1;
			Xil_Out32(XPAR_AXI_GPIO_18_BASEADDR, ((u32)enable_state)); //enables the system
			sleep(1);
			getWFDAQ();
			break;
		case 14: //HV and temp control

			break;
		case 15:
			ffs_res = f_open(&directoryLogFile, cDirectoryLogFile0, FA_READ);
			dirSize = file_size(&directoryLogFile) - 10;
			filptr_cDIRFile = 10;
			dirFileContents = malloc(1 * dirSize + 1);
			ffs_res = f_lseek(&directoryLogFile, 10);
			ffs_res = f_read(&directoryLogFile, dirFileContents, dirSize, &numBytesRead);
			ffs_res = f_close(&directoryLogFile);
			snprintf(dirFileContents, dirSize + 1, dirFileContents + '\0');
			xil_printf(dirFileContents);
			free(dirFileContents);
			break;
		case 16:
			//DAQ minimal function
			//need to loop over this function
			// utilize DAQ function below (as opposed to WFDAQ)
			break;*/

		default :
			break;
		} // End Switch-Case Menu Select

	}	// ******************* POLLING LOOP *******************//

    cleanup_platform();
    return 0;
}
//////////////////////////// MAIN //////////////////// MAIN //////////////

//////////////////////////// InitializeAXIDma////////////////////////////////
// Sets up the AXI DMA
int InitializeAXIDma(void) {
	u32 tmpVal_0 = 0;

	tmpVal_0 = Xil_In32(XPAR_AXI_DMA_0_BASEADDR + 0x30);

	tmpVal_0 = tmpVal_0 | 0x1001; //<allow DMA to produce interrupts> 0 0 <run/stop>

	Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x30, tmpVal_0);
	Xil_In32(XPAR_AXI_DMA_0_BASEADDR + 0x30);	//what does the return value give us? What do we do with it?

	return 0;
}
//////////////////////////// InitializeAXIDma////////////////////////////////

//////////////////////////// InitializeInterruptSystem////////////////////////////////
int InitializeInterruptSystem(u16 deviceID) {
	int Status;

	GicConfig = XScuGic_LookupConfig (deviceID);

	if(NULL == GicConfig) {

		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&InterruptController, GicConfig, GicConfig->CpuBaseAddress);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;

	}

	Status = SetUpInterruptSystem(&InterruptController);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;

	}

	Status = XScuGic_Connect (&InterruptController,
			XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR,
			(Xil_ExceptionHandler) InterruptHandler, NULL);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;

	}

	XScuGic_Enable(&InterruptController, XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR );

	return XST_SUCCESS;

}
//////////////////////////// InitializeInterruptSystem////////////////////////////////


//////////////////////////// Interrupt Handler////////////////////////////////
void InterruptHandler (void ) {

	u32 tmpValue;
	tmpValue = Xil_In32(XPAR_AXI_DMA_0_BASEADDR + 0x34);
	tmpValue = tmpValue | 0x1000;
	Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x34, tmpValue);

	global_frame_counter++;
}
//////////////////////////// Interrupt Handler////////////////////////////////


//////////////////////////// SetUp Interrupt System////////////////////////////////
int SetUpInterruptSystem(XScuGic *XScuGicInstancePtr) {
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, XScuGicInstancePtr);
	Xil_ExceptionEnable();
	return XST_SUCCESS;

}
//////////////////////////// SetUp Interrupt System////////////////////////////////

//////////////////////////// ReadCommandPoll////////////////////////////////
// Function used to clear the read buffer
// Read In new command, expecting a <return>
// Returns buffer size read
//
/*int ReadCommandPoll() {
	u32 rbuff = 0;			// read buffer size returned

	XUartPs_SetOptions(&Uart_PS,XUARTPS_OPTION_RESET_RX);	// Clear UART Read Buffer
	memset(RecvBuffer, '0', 32);	// Clear RecvBuffer Variable
	while (!(RecvBuffer[rbuff-1] == '\n' || RecvBuffer[rbuff-1] == '\r'))
	{
		rbuff += XUartPs_Recv(&Uart_PS, &RecvBuffer[rbuff],(32 - rbuff));
		sleep(0.1);			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 0.1 s
	}
	return rbuff;
}*/

int ReadCommandPoll() {
	u32 rbuff = 0;			// read buffer size returned

	XUartPs_SetOptions(&Uart_PS,XUARTPS_OPTION_RESET_RX);	// Clear UART Read Buffer
	memset(RecvBuffer, '0', 32);	// Clear RecvBuffer Variable
	while (!(RecvBuffer[rbuff-1] == '\n' || RecvBuffer[rbuff-1] == '\r'))
	{
		if (RecvBuffer[rbuff-1] == 'q'){
			return 1;
		}
		rbuff += XUartPs_Recv(&Uart_PS, &RecvBuffer[rbuff],(32 - rbuff));
		//usleep(1);			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 0.1 s
	}
	return 0;
}
//////////////////////////// ReadCommandPoll////////////////////////////////


//////////////////////////// Set Integration Times ////////////////////////////////
// wfid = 	0 for AA
//			1 for DFF
//			2 for LPF
// At the moment, the software is expecting a 5 char buffer.  This needs to be fixed.
void SetIntegrationTimes(int * setIntsArray, u32 * setSamples){
	//int setIntsArray[8] = {};
	//u32 setsamples[8] = {};

	xil_printf("Enter Baseline Stop Time in ns: -52 to 0 <return> \n");
	if (ReadCommandPoll())
	{
		return;
	}
	sscanf((char *)RecvBuffer,"%d",&setIntsArray[0]);

	xil_printf("\n\rEnter Short Integral Stop Time in ns: -52 to 8000 <return> \n");
	if(ReadCommandPoll())
	{
		return;
	}
	sscanf((char *)RecvBuffer,"%d",&setIntsArray[1]);

	xil_printf("\n\rEnter Long Integral Stop Time in ns: -52 to 8000 <return> \n");
	if(ReadCommandPoll()){
		return;
	}
	sscanf((char *)RecvBuffer,"%d",&setIntsArray[2]);

	xil_printf("\n\rEnter Full Integral Stop Time in ns: -52 to 8000 <return> \n");
	if(ReadCommandPoll()){
		return;
	}
	sscanf((char *)RecvBuffer,"%d",&setIntsArray[3]);

	setSamples[0] = ((u32)((setIntsArray[0]+52)/4));
	setSamples[1] = ((u32)((setIntsArray[1]+52)/4));
	setSamples[2] = ((u32)((setIntsArray[2]+52)/4));
	setSamples[3] = ((u32)((setIntsArray[3]+52)/4));

	Xil_Out32 (XPAR_AXI_GPIO_0_BASEADDR, setSamples[0]);
	Xil_Out32 (XPAR_AXI_GPIO_1_BASEADDR, setSamples[1]);
	Xil_Out32 (XPAR_AXI_GPIO_2_BASEADDR, setSamples[2]);
	Xil_Out32 (XPAR_AXI_GPIO_3_BASEADDR, setSamples[3]);

	xil_printf("\n\rInputs Rounded to the Nearest 4 ns : Number of Samples\n");
	xil_printf("Baseline Integral Window  [-200ns,%dns]: %d \n",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_0_BASEADDR))*4, 38+Xil_In32(XPAR_AXI_GPIO_0_BASEADDR) );
	xil_printf("Short Integral Window 	  [-200ns,%dns]: %d \n",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_1_BASEADDR))*4, 38+Xil_In32(XPAR_AXI_GPIO_1_BASEADDR));
	xil_printf("Long Integral Window      [-200ns,%dns]: %d \n",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_2_BASEADDR))*4, 38+Xil_In32(XPAR_AXI_GPIO_2_BASEADDR));
	xil_printf("Full Integral Window      [-200ns,%dns]: %d \n",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_3_BASEADDR))*4, 38+Xil_In32(XPAR_AXI_GPIO_3_BASEADDR));

	xil_printf("value of gpio 3: %d\r\n", Xil_In32(XPAR_AXI_GPIO_3_BASEADDR));

	return;
}
//////////////////////////// Set Integration Times ////////////////////////////////

//////////////////////////// PrintData ////////////////////////////////
/*
 * Possible changes:
 * - read in entire buffer before processing/printing
 * - better validation
 * - static buffer(s)
 */
int PrintData(u8 menusel ){
	int ipollReturn = 0;
	int dram_addr = 0;
	int dram_base = 0xa000000;
	int dram_ceiling = 0xA004000; //read out just adjacent average (0xA004000 - 0xa000000 = 16384)
	int i = 0;

	//TODO: should move to static buffers as dynamic buffers like these two are unsafe for embedded systems
	unsigned int *data_array;
	data_array = (unsigned int *)calloc(17,sizeof(unsigned int));

	unsigned int *ch_data;
	ch_data = (unsigned int *)calloc(16,sizeof(unsigned int));

	Xil_DCacheInvalidateRange(0xa0000000, 65536);

	dram_addr = dram_base;
	while(dram_addr <= dram_ceiling)
	{
		//get the data
		// Data format form FPGA
		// 00: ID : 111111
		// 01: Total Events
		// 02: Event Number
		// 03: Baseline
		// 04: Short
		// 05: Long
		// 06: Full
		// 07: {Ch0,Ch1}
		// 08: {Ch2,Ch3}
		// 09: {Ch4,Ch5}
		// 10: {Ch6,Ch7}
		// 11: {Ch8,Ch9}
		// 12: {Ch10,Ch11}
		// 13: {Ch12,Ch13}
		// 14: {Ch14,Ch15}
		// 15: Temp
		// 16: Time


		for (i = 0; i<16; i++ ) { data_array[i] = data_array[i+1]; }
		data_array[16] = Xil_In32(dram_addr);

		if(data_array[0] == 111111 && data_array[1] != 111111 )
		{

			for ( i = 7; i<=14; i++ )
			{
				ch_data[(i-7)*2] = (32768 + (data_array[i] >> 16)) & 0xFFFF;      //msb=even channels
				ch_data[((i-7)*2)+1] =  (32768 + (data_array[i] & 0xFFFF)) & 0xFFFF;  //lsb=odd channels
			}

			// This is the correct mapping as of 7/2/2018.  Reason for channel swizzeling is unknown.  [SBL]
			// All printing methods return data in the correct order.
			switch(menusel)
			{
				case 5: // with tabs
					printf("%u\t", data_array[0]);	// ID 111111
					EncodePrint(data_array[16], 1);	// Time
					for ( i = 1; i<=6; i++ ) {EncodePrint(data_array[i], 1); }	// Total Evts, Evt number, BL, SI, LI, FI

					for ( i = 1; i<=7; i++ ) {EncodePrint(ch_data[i], 1);}
					EncodePrint(ch_data[0], 1);
					for ( i = 9; i<=15; i++ ) {EncodePrint(ch_data[i], 1);}
					EncodePrint(ch_data[8], 1);

					EncodePrint(data_array[15], 0);
					printf("\n");
					break;

				case 2: // no tabs
					printf("%u\n",data_array[0]);
					printf("%u\n",data_array[16]);
					for ( i = 1; i<=6; i++ ) { printf("%u\n",data_array[i]); }

					for ( i = 1; i<=7; i++ ) {printf("%u\n",ch_data[i]);}
					printf("%u\n",ch_data[0]);
					for ( i = 9; i<=15; i++ ) {printf("%u\n",ch_data[i]);}
					printf("%u\n",ch_data[8]);

					printf("%u\n",data_array[15]);
					break;

				case 6: // BINARY mode
					PrintBinBytes(0xffffffff, 4);  // binary header replaces 111111 ID code
					PrintBinBytes(data_array[16], 4);
					for ( i = 1; i<=6; i++ ) { PrintBinBytes(data_array[i], 4); }

					for ( i = 1; i<=7; i++ ) {PrintBinBytes(ch_data[i], 2);}
					PrintBinBytes(ch_data[0], 2);
					for ( i = 9; i<=15; i++ ) {PrintBinBytes(ch_data[i], 2);}
					PrintBinBytes(ch_data[8], 2);

					PrintBinBytes(data_array[15], 2);
					break;

				default:
					printf("Err: Unknown mode\n");
					break;
			}
		}

		dram_addr+=4;

//		if(dram_addr % 200 == 0)
//		{
//			ipollReturn = PollUart(RecvBuffer, &Uart_PS);	//return value of 97(a), 113(q), or 0(else)
//			if(ipollReturn == 113) 	break;
//		}
	}

	free(data_array);
	free(ch_data);

	return ipollReturn;
}
//////////////////////////// PrintData ////////////////////////////////


//////////////////////////// Print integer as Binary bytes ////////////////////////////////
void PrintBinBytes(u32 OutWord, u8 NumBytes) {

	for ( i = 1; i<=NumBytes; i++ )
	{
		printf("%c", (int)(OutWord & 0xff));
		OutWord = OutWord >> 8;
	}
}
//////////////////////////// Print integer as Binary bytes ////////////////////////////////

//////////////////////////// Print integer as Encoded Char bytes ////////////////////////////////
/*
 * This function encodes an integer value (can be signed or unsigned) and prints out a base 223
 *  equivalent value. There are various values within the char character set which have been
 *  skipped. This function replaces the printf function for this specific use.
 *
 * @param	(unsigned int)The value to be printed
 *
 * @return	0 on success, 1 on failure (function didn't print any characters
 */
int EncodePrint(unsigned int outWord, int printTab)
{
	int status = 0;
	int i = 0;
	int iter = 0;
	int characters_printed = 0;
	unsigned int qVal[10] = {};
	int rVal[10] = {};

	qVal[0] = outWord;
	while(qVal[iter] > 0)
	{
		rVal[iter] = qVal[iter] % BASE;
		qVal[iter+1] = floor(qVal[iter] / BASE);
		iter++; i++;
	}

	//print the values from MSB -> LSB
	iter = 0;
	for(iter = i - 1; iter >= 0; iter--)
	{
		if(rVal[iter] >= 127)
			rVal[iter] += 1;
		characters_printed = printf("%c", rVal[iter] + CHAR_OFFSET);
	}

	if(printTab == 1)
		printf("\t");

	if(characters_printed == 0)
		status = 1;

	return status;
}
//////////////////////////// Print integer as Encoded Char bytes ////////////////////////////////

//////////////////////////// Clear Processed Data Buffers ////////////////////////////////
void ClearBuffers() {
	Xil_Out32(XPAR_AXI_GPIO_9_BASEADDR,1);
	usleep(1);
	Xil_Out32(XPAR_AXI_GPIO_9_BASEADDR,0);
}
//////////////////////////// Clear Processed Data Buffers ////////////////////////////////

//////////////////////////// DAQ ////////////////////////////////
int DAQ() {
// Why??  You can't re-write to the DMA length register once the DMA starts, otherwise data corruption results...  [SBL - 070218]
//	Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x48, 0xa000000); 		// DMA Transfer Step 1
//	Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x58 , 65536);			// DMA Transfer Step 2

	Xil_Out32 (XPAR_AXI_GPIO_15_BASEADDR, 1);
	Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x48, 0xa000000);
	Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x58 , 65536);
//	usleep(100);
	usleep(52);
	Xil_Out32 (XPAR_AXI_GPIO_15_BASEADDR, 0);

	ClearBuffers();

	return 0;
}
//////////////////////////// DAQ ////////////////////////////////

//////////////////////////// getWFDAQ ////////////////////////////////

int getWFDAQ(){
	int buffsize; 	//BRAM buffer size
	//int dram_addr;	// DRAM Address

	XUartPs_SetOptions(&Uart_PS,XUARTPS_OPTION_RESET_RX);

	Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x48, 0xa000000); 		// DMA Transfer Step 1
	Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x58 , 65536);			// DMA Transfer Step 2
	//sleep(1);						// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
	ClearBuffers();
	//for (dram_addr = 0xa000000; dram_addr <= 0xA004000; dram_addr+=4){Xil_In32(dram_addr);}

	while(1){
		if (!sw) { sw = XGpioPs_ReadPin(&Gpio, SW_BREAK_GPIO); } //read pin
		XUartPs_Recv(&Uart_PS, (u8 *)RecvBuffer, 32);
		if ( RecvBuffer[0] == 'q' ) { sw = 1; }
		if(sw) { return sw;	}

		buffsize = Xil_In32 (XPAR_AXI_GPIO_11_BASEADDR);	// AA write pointer // tells how far the system has read in the AA module
		if(buffsize >= 4095)
		{
			Xil_Out32 (XPAR_AXI_GPIO_15_BASEADDR, 1);				// init mux to transfer data between integrater modules to DMA
			Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x48, 0xa000000);
			Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x58 , 65536);
			sleep(1); 												// this will change
			Xil_Out32 (XPAR_AXI_GPIO_15_BASEADDR, 0);

			ClearBuffers();
			PrintData(mode);
		}
	}
	return sw;
}

//////////////////////////// getWFDAQ ////////////////////////////////

//////////////////////////// PrintWFData ////////////////////////////////
int PrintWFData( ){
	int ipollReturn = 0;
	int wf_data_value = 0;
	int dram_addr = 0;
	int dram_base = 0xa000000;
	int dram_ceiling = 0xA004000; //read out just adjacent average (0xA004000 - 0xa000000 = 16384)

	Xil_DCacheInvalidateRange(0xa0000000, 65536);

	dram_addr = dram_base;
	xil_printf("111111\r\n");
	while(dram_addr <= dram_ceiling)
	{
		//get the data
		wf_data_value = Xil_In32(dram_addr);
		xil_printf("%d\n",wf_data_value);
		dram_addr+=4;

		//check for user input
		ipollReturn = PollUart(RecvBuffer, &Uart_PS);	//return value of 97(a), 113(q), or 0(else)
		if(ipollReturn == 113){
			break;
		}
	}

	return ipollReturn;
}
//////////////////////////// PrintWFData ////////////////////////////////
