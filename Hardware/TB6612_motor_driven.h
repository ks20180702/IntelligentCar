#ifndef __TB6612_MOTOR_DRIVEN_H
#define __TB6612_MOTOR_DRIVEN_H
#include "stm32f10x.h" 

/*
	���������ʼ��
		PWMA	PA0		TIM2CH1
		AO1	    PB13	��ͨGPIO
		AO2	    PB12		
					
		PWMB	PA1		TIM2CH2
		BO1		PB1		
		BO2		PB0		
		
		PA2->ǰ�������ѡ���

*/
#define SPEED_MAX 40 //�ٶ����ֵ
#define SPEED_MIN 0 //�ٶ���Сֵ
#define SPEED_CHANGE_NUM 10 //�ٶȱ仯ֵ



/*
	�����ʼ����Ĭ�϶�ʱ��ʧ�ܣ�Ԥ��ƵΪ7200-1����װֵΪ100��Ƶ��Ϊ100hz=10ms
*/
void TB6612_Init(void);

/*
	С������ť��ʼ��������PA2�ⲿ�жϣ����ж��иı䷽��
*/
void Change_ward_button_Init(void);

/*
	����ٶ����ã�ռ�ձȶ����ٶȣ�max=40��min=0;
*/
void TB6612_set_speed(uint8_t motorSpeed);

/*
	�������(��ʼ���PWM��)��ʹ�ܶ�ʱ��
*/
void TB6612_start(uint8_t motorSpeed);

/*
	�ı�С��������ǰ
*/
void Change_forward(void);

/*
	�ı�С���������
*/
void Change_backward(void);

/*
	����һ��С���ٶ� ��max=Ԥ��
*/
void Speed_increase(void);
	
/*
	��Сһ��С���ٶ� , min=Ԥ��
*/
void Speed_decrease(void);

#endif
