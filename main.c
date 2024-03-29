/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the XMC MCU: Brownout Detection Example
*                for ModusToolbox. This code example demonstrates brownout detection
*                by reducing power supply voltage from 3.3 V to the defined voltage
*                threshold using brownout detection available in XMC™ MCU devices.
*
* Related Document: See README.md
*
******************************************************************************
*
* Copyright (c) 2015-2020, Infineon Technologies AG
* All rights reserved.
*
* Boost Software License - Version 1.0 - August 17th, 2003
*
* Permission is hereby granted, free of charge, to any person or organization
* obtaining a copy of the software and accompanying documentation covered by
* this license (the "Software") to use, reproduce, display, distribute,
* execute, and transmit the Software, and to prepare derivative works of the
* Software, and to permit third-parties to whom the Software is furnished to
* do so, all subject to the following:
*
* The copyright notices in the Software and this entire statement, including
* the above license grant, this restriction and the following disclaimer,
* must be included in all copies of the Software, in whole or in part, and
* all derivative works of the Software, unless such copies or derivative
* works are solely in the form of machine-executable object code generated by
* a source language processor.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/

#include "cybsp.h"
#include "cy_utils.h"
#include "xmc_scu.h"


/*******************************************************************************
* Macros
*******************************************************************************/

#if (UC_SERIES == XMC11) || (UC_SERIES == XMC12) || (UC_SERIES == XMC13)
#define Brownout_Detection_Interrupt_Handler  SCU_1_IRQHandler
#define INTERRUPT_PRIORITY_NODE_ID        SCU_1_IRQn
#define VOLTAGE_THRESHOLD 1 //3V
#define INTERVAL 0
#endif

#if (UC_SERIES == XMC14)
#define Brownout_Detection_Interrupt_Handler  IRQ1_Handler
#define INTERRUPT_PRIORITY_NODE_ID        IRQ1_IRQn
#define VOLTAGE_THRESHOLD 1 //3V
#define INTERVAL 0
#endif

#if (UC_SERIES == XMC48) || (UC_SERIES == XMC47) || (UC_SERIES == XMC45) || (UC_SERIES == XMC44) || (UC_SERIES == XMC43) || (UC_SERIES == XMC42)
#define VOLTAGE_THRESHOLD 135 //*22.5 mV
#define INTERVAL 1
#endif

/*******************************************************************************
* Function Name: Brownout_Detection_Interrupt_Handler
********************************************************************************
* Summary:
* This is the interrupt handler function for brownout detection for TARGET_KIT_XMC14_BOOT_001 interrupt.
*
* Parameters:
*  none
*
* Return:
*  void
*
*******************************************************************************/
#if (UC_SERIES == XMC11) || (UC_SERIES == XMC12) || (UC_SERIES == XMC13) || (UC_SERIES == XMC14)
void Brownout_Detection_Interrupt_Handler(void)
{
    /* User LED toggle due to brownout detection */
    XMC_GPIO_ToggleOutput(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);
    __BKPT(0);
}
#endif

/*******************************************************************************
* Function Name: NMI_Handler
********************************************************************************
* Summary:
* This is the interrupt handler function for brownout detection for XMC47_RELAX_V1 interrupt.
*
* Parameters:
*  none
*
* Return:
*  void
*
*******************************************************************************/
#if (UC_SERIES == XMC48) || (UC_SERIES == XMC47) || (UC_SERIES == XMC45) || (UC_SERIES == XMC44) || (UC_SERIES == XMC43) || (UC_SERIES == XMC42)
void NMI_Handler(void)
{
    /* toggle LED after brownout detected */
    XMC_GPIO_ToggleOutput(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);
    __BKPT(0);

}
#endif

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It configures the XMC_SCU_POWER_EnableMonitor()
* function which detects brownout. After the brownout detection the user LED will
* be toggled.
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    /* toggle configured LED */
    XMC_GPIO_ToggleOutput(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);

    /* Enable interrupt for brownout */
    #if (UC_SERIES == XMC11) || (UC_SERIES == XMC12) || (UC_SERIES == XMC13) || (UC_SERIES == XMC14)
    XMC_SCU_INTERRUPT_EnableEvent(XMC_SCU_INTERRUPT_EVENT_VDDPI);
    NVIC_EnableIRQ(INTERRUPT_PRIORITY_NODE_ID);
    #endif

    #if (UC_SERIES == XMC48) || (UC_SERIES == XMC47) || (UC_SERIES == XMC45) || (UC_SERIES == XMC44) || (UC_SERIES == XMC43) || (UC_SERIES == XMC42)
    XMC_SCU_TRAP_Enable(XMC_SCU_TRAP_BROWNOUT);
    #endif

    /* set voltage threshold for the supply voltage and the interval value to define the frequency of monitoring of the actual supply voltage */
    XMC_SCU_POWER_EnableMonitor(VOLTAGE_THRESHOLD, INTERVAL);

    /* Infinite loop */
    while(1U)
    {

    }
}

/* [] END OF FILE */
