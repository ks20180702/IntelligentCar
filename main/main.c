//#define USE_STDPERIPH_DRIVER
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Usart.h"
#include "TB6612_motor_driven.h"
#include "string.h"

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	MY_USART_Inti();
	TB6612_Init();
	Change_ward_button_Init();
	
	
	while(1)
	{
		if((strcmp((char *)RecvArr,"up")==0)||(strcmp((char *)RecvArr,"down")==0)
			||(strcmp((char *)RecvArr,"right")==0)||(strcmp((char *)RecvArr,"left")==0))
		{
			memset(RecvArr,0,RecvNum);
			RecvNum=0;
		}
		Delay_ms(1000);
	}
}

