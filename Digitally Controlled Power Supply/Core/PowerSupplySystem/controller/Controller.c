#include "Controller.h"
#include "PPS_Config.h"
#include "string.h"
#include "math.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_adc.h"
#include "stm32f7xx_hal_spi.h"


/*CONFIGURATION*/

int32_t TimeDelay = 20; // Time delay between tasks. Remember each task will be serviced once every Timedelay*4 miliseconds
int8_t CountAmount = 7; // Amount of ADC reading to average, for smoothing purposes

int8_t State = Implementation; // Change to Implementation when not using Pot to test

/***************/

xQueueHandle dVoltage_msg;
xQueueHandle dCurrent_msg;
xQueueHandle pActivated_msg;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;
extern SPI_HandleTypeDef hspi2;

void trace(char * string);
void dec2bin (int x, uint8_t *bin);
int testCurrent (float current); // 1 - Current Viable
																 // 2 - Current Unviable

int desVoltage = 0;
float curVoltage1 = 0;
float curVoltage2 = 0;
int desCurrent = 0;
float curCurrent = 0;
float preCurrent = 0;
float tempVoltage1 = 0;
float tempVoltage2 = 0;
char trace_msg[100];

int ADCReadingCount = 0;

void Controller_Initialize(void)
{
	
dVoltage_msg = xQueueCreate(1, 2);
dCurrent_msg = xQueueCreate(1, 2);
pActivated_msg = xQueueCreate(1, 2);

}

void Controller_Task(void* params)
{
	
	extern	xQueueHandle cVoltage_msg;
	extern	xQueueHandle cCurrent_msg;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	
	htim2.Instance->CCR1 = 15;
	htim3.Instance->CCR1 = 0;
	
	/* Endless loop for*/
	
	for(;;)
	{
		uint16_t msg = 0;
		
		float vol_des;
		float cur_des;
		int curTest;
		uint8_t decimalConvertion[12];
		uint8_t spiControlBits[4] = {0,1,1,0};
		uint8_t spiMessage[16];
			
		/*Check power Status*/
		
		vTaskDelay(TimeDelay);
		
		if (xQueueReceive(pActivated_msg, &msg, 0) == pdTRUE)
		{
			if (msg == PowerActivated)
			{
				sprintf(trace_msg,"Power Activate\n");
				HAL_GPIO_WritePin(RelayControl_GPIO_Port,RelayControl_Pin,GPIO_PIN_SET);
				trace(trace_msg);
			}
			else if (msg == PowerDeactivated)
			{
				sprintf(trace_msg,"Power Deactivate\n");
				HAL_GPIO_WritePin(RelayControl_GPIO_Port,RelayControl_Pin,GPIO_PIN_RESET);
				trace(trace_msg);
			}
		}
		
		/* Check desired voltage */
		vTaskDelay(TimeDelay);
		
		if (xQueueReceive(dVoltage_msg, &msg, 0) == pdTRUE)
		{
			if (desVoltage != msg)
			{
				desVoltage = msg;
				double temp = desVoltage;
				
//				sprintf(trace_msg,"New CCR Value for Voltage= %.0f \n",(temp/3300)*216);
//				trace(trace_msg);
				
				htim2.Instance->CCR1 = (temp/3300)*500;
			}
		}
		
		/* Check desired current */
		vTaskDelay(TimeDelay);
		
		if (xQueueReceive(dCurrent_msg, &msg, 0) == pdTRUE)
		{
			if (desCurrent != msg)
			{
				desCurrent = msg;
				double temp = desCurrent;
				
//				sprintf(trace_msg,"New CCR Value for Voltage= %.0f \n",(temp/3300)*216);
//				trace(trace_msg); 
				
				htim3.Instance->CCR1 = (temp/3300)*500;
			}
		}
		
		/* Check current Voltage & Current */
		vTaskDelay(TimeDelay);
		
		sprintf(trace_msg,"READING ADC \n");
		trace(trace_msg);
		
		HAL_ADC_Start(&hadc1);
		HAL_ADC_Start(&hadc3);
		
		tempVoltage1  += (double)HAL_ADC_GetValue(&hadc1);
		tempVoltage2  += (double)HAL_ADC_GetValue(&hadc3);
		
		HAL_ADC_Stop(&hadc1);
		HAL_ADC_Stop(&hadc3);
		
		ADCReadingCount ++;
		
		sprintf(trace_msg,"CURRENT TEMP VOLTAGE 1 READING %.5f \n",tempVoltage1);
		trace(trace_msg);
		sprintf(trace_msg,"CURRENT TEMP VOLTAGE 2 READING %.5f \n",tempVoltage2);
		trace(trace_msg);
		sprintf(trace_msg,"CURRENT ADC READING COUNT %d \n",ADCReadingCount);
		trace(trace_msg);
		
		if (ADCReadingCount >= CountAmount)
		{
			sprintf(trace_msg,"CALCULATING VOLTAGE\n");
			trace(trace_msg);
			
			curVoltage1 = ((tempVoltage1/CountAmount)/(4096-1))*33;
			curVoltage2 = ((tempVoltage2/CountAmount)/(4096-1))*33;
			
			msg = (curVoltage1*100);
			if (cVoltage_msg)
			{
				xQueueSend(cVoltage_msg, &msg, 0);
			}
			
			sprintf(trace_msg,"CALCULATING CURRENT\n");
			trace(trace_msg);
			
			curCurrent = (curVoltage1-curVoltage2)/0.001;
			curTest = testCurrent(curCurrent);
			if (curTest == 2)
			{
				curCurrent = preCurrent;
			}
			else
			{
				preCurrent = curCurrent;
			}
			
			msg = (curCurrent*100);
			if (cCurrent_msg)
			{
				xQueueSend(cCurrent_msg, &msg, 0);
			}
				
			sprintf(trace_msg,"Voltage Reading 1: %.4f \n",curVoltage1);
			trace(trace_msg);
			sprintf(trace_msg,"Voltage Reading 2: %.4f \n",curVoltage2);
			trace(trace_msg);
//			sprintf(trace_msg,"Current Reading : %.3f \n",curCurrent);
//			trace(trace_msg);
			
			ADCReadingCount = 0;
			tempVoltage1    = 0;
			tempVoltage2    = 0;
		}
	}
}

void trace(char * string){
	
	uint8_t len = strlen(string);
	uint8_t i;
	
	for(i = 0;i<len;i++){
		ITM_SendChar(string[i]);
	}
	
}

void dec2bin (int x, uint8_t *bin){
	
	double y = x;
	for (int i = 11;i>=0;i--)
	{

		if (y/(pow(2,i)) >= 1)
		{
			bin[11-i] = 1;
			y = y - pow(2,i);
		}
		else 
		{
			bin[11-i] = 0;
 		}
	}
}

int testCurrent (float current)
{
	if ((current <0) || (current>(desCurrent/1000)))
	{
		return 2;
	}
	else
	{
		return 1;
	}
}
