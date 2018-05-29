#include "LPC11XX.H"

#define LED1_ON()  (LPC_GPIO2->DATA &= ~(1<<0))
#define LED1_OFF() (LPC_GPIO2->DATA |= (1<<0))

#define LED2_ON()  (LPC_GPIO2->DATA &= ~(1<<1))
#define LED2_OFF() (LPC_GPIO2->DATA |= (1<<1))

#define KEY1_DOWN() ((LPC_GPIO3->DATA & (1<<0)) != (1<<0))
#define KEY2_DOWN() ((LPC_GPIO3->DATA & (1<<1))!=(1<<1))
/********************************************************************************
* FunctionName   : LEDInit()
* Description    : LED��ʼ������
* EntryParameter : None
* ReturnValue    : None
********************************************************************************/

void LEDInit()
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16); // ʹ��IOCONʱ�� 
    LPC_IOCON->R_PIO1_0 &= ~0x07;    
    LPC_IOCON->R_PIO1_0 |= 0x01; //��P1.0������ΪGPIO
    LPC_IOCON->R_PIO1_1 &= ~0x07;   
    LPC_IOCON->R_PIO1_1 |= 0x01; //��P1.1������ΪGPIO
    LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16); // ����IOCONʱ��
    
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);  //ʹ��AHBʱ�ӵ�GPIO��    
    LPC_GPIO2->DIR |= (1<<0); // ��P1.0����Ϊ�������
    LPC_GPIO2->DATA |= (1<<0); // ��P1.0����Ϊ�ߵ�ƽ
    LPC_GPIO2->DIR |= (1<<1); // ��P1.1����Ϊ�������
    LPC_GPIO2->DATA |= (1<<1); // ��P1.1����Ϊ�ߵ�ƽ
}
/********************************************************************************
* FunctionName   : PIOINT1_IRQHandler()
* Description    : P1���жϷ�����
* EntryParameter : None
* ReturnValue    : None
********************************************************************************/

void PIOINT3_IRQHandler()
{
    if((LPC_GPIO3->MIS & (1<<0))==(1<<0)) // �����P1.9������ж�
    {
        LED1_ON();
        while(KEY1_DOWN());
        LED1_OFF();
        LPC_GPIO3->IC = (1<<0);  // ���ж�
    }
    if((LPC_GPIO3->MIS & (1<<1))==(1<<1)) // �����P1.10������ж�
    {
        LED2_ON();
        while(KEY2_DOWN());
        LED2_OFF();
        LPC_GPIO3->IC = (1<<1);  // ���ж�
    }
}
/********************************************************************************
* FunctionName   : main()
* Description    : ������
* EntryParameter : None
* ReturnValue    : None
********************************************************************************/
int main()
{
    //LEDInit(); // ����LED
    
    LPC_IOCON->PIO3_0 &= ~(0x07);//��P1_9����ΪGPIO
		LPC_GPIO3->DIR    &= ~(1<<0);//��P1_9����Ϊ����
    LPC_IOCON->PIO3_1 &= ~(0x07);//��P1_10����ΪGPIO
    LPC_GPIO3->DIR &= ~(1<<1);//��P1_10����Ϊ����
    
    LPC_GPIO3->IE |= (1<<0); // ����P1.9�����ϵ��ж�
    LPC_GPIO3->IE |= (1<<1); // ����P1.10�����ϵ��ж�
    LPC_GPIO3->IS &= ~(1<<0); // ����P1.9����Ϊ���ش���
    LPC_GPIO3->IS &= ~(1<<1); // ����P1.10����Ϊ���ش���
    LPC_GPIO3->IEV &= ~(1<<0); // ����P1.9����Ϊ�½��ش���
    LPC_GPIO3->IEV &= ~(1<<1); // ����P1.10����Ϊ�½��ش���
    NVIC_EnableIRQ(EINT3_IRQn); // ��GPIO1�ж�
    while(1)
    {
       ;
    }
}

