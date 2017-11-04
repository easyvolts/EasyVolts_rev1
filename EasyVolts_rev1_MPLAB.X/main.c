/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/
// This software implements EasyVolt logic (composit USB CDC device made of 2 
// CDC ACM profiles, one to control power converter, second - to be used as 
// USB-UART/RS485 converter).
// Created by: Valerii Proskurin.
// Date: Feb 2017. 

/** INCLUDES *******************************************************/
#include "system.h"

#include "app_uart_usb_utils.h"

#include "usb.h"
#include "usb_device.h"
#include "usb_device_cdc.h"
#include "mcc_generated_files/mcc.h"
#include <stdlib.h>

//#define PWM_TEST
#define PWM_PERIOD              (188)// (333)//142kHz. (283)//166kHz, (237) //200KHz, (187)//250kHz, (93)//500KHz,
#define PWM_PERIOD_LIMIT        (PWM_PERIOD)

#define MAX_POWER_mWt           (2400000U)

#define MAX_UOUT_mV             (15000U)
#define MIN_UOUT_mV             (0U)

#define MAX_IOUT_mA             (1000U)
#define MIN_IOUT_mA             (0U)

uint8_t PwmDutyCycleVoltageLimit = 0, PwmDutyCycleCurrentLimit = 0;
volatile uint16_t Uadc = 0, Iadc = 0;
uint16_t Iadc_limit = 0, Uadc_target = 0;

int32_t MAIN_Uout_target_mV = 0;
int32_t MAIN_Iout_limit_mA = 500;

static uint16_t MAIN_Uout_mV = 0;
static uint16_t MAIN_Iout_mA = 0;
static int8_t MAIN_ovcFlag = 0, MAIN_upConverterFlag = 0;

static uint8_t readBuffer[CDC_DATA_OUT_EP_SIZE];
static uint8_t writeBuffer[CDC_DATA_IN_EP_SIZE];
static uint8_t writeStatusBuffer[CDC_DATA_IN_EP_SIZE];
static uint8_t cmdRxBuffer[CDC_DATA_OUT_EP_SIZE];

static const uint8_t errorMsg[] = "Wrong cmd!\r\n";
static const uint8_t okMsg[] = "OK\r\n";

static uint8_t USB_Out_Buffer[CDC_DATA_OUT_EP_SIZE];
static uint8_t RS232_Out_Data[CDC_DATA_IN_EP_SIZE];
unsigned char  NextUSBOut;
unsigned char  NextUSBOut;
unsigned char  LastRS232Out;  // Number of characters in the buffer
unsigned char  RS232cp;       // current position within the buffer
unsigned char  RS232_Out_Data_Rdy = 0;

//we use stepDown for voltages 0..5v, stepUp for 5..15v
void main_switchStepUp_Down(int32_t Uout_mV)
{
   //disable power
   RA5_SetHigh();
   //adjust PWM
   if(Uout_mV <= 4500) { 
        PwmDutyCycleVoltageLimit = (PWM_PERIOD*(350+Uout_mV))/(5000);//use larger PWM due to losses in coil and diod
        PWM2_LoadDutyValue(0);
        PWM2CON = 0xD0; //active LOW
        APFCON = 0x04;
        MAIN_upConverterFlag = 0;
    } else if(Uout_mV <= 4850) {
        PwmDutyCycleVoltageLimit = 1;
        PWM2_LoadDutyValue(0);
        PWM2CON = 0xC0; //active HIGH
        APFCON = 0x00;
        MAIN_upConverterFlag = 1;
        RA5_SetLow();//enable power output
    } else {
        PwmDutyCycleVoltageLimit = 2 + ((PWM_PERIOD*(Uout_mV - 4850))/(Uout_mV));
        PWM2_LoadDutyValue(0);
        PWM2CON = 0xC0; //active HIGH
        APFCON = 0x00;
        MAIN_upConverterFlag = 1;
        RA5_SetLow();//enable power output
    }
}

void main_CDC_to_RS485_Initialize()
{   
    unsigned char i;
    
    line_coding2.bCharFormat = 0;
    line_coding2.bDataBits = 8;
    line_coding2.bParityType = 0;
    line_coding2.dwDTERate = 9600;

// 	 Initialize the arrays
	for (i=0; i<sizeof(USB_Out_Buffer); i++)
    {
		USB_Out_Buffer[i] = 0;
    }

	NextUSBOut = 0;
	LastRS232Out = 0;
}

#define mDataRdyUSART()         PIR1bits.RCIF
#define mTxRdyUSART()           TXSTAbits.TRMT

