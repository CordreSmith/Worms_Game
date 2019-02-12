#ifndef PPS_CONFIG_H
#define PPS_CONFIG_H

/* Includes */

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_spi.h"
#include "stm32f7xx_hal_adc.h"
#include "stm32f7xx_hal_tim.h"

/* Defines */

#define RelayControl_Pin GPIO_PIN_8
#define RelayControl_GPIO_Port GPIOB
#define SPI_SCK_Pin GPIO_PIN_1
#define SPI_SCK_GPIO_Port GPIOI
#define DAC1_CS_Pin GPIO_PIN_7
#define DAC1_CS_GPIO_Port GPIOC
#define DAC2_CS_Pin GPIO_PIN_6
#define DAC2_CS_GPIO_Port GPIOC
#define ADC2_Pin GPIO_PIN_10
#define ADC2_GPIO_Port GPIOF
#define ADC1_Pin GPIO_PIN_0
#define ADC1_GPIO_Port GPIOA
#define SPI_MOSI_Pin GPIO_PIN_15
#define SPI_MOSI_GPIO_Port GPIOB

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

void MX_GPIO_Init(void);
void MX_ADC1_Init(void);
void MX_ADC3_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);

void HAL_MspInit(void);
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm);

#endif /* PPS_CONFIG_H */
