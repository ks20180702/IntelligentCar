#include "TB6612_motor_driven.h" 
#include "Usart.h"

uint8_t SpeedNow;   //当前小车速度(不带方向)

void Change_ward_button_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//开启一个引脚，用于按钮，低电平接通表示按下，改变小车方向
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//开启外部中断，下降沿触发
	EXTI_StructInit(&EXTI_InitStruct);
	EXTI_InitStruct.EXTI_Line=EXTI_Line2;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	//设置NVIC
	NVIC_InitStruct.NVIC_IRQChannel=EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStruct);
}
void TB6612_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	
	//电机方向选择引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//PWM波输出脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//定时器基础设置
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;  
	TIM_TimeBaseInitStruct.TIM_Prescaler=7200-1; //预分频值
	TIM_TimeBaseInitStruct.TIM_Period=100-1;  //72000 000/7200/100 = 100hz = 10ms 
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	//定时器设置，用于输出表(输出PWM)
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High; //极性
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=20; //比较寄存器 ARR
	
	TIM_OC1Init(TIM2,&TIM_OCInitStruct );
	
	TIM_OC2Init(TIM2,&TIM_OCInitStruct );
	
	TIM_Cmd(TIM2,DISABLE);
}

//占空比即为速度，PWM频率为100hz
void TB6612_set_speed(uint8_t motorSpeed)
{
	if(motorSpeed>SPEED_MAX) motorSpeed=SPEED_MAX;
	
	TIM_SetCompare1(TIM2,motorSpeed);
	TIM_SetCompare2(TIM2,motorSpeed);
}

//开始输出PWM波形，使能TIM2
void TB6612_start(uint8_t motorSpeed)
{
	TIM_Cmd(TIM2,ENABLE);
	
	TB6612_set_speed(motorSpeed);
}

//前进和后退这里还需要修改一下
void Change_forward(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_13);  //AO1
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);  //AO2
	
	GPIO_SetBits(GPIOB,GPIO_Pin_1);  //BO1
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);    //BO2
}
void Change_backward(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //AO1
	GPIO_SetBits(GPIOB,GPIO_Pin_12);  //AO2
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);  //BO1
	GPIO_SetBits(GPIOB,GPIO_Pin_0);    //BO2
}
void Speed_increase(void)
{
	SpeedNow+=SPEED_CHANGE_NUM;
	if(SpeedNow>SPEED_MAX) SpeedNow=SPEED_MAX;
	
	TB6612_set_speed(SpeedNow);
}
void Speed_decrease(void)
{
	if(SpeedNow<=SPEED_MIN) SpeedNow=SPEED_MIN;
	else SpeedNow-=SPEED_CHANGE_NUM;
	
	TB6612_set_speed(SpeedNow);
}


void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)!=SET) return;
	
	printf("现在可以改变方向\r\n");
	
	EXTI_ClearITPendingBit(EXTI_Line2);
}
