 /**
   PWM2 Generated Driver File  
 
   @Company
     Microchip Technology Inc. 

   @File Name
     pwm2.c

   @Summary
     This is the generated driver implementation file for the PWM2 driver using MPLAB(c) Code Configurator

   @Description
     This header file provides implementations for driver APIs for PWM2.
     Generation Information :
         Product Revision  :  MPLAB(c) Code Configurator - 4.15
         Device            :  PIC16F1455
         Driver Version    :  2.00
     The generated drivers are tested against the following:
         Compiler          :  XC8 1.35
         MPLAB             :  MPLAB X 3.40
 */

 /*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

 #ifndef _PWM2_H
 #define _PWM2_H
 
 /**
   Section: Included Files
 */

 #include <xc.h>
 #include <stdint.h>

 #ifdef __cplusplus  // Provide C++ Compatibility

     extern "C" {

 #endif

 /**
   Section: Macro Declarations
 */

 #define PWM2_INITIALIZE_DUTY_VALUE    0

 /**
   Section: PWM Module APIs
 */

 /**
   @Summary
     Initializes the PWM2

   @Description
     This routine initializes the EPWM2_Initialize
     This routine must be called before any other PWM2 routine is called.
     This routine should only be called once during system initialization.

   @Preconditions
     None

   @Param
     None

   @Returns
     None

   @Comment
     

  @Example
     <code>
     uint16_t dutycycle;

     PWM2_Initialize();
     PWM2_LoadDutyValue(dutycycle);
     </code>
  */
 void PWM2_Initialize(void);

 /**
   @Summary
     Loads 16-bit duty cycle.

   @Description
     This routine loads the 16 bit duty cycle value.

   @Preconditions
     PWM2_Initialize() function should have been called 
         before calling this function.

   @Param
     Pass 16bit duty cycle value.

   @Returns
     None

   @Example
     <code>
     uint16_t dutycycle;

     PWM2_Initialize();
     PWM2_LoadDutyValue(dutycycle);
     </code>
 */
 void PWM2_LoadDutyValue(uint16_t dutyValue);
 
 #ifdef __cplusplus  // Provide C++ Compatibility

     }

 #endif

 #endif	//PWM2_H
 /**
  End of File
 */