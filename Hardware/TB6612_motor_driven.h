#ifndef __TB6612_MOTOR_DRIVEN_H
#define __TB6612_MOTOR_DRIVEN_H
#include "stm32f10x.h" 

/*
	电机驱动初始化
		PWMA	PA0		TIM2CH1
		AO1	    PB13	普通GPIO
		AO2	    PB12		
					
		PWMB	PA1		TIM2CH2
		BO1		PB1		
		BO2		PB0		
		
		PA2->前进或后退选择脚

*/
#define SPEED_MAX 40 //速度最大值
#define SPEED_MIN 0 //速度最小值
#define SPEED_CHANGE_NUM 10 //速度变化值



/*
	电机初始化，默认定时器失能，预分频为7200-1，重装值为100，频率为100hz=10ms
*/
void TB6612_Init(void);

/*
	小车方向按钮初始化，开启PA2外部中断，在中断中改变方向
*/
void Change_ward_button_Init(void);

/*
	电机速度设置，占空比对射速度，max=40，min=0;
*/
void TB6612_set_speed(uint8_t motorSpeed);

/*
	电机启动(开始输出PWM波)，使能定时器
*/
void TB6612_start(uint8_t motorSpeed);

/*
	改变小车方向向前
*/
void Change_forward(void);

/*
	改变小车方向向后
*/
void Change_backward(void);

/*
	增大一次小车速度 ，max=预设
*/
void Speed_increase(void);
	
/*
	减小一次小车速度 , min=预设
*/
void Speed_decrease(void);

#endif
