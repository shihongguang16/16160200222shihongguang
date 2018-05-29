#include "LPC11XX.H"

#define KEY1_DOWN() ((LPC_GPIO3->DATA & (1<<0)) != (1<<0))
#define KEY2_DOWN() ((LPC_GPIO3->DATA & (1<<1))!=(1<<1))

void CT32B1_Init(uint32_t interval)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);
	LPC_IOCON->R_PIO1_2 &= ~(0X07);
	LPC_IOCON->R_PIO1_2 |= 0X03;
  LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);
	LPC_TMR32B1->TCR = 0X02;
	LPC_TMR32B1->PR = 0;
	LPC_TMR32B1->MCR = 0X02<<9;
	LPC_TMR32B1->PWMC = 0X02;
	LPC_TMR32B1->MR1 = 5*interval/10;
	LPC_TMR32B1->MR3 = interval;
	LPC_TMR32B1->TCR = 0x01;
}
void PIOINT3_IRQHandler()
{
	  uint32_t interval;
		if((LPC_GPIO3->MIS & (1<<0))==(1<<0))
    {
//        LPC_TMR32B1->MR1 = 5*interval/10;
        while(KEY1_DOWN());
			if( LPC_TMR32B1->MR1!=interval)
				 LPC_TMR32B1->MR1 +=interval/10;
        LPC_GPIO3->IC = (1<<0);
    }
    if((LPC_GPIO3->MIS & (1<<1))==(1<<1))
    {
//        LPC_TMR32B1->MR1 = 5*interval/10;
        while(KEY2_DOWN());
			if( LPC_TMR32B1->MR1!=0)
        LPC_TMR32B1->MR1 -=interval/10;
       LPC_GPIO3->IC = (1<<1);
    }
}
int main(void)
{
	  CT32B1_Init(SystemCoreClock/1000);
	  LPC_IOCON->PIO3_0 &= ~(0x07);
    LPC_GPIO3->DIR    &= ~(1<<0);
    LPC_IOCON->PIO3_1 &= ~(0x07);
    LPC_GPIO3->DIR &= ~(1<<1);
    LPC_GPIO3->IE |= (1<<0);
    LPC_GPIO3->IE |= (1<<1);
    LPC_GPIO3->IS &= ~(1<<0);
    LPC_GPIO3->IS &= ~(1<<1);
    LPC_GPIO3->IEV &= ~(1<<0);
    LPC_GPIO3->IEV &= ~(1<<1);
    NVIC_EnableIRQ(EINT3_IRQn);
	while(1)
	{
		;
	}	
}	