void main_CDC_to_RS485_Tasks()
{
    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. */
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
    }

    /* If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop. */
    if( USBIsDeviceSuspended()== true )
    {
        return;
    }

//    /* Check to see if there is a transmission in progress, if there isn't, then
//     * we can see about performing an echo response to data received.
//     */
//    if( USBUSARTIsTxTrfReady2() == true)
//    {
//        uint8_t i;
//        uint8_t numBytesRead;
//
//        numBytesRead = getsUSBUSART2(readBufferRS485, sizeof(readBufferRS485));
//
//        //for test only!!!!
//        if(numBytesRead > 0)
//        {
//            memcpy(writeBufferRS485,readBufferRS485, numBytesRead);
//            /* send out the "echo" data now.
//             */
//            putUSBUSART2(writeBufferRS485,numBytesRead);
//        }
//    }

    if (RS232_Out_Data_Rdy == 0)  // only check for new USB buffer if the old RS232 buffer is
	{						      // empty.  This will cause additional USB packets to be NAK'd
		LastRS232Out = getsUSBUSART2(RS232_Out_Data,64); //until the buffer is free.
		if(LastRS232Out > 0)
		{
			RS232_Out_Data_Rdy = 1;  // signal buffer full
			RS232cp = 0;  // Reset the current position
            IO_RC1_SetHigh(); //Set TXE signal for RS485
		} else {
            if(mTxRdyUSART()) IO_RC1_SetLow(); //Reset TXE signal for RS485
        }
	}

    
    //Check if one or more bytes are waiting in the physical UART transmit
    //queue.  If so, send it out the UART TX pin.
	if(RS232_Out_Data_Rdy && mTxRdyUSART())
	{
	        //Hardware flow control not being used.  Just send the data.
    		USART_putcUSART(RS232_Out_Data[RS232cp]);
    		++RS232cp;
    		if (RS232cp == LastRS232Out) {
                RS232_Out_Data_Rdy = 0;
            }
	}

    //Check if we received a character over the physical UART, and we need
    //to buffer it up for eventual transmission to the USB host.
	if(mDataRdyUSART() && (NextUSBOut < (CDC_DATA_OUT_EP_SIZE - 1)))
        {
		USB_Out_Buffer[NextUSBOut] = USART_getcUSART();
		++NextUSBOut;
		USB_Out_Buffer[NextUSBOut] = 0;
	}

    //Check if any bytes are waiting in the queue to send to the USB host.
    //If any bytes are waiting, and the endpoint is available, prepare to
    //send the USB packet to the host.
	if((USBUSARTIsTxTrfReady2()) && (NextUSBOut > 0))
	{
		putUSBUSART2(&USB_Out_Buffer[0], NextUSBOut);
		NextUSBOut = 0;
	}
    
    CDCTxService2();
}


void main_SetDcDcRegulator(int32_t * pU_mV, int32_t * pI_mA)
{
    //limit voltage
    if((*pU_mV) > MAX_UOUT_mV) (*pU_mV) = MAX_UOUT_mV;
    if((*pU_mV) < MIN_UOUT_mV) (*pU_mV) = MIN_UOUT_mV;
    //limit current
    if((*pI_mA) > MAX_IOUT_mA) (*pI_mA) = MAX_IOUT_mA;
    if((*pI_mA) < MIN_IOUT_mA) (*pI_mA) = MIN_IOUT_mA;
    
    //limit DC-DC power due to USB limitations
    if(((*pU_mV)*(*pI_mA)) > MAX_POWER_mWt) {
        *pI_mA = MAX_POWER_mWt/(*pU_mV);
    }
    
    //adjust dc-dc regulation limits
    //switch between step Up/Down modes
    INTERRUPT_PeripheralInterruptDisable();
    main_switchStepUp_Down(*pU_mV);
    Iadc_limit = 3+(*pI_mA)/10;
    Uadc_target = (100*(50+*pU_mV))/1761;//17.61v per 1024steps
    INTERRUPT_PeripheralInterruptEnable();
    MAIN_ovcFlag = 0;
}

// Implementation of itoa()
const uint16_t dividerArray[5][9] = { 
    {1,2,3,4,5,6,7,8,9},
    {10,20,30,40,50,60,70,80,90},
    {100,200,300,400,500,600,700,800,900},
    {1000,2000,3000,4000,5000,6000,7000,8000,9000},
    {10000,20000,30000,40000,50000,60000,0,0,0} }; //we have up to 65535 input number
    
