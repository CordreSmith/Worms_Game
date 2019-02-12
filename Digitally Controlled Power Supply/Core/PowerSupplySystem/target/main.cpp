/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.8.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * This file is free software and is provided for example purposes. You may
 * use, copy, and modify within the terms and conditions of the license
 * agreement.
 *
 * This is licensed software for evaluation use, any use must strictly comply
 * with the evaluation license agreement provided with delivery of the
 * TouchGFX software.
 *
 * The evaluation license agreement can be seen on www.touchgfx.com
 *
 * @section Disclaimer
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
 * no obligation to support this software. Draupner Graphics A/S is providing
 * the software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Draupner Graphics A/S can not be held liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this software.
 *
 *****************************************************************************/
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>

using namespace touchgfx;

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* PSS Changes */

#include "main.h"

/* Controller Includes. */
#ifdef __cplusplus
extern "C"
{
#endif
	#include "PPS_Config.h"
	#include "Controller.h"

#ifdef __cplusplus
}
#endif
/***********************/

/**
 * Define the FreeRTOS task priorities and stack sizes
 */
#define configGUI_TASK_PRIORITY                 ( tskIDLE_PRIORITY + 2 )
#define configCONTROLLER_TASK_PRIORITY          ( tskIDLE_PRIORITY + 3 )

#define configGUI_TASK_STK_SIZE                 ( 512 )
#define configCONTROLLER_TASK_STK_SIZE          ( 512 )

#define CANVAS_BUFFER_SIZE (3600)


/**/

static void GUITask(void* params)
{
    touchgfx::HAL::getInstance()->taskEntry();
}


int main(void)
{
    hw_init();
    touchgfx_init();

		/* PSS Changes */
	
		MX_GPIO_Init();
		MX_ADC1_Init();
		MX_ADC3_Init();
		MX_TIM2_Init();
		MX_TIM3_Init();
		Controller_Initialize();

    static uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
    CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);

    xTaskCreate(GUITask, (TASKCREATE_NAME_TYPE)"GUITask",
                configGUI_TASK_STK_SIZE,
                NULL,
                configGUI_TASK_PRIORITY,
                NULL);
		
		/* PSS Changes */
		xTaskCreate(Controller_Task, (TASKCREATE_NAME_TYPE)"Controller_Task",
                configCONTROLLER_TASK_STK_SIZE,
                NULL,
                configCONTROLLER_TASK_PRIORITY,
                NULL);	
								
		/***********************/
			
    vTaskStartScheduler();

    for (;;);

}

