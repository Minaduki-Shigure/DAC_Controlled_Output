#include <msp430f6638.h>
#include "CTS/structure.h"
#include "CTS/CTS_Layer.h"
#include "CTS/CTS_HAL.h"
#include "F5xx_F6xx_Core_Lib/HAL_UCS.h"

int DAC_Init(void)
{
    DAC12_1CTL0 = DAC12IR + DAC12SREF_1 + DAC12AMP_5 + DAC12CALON + DAC12OPS;
    DAC12_1CTL0 |= DAC12ENC;

    //P5DIR=BIT6;
    //P4DIR=BIT4;
    P7OUT&=~BIT6;
    P7SEL|=BIT6;
    P7DIR|=BIT6;
    //输出针脚应该对应的是P7.6

    return 0;
}

void TouchPad_Init(void)
{
    //configure CapTouchs ports
    P3OUT &=~0x7e;
    P3DIR |= 0x7e;
    P6OUT = 0X00;
    P6DIR = 0X00;
}

void Clock_Init(void)
{
    SetVCore(PMMCOREV_3);                       // Set Vcore to accomodate for max. allowed system speed
    UCSCTL3 |= SELREF_2;                        // Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_2;                          // Set ACLK = REFO
    Init_FLL_Settle(20000,630);                 // Set system clock to max (20MHz)
}

/**
 * main.c
 */
void main(void)
{
    unsigned char sliderPosition = 0;

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	DAC_Init();
	TouchPad_Init();
	Clock_Init();

	TI_CAPT_Init_Baseline(&slider);             //Initialize baseline measurement
	TI_CAPT_Update_Baseline(&slider,5);

	while (1)
	{
	    sliderPosition = TI_CAPT_Slider(&slider);   // Get the raw delta counts for element characterization
	    sliderPosition = (sliderPosition + 10)/20;
	    switch (sliderPosition){
	        case 0: DAC12_1DAT &= 0xFFF; break; //Hold
	        case 1: DAC12_1DAT = 0xFFF; break; //PAD5
	        case 2: DAC12_1DAT = 0xBFF; break; //PAD4
	        case 3: DAC12_1DAT = 0x7FF; break; //PAD3
	        case 4: DAC12_1DAT = 0x3FF; break; //PAD2
	        case 5: DAC12_1DAT = 0x000; break; //PAD1
	        default: DAC12_1DAT &= 0xFFF; break; //Hold
	    }
	    //case1-5对应从右往左5个touchpad，0是无触摸，理论来说，Vout=Vref*DAC12_1DAT/0xFFF
	    //但是我并没有找到参考电压到底是多少，看德研的手册可能是3.0V，找时间我去实测一下试试
	}

	 __no_operation();                         // For debugger
}
