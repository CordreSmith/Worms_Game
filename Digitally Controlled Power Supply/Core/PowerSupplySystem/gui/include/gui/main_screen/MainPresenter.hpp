#ifndef MAIN_PRESENTER_HPP
#define MAIN_PRESENTER_HPP

#include <mvp/Presenter.hpp>
#include <gui/model/ModelListener.hpp>

using namespace touchgfx;

class MainView;

class MainPresenter : public Presenter, public ModelListener
{
public:
    MainPresenter(MainView& v);
    virtual void activate();
    virtual void deactivate();
    virtual ~MainPresenter() {};

	/* USER Funtions */

	void startupComplete();

	void voltageUpPressed();
	void voltageDownPressed();
	void currentUpPressed();
	void currentDownPressed();
	void multiplierPressed();
	void powerActivated();
	void tickHandler();
			
private:
    MainPresenter();

    MainView& view;
	
	/*  USER VOLTAGE  */

	/*
		Set the Maximum and Minimum Current and Voltage levels.
		Aswell as the voltage and current change.
	*/

	int maxVoltage;
	int minVoltage;
	int maxCurrent;
	int minCurrent;

	int deltaVoltage;
	int deltaCurrent;

};

#endif
