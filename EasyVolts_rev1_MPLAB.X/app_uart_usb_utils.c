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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include "mcc_generated_files/mcc.h"
#include "usb_device_cdc.h"


// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data Types
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Macros or Functions
// *****************************************************************************
// *****************************************************************************

/******************************************************************************
 * Function:        void USART_putcUSART(char c)
 *
 * PreCondition:    None
 *
 * Input:           char c - character to print to the UART
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Print the input character to the UART
 *
 * Note:
 *
 *****************************************************************************/
void USART_putcUSART(char c)
{
      TXREG = c;
}


/******************************************************************************
 * Function:        void USART_mySetLineCodingHandler(void)
 *
 * PreCondition:    USB_CDC_SET_LINE_CODING_HANDLER is defined
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function gets called when a SetLineCoding command
 *                  is sent on the bus.  This function will evaluate the request
 *                  and determine if the application should update the baudrate
 *                  or not.
 *
 * Note:
 *
 *****************************************************************************/
#if defined(USB_CDC_SET_LINE_CODING_HANDLER2)
void USART_mySetLineCodingHandler(void)
{
    //If the request is not in a valid range
    //if(cdc_notice.GetLineCoding.dwDTERate > 115200)
    //{
        //NOTE: There are two ways that an unsupported baud rate could be
        //handled.  The first is just to ignore the request and don't change
        //the values.  That is what is currently implemented in this function.
        //The second possible method is to stall the STATUS stage of the request.
        //STALLing the STATUS stage will cause an exception to be thrown in the
        //requesting application.  Some programs, like HyperTerminal, handle the
        //exception properly and give a pop-up box indicating that the request
        //settings are not valid.  Any application that does not handle the
        //exception correctly will likely crash when this requiest fails.  For
        //the sake of example the code required to STALL the status stage of the
        //request is provided below.  It has been left out so that this demo
        //does not cause applications without the required exception handling
        //to crash.
        //---------------------------------------
        //USBStallEndpoint(0,1);
    //}
    //else
    //{
        //Update the baudrate info in the CDC driver
        CDCSetBaudRate2(cdc_notice2.GetLineCoding.dwDTERate);
        //Update the baudrate of the UART
        uint32_t dwBaud;
        dwBaud = ((_XTAL_FREQ/4) / line_coding2.dwDTERate) - 1;
        SPBRG = (uint8_t) dwBaud;
        SPBRGH = (uint8_t)((uint16_t) (dwBaud >> 8));
    //}
}
#endif

/******************************************************************************
 * Function:        void USART_getcUSART(char c)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          unsigned char c - character to received on the UART
 *
 * Side Effects:    None
 *
 * Overview:        Print the input character to the UART
 *
 * Note:
 *
 *****************************************************************************/
unsigned char USART_getcUSART ()
{
	char  c;

	if (RCSTAbits.OERR)  // in case of overrun error
	{                    // we should never see an overrun error, but if we do,
		RCSTAbits.CREN = 0;  // reset the port
		c = RCREG;
		RCSTAbits.CREN = 1;  // and keep going.
	}
	else
    {
		c = RCREG;
    }
// not necessary.  EUSART auto clears the flag when RCREG is cleared
//	PIR1bits.RCIF = 0;    // clear Flag

  
	return c;
}

/*******************************************************************************
 End of File
*/
