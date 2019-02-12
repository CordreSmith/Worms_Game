#include <gui/main_screen/MainPresenter.hpp>
#include <gui/main_screen/MainView.hpp>

MainPresenter::MainPresenter(MainView& v)
    : view(v)
{
	maxVoltage = 2400;
	maxCurrent = 1000;
	minVoltage = 102;
	minCurrent = 0;

	deltaVoltage = 6;
	deltaCurrent = 6;
	
}

void MainPresenter::activate()
{
	view.setCurrent(model->getdCurrent());
	view.setVoltage(model->getdVoltage());
	view.setMultiplier(model->getMultiplier());

	view.setLimitEffect(true, false, 1);
	view.setLimitEffect(true, false, 2);

}

void MainPresenter::deactivate()
{

}

void MainPresenter::startupComplete()
{
	model->setState(2);
}

void MainPresenter::voltageUpPressed()
{
	int desVoltage = model->getdVoltage();
	int Multiplier = model->getMultiplier();
	
	if (desVoltage < maxVoltage)
	{
		desVoltage = desVoltage + deltaVoltage*Multiplier;	

		if (desVoltage >= maxVoltage)
		{
			desVoltage = maxVoltage;
			view.setLimitEffect(false, true, 1);
		}
		else if (desVoltage == minVoltage + deltaVoltage*Multiplier)
		{
			view.setLimitEffect(true, true, 1);
		}

		view.setVoltage(desVoltage);
		model->setdVoltage(desVoltage);
	}
}

void MainPresenter::voltageDownPressed()
{
	int desVoltage = model->getdVoltage();
	int Multiplier = model->getMultiplier();
	

	if (desVoltage > minVoltage)
	{
		desVoltage = desVoltage - deltaVoltage*Multiplier;

		if (desVoltage <= minVoltage)
		{
			desVoltage = minVoltage;
			view.setLimitEffect(true, false, 1);
		}
		else if (desVoltage == maxVoltage - deltaVoltage*Multiplier)
		{
			view.setLimitEffect(true, true, 1);
		}

		view.setVoltage(desVoltage);
		model->setdVoltage(desVoltage);
	}
}

void MainPresenter::currentUpPressed()
{
	int desCurrent = model->getdCurrent();
	int Multiplier = model->getMultiplier();

	if (desCurrent < maxCurrent)
	{
		desCurrent = desCurrent + deltaCurrent*Multiplier;

		if (desCurrent >= maxCurrent)
		{
			view.setLimitEffect(false, true, 2);
			desCurrent = maxCurrent;
		}
		else if (desCurrent == minCurrent + deltaCurrent*Multiplier)
		{
			view.setLimitEffect(true, true, 2);
		}

		view.setCurrent(desCurrent);
		model->setdCurrent(desCurrent);
	}
}

void MainPresenter::currentDownPressed()
{
	int desCurrent = model->getdCurrent();
	int Multiplier = model->getMultiplier();

	if (desCurrent > minCurrent)
	{
		desCurrent = desCurrent - deltaCurrent*Multiplier;
		
		if (desCurrent <= minCurrent)
		{
			desCurrent = minCurrent;
			view.setLimitEffect(true, false, 2);
		}
		else if (desCurrent == maxCurrent - deltaCurrent*Multiplier)
		{
			view.setLimitEffect(true, true, 2);
		}

		view.setCurrent(desCurrent);
		model->setdCurrent(desCurrent);
	}
}

void MainPresenter::multiplierPressed()
{
	int curMultiplier = model->getMultiplier();

	switch (curMultiplier)
	{
	case 1:
		curMultiplier = 2;
		break;
	case 2:
		curMultiplier = 5;
		break;
	case 5:
		curMultiplier = 10;
		break;
	case 10:
		curMultiplier = 25;
		break;
	case 25:
		curMultiplier = 50;
		break;
	case 50:
		curMultiplier = 100;
		break;
	case 100:
		curMultiplier = 1;
		break;
	default:
		curMultiplier = 1;
		break;
	}

	model->setMultiplier(curMultiplier);
	view.setMultiplier(curMultiplier);

}

void MainPresenter::powerActivated()
{
	int curState = model->getState();
	if (curState == 2)
	{
		model->setState(3);
		view.setPowerButton(2);
	}
	else if (curState == 3)
	{
		model->setState(2);
		view.setPowerButton(1);
	}
}

void MainPresenter::tickHandler()
{
	int Voltage = model->getcVoltage();
	int Current = model->getcCurrent();

	view.handleGraph(Voltage,Current);
	view.setCurVoltage(Voltage);
	view.setCurCurrent(Current);
}
