#include "Move_contral.h"
#include "TB6612_motor_driven.h"
#include "string.h"

uint8_t Parse_order(char *recvArr,uint8_t recvNum)
{
	if(strcmp(recvArr,"up")==0)
	{
		Speed_increase();
		return 1;
	}
	else if(strcmp(recvArr,"down")==0)
	{
		Speed_decrease();
		return 1;
	}
//	else if(strcmp(recvArr,"right")==0)
//	{
//		Speed_increase();
//	}
//	else if(strcmp(recvArr,"left")==0)
//	{
//		Speed_increase();
//	}
	
	return 0;
}
