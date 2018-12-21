#include <msp430f6638.h>
#include "CTS/structure.h"
#include "CTS/CTS_Layer.h"
#include "CTS/CTS_HAL.h"
#include "F5xx_F6xx_Core_Lib/HAL_UCS.h"

#define LED_PORT_DIR    P4DIR
#define LED_PORT_OUT    P4OUT
#define LED1    BIT4
#define LED2    BIT5
#define LED3    BIT6

void Board_ledOn(unsigned char LEDx)
{
    if(LEDx & LED1)LED_PORT_OUT |= BIT4;
    if(LEDx & LED2)LED_PORT_OUT |= BIT5;
    if(LEDx & LED3)LED_PORT_OUT |= BIT6;
}

void Board_ledOff(unsigned char LEDx)
{
    if(LEDx & LED1)LED_PORT_OUT &= ~BIT4;
    if(LEDx & LED2)LED_PORT_OUT &= ~BIT5;
    if(LEDx & LED3)LED_PORT_OUT &= ~BIT6;
}

int DAC_Init(void)
{
    DAC12_1CTL0 = DAC12IR + DAC12SREF_1 + DAC12AMP_5 + DAC12CALON + DAC12OPS;
    DAC12_1CTL0 |= DAC12ENC;

    //P5DIR=BIT6;
    //P4DIR=BIT4;
    P5OUT&=~BIT7;
    P5SEL|=BIT7;
    P5DIR|=BIT7;
    //输出针脚应该对应的是???

    return 0;
}

void TouchPad_Init(void)
{
    //configure CapTouchs ports
    P3OUT &=~0x7e;
    P3DIR |= 0x7e;
    P6OUT = 0X00;
    P6DIR = 0X00;
    P4DIR |= BIT4+BIT5+BIT6;
    P4OUT &= ~(BIT4+BIT5+BIT6);
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
	        case 0: DAC12_1DAT &= 0xFFF; Board_ledOff(LED1 + LED2 + LED3); break; //Hold
	        case 1: DAC12_1DAT = 0xFFF; Board_ledOn(LED3); Board_ledOff(LED2 + LED1); break; //PAD1
	        case 2: DAC12_1DAT = 0xBFF; Board_ledOn(LED2); Board_ledOff(LED1 + LED3); break; //PAD2
	        case 3: DAC12_1DAT = 0x7FF; Board_ledOn(LED1); Board_ledOff(LED3 + LED2); break; //PAD3
	        case 4: DAC12_1DAT = 0x3FF; Board_ledOn(LED1 + LED2 + LED3); break; //PAD4
	        case 5: DAC12_1DAT = 0x000; Board_ledOff(LED1 + LED2 + LED3); break; //PAD5
	        default: DAC12_1DAT &= 0xFFF; break; //Hold
	    }
	    //case1-5对应从左往右5个touchpad，0是无触摸，理论来说，Vout=Vref*DAC12_1DAT/0xFFF
	    //但是我并没有找到参考电压到底是多少，看德研的手册可能是3.0V，找时间我去实测一下试试
	}

	 __no_operation();                         // For debugger
}