void my_uitoa(char* str, uint16_t num)
{
    int8_t powerOfTen, ziffer = 5;
    uint8_t strIncremet = 0;

    for(powerOfTen=4; powerOfTen>=0; powerOfTen--) {
        *str = '0';
        for(; ziffer>=0; ziffer--) {            
            if(num > dividerArray[powerOfTen][ziffer]) {
                strIncremet = 1;
                *str += ziffer+1;
                num -= dividerArray[powerOfTen][ziffer];
                break;
            }
        }
        ziffer=8;
        str+=strIncremet;
    }
    
}

void main_Communication_Initialize(void)
{
    unsigned char i;
    
    line_coding.bCharFormat = 0;
    line_coding.bDataBits = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate = 9600;

    // clear the TMR1 interrupt flag
    TMR1IF = 0;
    // Reload the initial value of TMR1
    TMR1_Reload();
    
    // 	 Initialize the arrays
	for (i=0; i<sizeof(writeBuffer); i++)
    {
		writeBuffer[i] = 0;
    }    
}

void main_CommunicationHandler()
{
    static uint8_t step = 0;
    static uint8_t cmdRxBufferPointer = 0;
    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. */
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
    }

    /* If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop. */
    if( USBIsDeviceSuspended()== true )
    {
        return;
    }

    /* Check to see if there is a transmission in progress, if there isn't, then
     * we can see about performing an echo response to data received.
     */
    if( USBUSARTIsTxTrfReady() == true)
    {
        uint8_t i;
        uint8_t numBytesRead;

        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));

        if(numBytesRead) {
            /* For every byte that was read... */
            for(i=0; i<numBytesRead; i++)
            {
                cmdRxBuffer[cmdRxBufferPointer] = readBuffer[i];
                if((0x0A == cmdRxBuffer[cmdRxBufferPointer]) || (0x0D == cmdRxBuffer[cmdRxBufferPointer]))
                {
                    //execute command here
                    if(('U' == cmdRxBuffer[0]) || ('u' == cmdRxBuffer[0])) {
                        MAIN_Uout_target_mV = atoi(&cmdRxBuffer[1]);
//                        memcpy(writeBuffer,okMsg,sizeof(okMsg));
//                        putUSBUSART(writeBuffer,sizeof(okMsg)-1);
                        main_SetDcDcRegulator(&MAIN_Uout_target_mV, &MAIN_Iout_limit_mA);
                        if(MAIN_Uout_target_mV) IO_RC0_SetHigh();
                        else IO_RC0_SetLow();
                    } else if(('I' == cmdRxBuffer[0]) || ('i' == cmdRxBuffer[0])) {
                        MAIN_Iout_limit_mA = atoi(&cmdRxBuffer[1]);                        
//                        memcpy(writeBuffer,okMsg,sizeof(okMsg));
//                        putUSBUSART(writeBuffer,sizeof(okMsg)-1);
                        main_SetDcDcRegulator(&MAIN_Uout_target_mV, &MAIN_Iout_limit_mA);
                        if(MAIN_Uout_target_mV) IO_RC0_SetHigh();
                        else IO_RC0_SetLow();
                    } else {
                        memcpy(writeBuffer,errorMsg,sizeof(errorMsg));
                        putUSBUSART(writeBuffer,sizeof(errorMsg)-1);
                    }
                    //reset buffer
                    cmdRxBufferPointer = 0;
                    memset(cmdRxBuffer,0,sizeof(cmdRxBuffer));
                    break;
                }
                cmdRxBufferPointer++;
                //check buffer overflow
                if(sizeof(cmdRxBuffer) < cmdRxBufferPointer) {
                    cmdRxBufferPointer = 0;
                    memset(cmdRxBuffer,0,sizeof(cmdRxBuffer));
                    break;
                }
            }
        } else if(TMR1IF) {
            switch (step){
            case 0:
                MAIN_Uout_mV = (1761*(uint32_t)Uadc)/100;
                MAIN_Iout_mA = Iadc*10;
                step++;
                break;
            case 1:                    
                writeStatusBuffer[0] = 0;
                memcpy(writeStatusBuffer,"EasyVolts \tU(mV)=      \t I(mA)=     \r\n", 38/*+5*/);
                step++;
                break;
            case 2:                    
                my_uitoa(&writeStatusBuffer[17],MAIN_Uout_mV);//voltage
                step++;
                break;
            case 3:
                my_uitoa(&writeStatusBuffer[31],MAIN_Iout_mA);//current  
//                my_uitoa(&writeStatusBuffer[36],PwmDutyCycle);
                step++;
                break;
            default:
                //handle led and special symbols
                switch(MAIN_ovcFlag){
                    case 0:
                        if(0 == MAIN_Uout_target_mV) {
                            IO_RC0_SetLow();//switch off led and print "_" if output is OFF
                            writeStatusBuffer[24] = '_';
                        } else IO_RC0_SetHigh();
                        break;
                    case 1:
                        IO_RC0_Toggle();//blink led and print "^" if overcurrent
                        writeStatusBuffer[24] = '^';
                        break;
                    default:
                        IO_RC0_SetLow();//switch off led and print "!" if overcurrent error
                        writeStatusBuffer[24] = '!';
                        break;
                }
                putUSBUSART(writeStatusBuffer,38/*+5*/);
                // clear the TMR1 interrupt flag
                TMR1IF = 0;
                // Reload the initial value of TMR1
                TMR1_Reload();
                step = 0;
                break;
            }
        }
    }

    CDCTxService();
}

