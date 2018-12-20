# DAC_Controlled_Output
This is a program used to control the output voltage by DAC on MSP430F6638.  
_The program is not finished yet. Feel free to tell me your idea please._
## Definition of the pins
The Output pin of the DAC is defined as P7.6. 
## Output voltage.
The output voltage is based on the following formula.  

> Vout = Vref * (DAC12_1DAT / 0xFFF)  

The reference voltage is set as __(unknown)__ and can be modified.  
The output voltage is controlled by the TouchPad buttons on the device.  
The output voltage of each TouchPad button can be modified in the file according to the formula of `Vout` above.
