#include "LPC11XX.H"

#define LED1_ON()  (LPC_GPIO2->DATA &= ~(1<<0))
#define LED1_OFF() (LPC_GPIO2->DATA |= (1<<0))

#define LED2_ON()  (LPC_GPIO2->DATA &= ~(1<<1))
#define LED2_OFF() (LPC_GPIO2->DATA |= (1<<1))

#define KEY1_DOWN() ((LPC_GPIO3->DATA & (1<<0)) != (1<<0))
#define KEY2_DOWN() ((LPC_GPIO3->DATA & (1<<1))!=(1<<1))
/********************************************************************************
* FunctionName   : LEDInit()
* Description    : LED初始化函数
* EntryParameter : None
* ReturnValue    : None
********************************************************************************/

void LEDInit()
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16); // 使能IOCON时钟 
    LPC_IOCON->R_PIO1_0 &= ~0x07;    
    LPC_IOCON->R_PIO1_0 |= 0x01; //把P1.0脚设置为GPIO
    LPC_IOCON->R_PIO1_1 &= ~0x07;   
    LPC_IOCON->R_PIO1_1 |= 0x01; //把P1.1脚设置为GPIO
    LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16); // 禁能IOCON时钟
    
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);  //使能AHB时钟到GPIO域。    
    LPC_GPIO2->DIR |= (1<<0); // 把P1.0设置为输出引脚
    LPC_GPIO2->DATA |= (1<<0); // 把P1.0设置为高电平
    LPC_GPIO2->DIR |= (1<<1); // 把P1.1设置为输出引脚
    LPC_GPIO2->DATA |= (1<<1); // 把P1.1设置为高电平
}
/********************************************************************************
* FunctionName   : PIOINT1_IRQHandler()
* Description    : P1口中断服务函数
* EntryParameter : None
* ReturnValue    : None
********************************************************************************/

void PIOINT3_IRQHandler()
{
    if((LPC_GPIO3->MIS & (1<<0))==(1<<0)) // 如果是P1.9引起的中断
    {
        LED1_ON();
        while(KEY1_DOWN());
        LED1_OFF();
        LPC_GPIO3->IC = (1<<0);  // 清中断
    }
    if((LPC_GPIO3->MIS & (1<<1))==(1<<1)) // 如果是P1.10引起的中断
    {
        LED2_ON();
        while(KEY2_DOWN());
        LED2_OFF();
        LPC_GPIO3->IC = (1<<1);  // 清中断
    }
}
/********************************************************************************
* FunctionName   : main()
* Description    : 主函数
* EntryParameter : None
* ReturnValue    : None
********************************************************************************/
int main()
{
    //LEDInit(); // 配置LED
    
    LPC_IOCON->PIO3_0 &= ~(0x07);//将P1_9设置为GPIO
		LPC_GPIO3->DIR    &= ~(1<<0);//将P1_9设置为输入
    LPC_IOCON->PIO3_1 &= ~(0x07);//将P1_10设置为GPIO
    LPC_GPIO3->DIR &= ~(1<<1);//将P1_10设置为输入
    
    LPC_GPIO3->IE |= (1<<0); // 允许P1.9引脚上的中断
    LPC_GPIO3->IE |= (1<<1); // 允许P1.10引脚上的中断
    LPC_GPIO3->IS &= ~(1<<0); // 设置P1.9引脚为边沿触发
    LPC_GPIO3->IS &= ~(1<<1); // 设置P1.10引脚为边沿触发
    LPC_GPIO3->IEV &= ~(1<<0); // 设置P1.9引脚为下降沿触发
    LPC_GPIO3->IEV &= ~(1<<1); // 设置P1.10引脚为下降沿触发
    NVIC_EnableIRQ(EINT3_IRQn); // 打开GPIO1中断
    while(1)
    {
       ;
    }
}

