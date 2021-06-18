#include "utils.h"
#include "gd32vf103_libopt.h"
#include <stdlib.h>

/* -----------------------------
 Description: Return 1 if button number ch is pressed
 			  Return 0 otherwise
----------------------------- */
int Get_Button(int ch)
{
    ADC_RSQ2(ADC0)=0;
    ADC_RSQ2(ADC0)=ch;
    ADC_CTL1(ADC0)|=ADC_CTL1_ADCON;
    while(!(ADC_STAT(ADC0)&ADC_STAT_EOC));
    uint16_t ret=ADC_RDATA(ADC0)&0xFFFF;
    ADC_STAT(ADC0)&=~ADC_STAT_EOC;
    return(ret>4000?1:0);
}

/* -----------------------------
 Description: Return 1 if button BOOT0 ch is pressed
 			  Return 0 otherwise
----------------------------- */
int Get_BOOT0(void)
{
    return (int)(gpio_input_bit_get(GPIOA, GPIO_PIN_8));
}

/* -----------------------------
 Description: Generate random U16
----------------------------- */
unsigned int Generate_U16(unsigned int lower_bound, unsigned int upper_bound, unsigned int seed) {
    srand(seed);
    unsigned int res = (rand() % (upper_bound - lower_bound)) + lower_bound;
    return res;
}