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
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifndef SIMULATOR
	#include "FreeRTOS.h"
	#include "task.h"
	#include "queue.h"

	extern "C"
	{
		#include "Controller.h"

		xQueueHandle cVoltage_msg;
		xQueueHandle cCurrent_msg;

		extern xQueueHandle dVoltage_msg;
		extern xQueueHandle dCurrent_msg;
		extern xQueueHandle pActivated_msg;
	}
#endif // !SIMULATOR




Model::Model() : modelListener(0)
{

	desiredCurrent = 0;
	desiredVoltage = 102;
	currentCurrent = 0;
	currentVoltage = 0;
	currentMultiplier = 1;
	currentState = 1;
	
#ifndef SIMULATOR
	cVoltage_msg = xQueueCreate(1, 2);
	cCurrent_msg = xQueueCreate(1, 2);
#endif // !SIMULATOR
}

void Model::tick()
{
	/* Update Graph */
	modelListener->tickHandler();
	
	#ifndef SIMULATOR
	/* Check the Q for a new Voltage/Current Value from Controller*/
		uint16_t msg = 0;
		if (xQueueReceive(cVoltage_msg, &msg, 0) == pdTRUE)
		{
			currentVoltage = msg; // Value that will be sent must be int.
		}
		if (xQueueReceive(cCurrent_msg, &msg, 0) == pdTRUE)
		{
			currentCurrent = msg; // Value that will be sent must be int.
		}
	#endif // !SIMULATOR
}

int Model::getState()
{
	return currentState;
}

int Model::getdVoltage()
{
	return desiredVoltage;
}

int Model::getdCurrent()
{
	return desiredCurrent;
}

int Model::getMultiplier()
{
	return currentMultiplier;
}

int Model::getcCurrent()
{
	return currentCurrent;
}

int Model::getcVoltage()
{
	return currentVoltage;
}

void Model::setState(int newState)
{
	currentState = newState;
#ifndef SIMULATOR
	uint8_t msg;

	if (newState == 2)
	{
		msg = PowerDeactivated;
		if (pActivated_msg)
		{
			xQueueSend(pActivated_msg, &msg, 0);
		}
	}
	else if (newState == 3)
	{
		msg = PowerActivated;
		if (pActivated_msg)
		{
			xQueueSend(pActivated_msg, &msg, 0);
		}
	}
#endif // !SIMULATOR

}

void Model::setdVoltage(int newVoltage)
{

	desiredVoltage = newVoltage;
#ifndef SIMULATOR
	uint16_t msg = newVoltage;
	if (dVoltage_msg)
	{
		xQueueSend(dVoltage_msg, &msg, 0);
	}
#endif // !1

}

void Model::setdCurrent(int newCurrent)
{
	desiredCurrent = newCurrent;
#ifndef SIMULATOR
	uint16_t msg = newCurrent;

	if (dCurrent_msg)
	{
		xQueueSend(dCurrent_msg, &msg, 0);
	}
#endif // !SIMULATOR

}

void Model::setMultiplier(int newMultiplier)
{
	currentMultiplier = newMultiplier;
}



