#include "TB6612_motor_driven.h" 
#include "Usart.h"

uint8_t SpeedNow;   //��ǰС���ٶ�(��������)

void Change_ward_button_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//����һ�����ţ����ڰ�ť���͵�ƽ��ͨ��ʾ���£��ı�С������
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//�����ⲿ�жϣ��½��ش���
	EXTI_StructInit(&EXTI_InitStruct);
	EXTI_InitStruct.EXTI_Line=EXTI_Line2;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	//����NVIC
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
	
	//�������ѡ������
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//PWM�������
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//��ʱ����������
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;  
	TIM_TimeBaseInitStruct.TIM_Prescaler=7200-1; //Ԥ��Ƶֵ
	TIM_TimeBaseInitStruct.TIM_Period=100-1;  //72000 000/7200/100 = 100hz = 10ms 
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	//��ʱ�����ã����������(���PWM)
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High; //����
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=20; //�ȽϼĴ��� ARR
	
	TIM_OC1Init(TIM2,&TIM_OCInitStruct );
	
	TIM_OC2Init(TIM2,&TIM_OCInitStruct );
	
	TIM_Cmd(TIM2,DISABLE);
}

//ռ�ձȼ�Ϊ�ٶȣ�PWMƵ��Ϊ100hz
void TB6612_set_speed(uint8_t motorSpeed)
{
	if(motorSpeed>SPEED_MAX) motorSpeed=SPEED_MAX;
	
	TIM_SetCompare1(TIM2,motorSpeed);
	TIM_SetCompare2(TIM2,motorSpeed);
}

//��ʼ���PWM���Σ�ʹ��TIM2
void TB6612_start(uint8_t motorSpeed)
{
	TIM_Cmd(TIM2,ENABLE);
	
	TB6612_set_speed(motorSpeed);
}

//ǰ���ͺ������ﻹ��Ҫ�޸�һ��
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
	
	printf("���ڿ��Ըı䷽��\r\n");
	
	EXTI_ClearITPendingBit(EXTI_Line2);
}
