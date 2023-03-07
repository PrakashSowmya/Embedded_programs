#include<lpc17xx.h>
void uart_init();
void uart_tx(char data);
unsigned char uart_rx()	;
void uart_enter();
void uart_string(char *str);
void convert(unsigned int convert_value);
void delay();
void delay_short(unsigned int delay);
void pll_init();
#define SCS_Val               0x00000020
#define CLKSRCSEL_Val         0x00000001
#define PLL0CFG_Val           0x00050063
#define CCLKCFG_Val           0x00000003

void delay()
{
unsigned int i,j;
for(i=0;i<1000;i++)
for(j=0;j<1000;j++);
}

int main()
{
unsigned int adc0_value;
LPC_PINCON->PINSEL0=0X00000050;
 LPC_PINCON->PINSEL1=0X00140000;
 pll_init();
 delay(1);
 uart_init();
 while(1)
 {
 uart_enter();
uart_string("channel 0");

LPC_SC->  PCONP|=0X00001000;
LPC_ADC->ADCR=0X01200201;
while(!(LPC_ADC->ADGDR&0X80000000));
adc0_value=((LPC_ADC->ADGDR&0X0000FFF0)>>4);
convert(adc0_value);
delay(10000000);
}
}
void uart_init()
{
LPC_UART0->LCR=0X83;
LPC_UART0->DLL=0XA2;
LPC_UART0->DLM=0X00;
LPC_UART0->LCR=0X03;
}
void uart_tx(char data)
{
LPC_UART0->THR=data;
while(!(LPC_UART0->LSR&0X40));
}
unsigned char uart_rx()
{
while(!(LPC_UART0->LSR&0X01));
return (LPC_UART0->RBR);
}

void uart_string(char *str)
{
   while(*str)
   uart_tx(*str++);

}
void uart_enter()
{
uart_tx(0x0d);
uart_tx(0x0a);
}

void pll_init(void)
{
	LPC_SC->SCS       = SCS_Val;
	if (SCS_Val& (1 << 5)) 
	{             /* If Main Oscillator is enabled      */
	while ((LPC_SC->SCS & (1<<6)) == 0);/* Wait for Oscillator to be ready    */
	}
  LPC_SC->CCLKCFG   = CCLKCFG_Val;
  LPC_SC->CLKSRCSEL = CLKSRCSEL_Val;    /* Select Clock Source for PLL0       */

  LPC_SC->PLL0CFG   = PLL0CFG_Val;      /* configure PLL0                     */
  LPC_SC->PLL0FEED  = 0xAA;
  LPC_SC->PLL0FEED  = 0x55;

  LPC_SC->PLL0CON   = 0x01;             /* PLL0 Enable                        */
  LPC_SC->PLL0FEED  = 0xAA;
  LPC_SC->PLL0FEED  = 0x55;
  while (!(LPC_SC->PLL0STAT & (1<<26)));/* Wait for PLOCK0                    */

  LPC_SC->PLL0CON   = 0x03;             /* PLL0 Enable & Connect              */
  LPC_SC->PLL0FEED  = 0xAA;
  LPC_SC->PLL0FEED  = 0x55;
  while (!(LPC_SC->PLL0STAT & ((1<<25) | (1<<24))));/* Wait for PLLC0_STAT & PLLE0_STAT */
}





void convert(unsigned int convert_value)
{
	unsigned int d1, d2, d3, d4;

	d4 = convert_value % 10;
	convert_value = convert_value / 10;

	d3 = convert_value % 10;
	convert_value = convert_value / 10;

	d2 = convert_value % 10;
	convert_value = convert_value / 10;

	d1 = convert_value % 10;
	convert_value = convert_value / 10;

	uart_tx(d1+0x30);
	uart_tx(d2+0x30);
	uart_tx(d3+0x30);
	uart_tx(d4+0x30);
}

