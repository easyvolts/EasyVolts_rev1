/**
  @Generated MPLAB(c) Code Configurator Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.c

  @Summary:
    This is the mcc.c file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC16F1455
        Driver Version    :  1.02
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

// Configuration bits: selected in the GUI

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits->INTOSC oscillator: I/O function on CLKIN pin
#pragma config WDTE = ON    // Watchdog Timer Enable->WDT enabled
#pragma config PWRTE = OFF    // Power-up Timer Enable->PWRT disabled
#pragma config MCLRE = ON    // MCLR Pin Function Select->MCLR/VPP pin function is MCLR
#pragma config CP = ON    // Flash Program Memory Code Protection->Program memory code protection is enabled
#pragma config BOREN = ON    // Brown-out Reset Enable->Brown-out Reset enabled
#pragma config CLKOUTEN = OFF    // Clock Out Enable->CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin
#pragma config IESO = OFF    // Internal/External Switchover Mode->Internal/External Switchover Mode is disabled
#pragma config FCMEN = OFF    // Fail-Safe Clock Monitor Enable->Fail-Safe Clock Monitor is disabled

// CONFIG2
#pragma config WRT = OFF    // Flash Memory Self-Write Protection->Write protection off
#pragma config CPUDIV = NOCLKDIV    // CPU System Clock Selection Bit->NO CPU system divide
#pragma config USBLSCLK = 48MHz    // USB Low SPeed Clock Selection bit->System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.
#pragma config PLLMULT = 3x    // PLL Multipler Selection Bit->3x Output Frequency Selected
#pragma config PLLEN = ENABLED    // PLL Enable Bit->3x or 4x PLL Enabled
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable->Stack Overflow or Underflow will cause a Reset
#pragma config BORV = LO    // Brown-out Reset Voltage Selection->Brown-out Reset Voltage (Vbor), low trip point selected.
#pragma config LPBOR = OFF    // Low-Power Brown Out Reset->Low-Power BOR is disabled
#pragma config LVP = OFF    // Low-Voltage Programming Enable->High-voltage on MCLR/VPP must be used for programming

#include "mcc.h"

void SYSTEM_Initialize(void)
{

    PIN_MANAGER_Initialize();
    OSCILLATOR_Initialize();
    WDT_Initialize();
    FVR_Initialize();
    PWM2_Initialize();
    ADC_Initialize();
    TMR2_Initialize();
    TMR1_Initialize();
    EUSART_Initialize();
}

void OSCILLATOR_Initialize(void)
{
    // SCS FOSC; SPLLMULT 3xPLL; SPLLEN enabled; IRCF 16MHz_HF; 
    OSCCON = 0xFC;
    // TUN 0; 
    OSCTUNE = 0x00;
    // ACTSRC Full speed USB events; ACTUD enabled; ACTEN enabled; 
    ACTCON = 0x90;
    // Set the secondary oscillator
    
    // Wait for PLL to stabilize
    while(PLLRDY == 0)
    {
    }
}

void WDT_Initialize(void)
{
    // WDTPS 1:256; SWDTEN OFF; 
    WDTCON = 0x06;
}

/**
 End of File
*/