void main_overcurrentProtection()
{
    static uint8_t tOut = 0;
    static Iadc_avg = 0;
    
    Iadc_avg = ((Iadc_avg + Iadc)>>1);
    
    if((Iadc_avg <= Iadc_limit)&&(2 != MAIN_ovcFlag)) {
        if(PwmDutyCycleCurrentLimit < PWM_PERIOD) PwmDutyCycleCurrentLimit++;
        else MAIN_ovcFlag = 0;
    } else {
        if(PwmDutyCycleCurrentLimit > 0) {
            PwmDutyCycleCurrentLimit--;
        } else {
            if(tOut > 100) {
                tOut = 0;
                //disable power
                RA5_SetHigh();
                MAIN_ovcFlag = 2;
            } else {
                tOut++;
            }
        }      
    }
}
/********************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/
MAIN_RETURN main(void)
{
    SYSTEM_Initialize();
    TMR1_StartTimer();
//    TMR0_Reload();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    ADC_StartConversion();
    
    USBDeviceInit();
    USBDeviceAttach();
    
    main_SetDcDcRegulator(&MAIN_Uout_target_mV, &MAIN_Iout_limit_mA);
    
    while(1)
    {
        SYSTEM_Tasks();
//        #if defined(USB_POLLING)
//            // Interrupt or polling method.  If using polling, must call
//            // this function periodically.  This function will take care
//            // of processing and responding to SETUP transactions
//            // (such as during the enumeration process when you first
//            // plug in).  USB hosts require that USB devices should accept
//            // and process SETUP packets in a timely fashion.  Therefore,
//            // when using polling, this function should be called
//            // regularly (such as once every 1.8ms or faster** [see
//            // inline code comments in usb_device.c for explanation when
//            // "or faster" applies])  In most cases, the USBDeviceTasks()
//            // function does not take very long to execute (ex: <100
//            // instruction cycles) before it returns.
//            USBDeviceTasks();
//        #endif
        //Application specific tasks   
        main_overcurrentProtection();
        main_CommunicationHandler();
        CLRWDT();             
    }//end while
}//end main

/*******************************************************************************
 End of File
*/

void interrupt INTERRUPT_InterruptManager(void)
{
    static int8_t pwmCorrection = 0;
    uint16_t pwm;
    
    Uadc = (Uadc + ADC_GetConversionResult() - (Iadc>>1))>>1;
    FVRCON = 0x82; //switch Vref for adc to 2.048v and start I measurement
    ADC_SelectChannel(channel_AN3);
    if(MAIN_upConverterFlag) pwmCorrection = ((pwmCorrection<<2) + Iadc)>>3;
    else pwmCorrection = ((pwmCorrection<<1) + Iadc)>>2;
   
    if((Uadc > Uadc_target) || (2 == MAIN_ovcFlag)) {
        PWM2_LoadDutyValue(0);
    } else {
        pwm = PwmDutyCycleVoltageLimit+pwmCorrection;
        if(pwm > PWM_PERIOD_LIMIT) pwm = PWM_PERIOD_LIMIT;
        if(pwm > PwmDutyCycleCurrentLimit) {
            pwm = PwmDutyCycleCurrentLimit;
            MAIN_ovcFlag = 1;
        }
        PWM2_LoadDutyValue(pwm);
    }
        

    ADC_StartConversion();  
    main_CDC_to_RS485_Tasks();
    while(false == ADC_IsConversionDone());
    Iadc = ADC_GetConversionResult();
    FVRCON = 0x83;
    ADC_SelectChannel(channel_AN6);
    USBDeviceTasks();
    ADC_StartConversion();        
        
    // Clear the ADC interrupt flag
    PIR1bits.ADIF = 0; 
}