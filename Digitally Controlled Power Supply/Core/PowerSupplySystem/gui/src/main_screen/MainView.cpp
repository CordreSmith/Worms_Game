#include <gui/main_screen/MainView.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

MainView::~MainView()
{

}

void MainView::setupScreen()
{

	tickAmount = 5;
	tickNumber = 0;
	timeStamp = 0;

	backgroundBox.setXY(0, 0);
	backgroundBox.setWidth(480);
	backgroundBox.setHeight(272);
	backgroundBox.setColor(touchgfx::Color::getColorFrom24BitRGB(70,70,70));

	imgVoltageGraph.setXY(90, 90);
	imgVoltageGraph.setBitmap(Bitmap(BITMAP_GRAPHIMAGE_ID));

	imgCurrentGraph.setXY(270, 90);
	imgCurrentGraph.setBitmap(Bitmap(BITMAP_GRAPHIMAGE_ID));

	Voltage.setPosition(0, 0, 80, 150);

	btnVoltageDown.setPosition(10, 90, 60, 60);
	btnVoltageDown.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_ICONS_DOWN_ARROW_48_ID), Bitmap(BITMAP_BLUE_ICONS_DOWN_ARROW_48_ID));
	btnVoltageDown.setIconXY(7, 17);
	Voltage.add(btnVoltageDown);

	btnVoltageUp.setPosition(10, 30, 60, 60);
	btnVoltageUp.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_ICONS_UP_ARROW_48_ID), Bitmap(BITMAP_BLUE_ICONS_UP_ARROW_48_ID));
	btnVoltageUp.setIconXY(7, 17);
	Voltage.add(btnVoltageUp);

	txtDesVol.setPosition(0, 3, 80, 30);
	txtDesVol.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	txtDesVol.setLinespacing(0);
	txtDesVol.setTypedText(TypedText(T_VOLTAGETEXT));
	DesVolTextBuffer[0] = 0;
	txtDesVol.setWildcard(DesVolTextBuffer);
	Voltage.add(txtDesVol);


	Current.setPosition(400, 0, 80, 155);

	btnCurrentUp.setPosition(10, 35, 60, 60);
	btnCurrentUp.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_ICONS_UP_ARROW_48_ID), Bitmap(BITMAP_BLUE_ICONS_UP_ARROW_48_ID));
	btnCurrentUp.setIconXY(7, 17);
	Current.add(btnCurrentUp);

	btnCurrentDown.setPosition(10, 95, 60, 60);
	btnCurrentDown.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_ICONS_DOWN_ARROW_48_ID), Bitmap(BITMAP_BLUE_ICONS_DOWN_ARROW_48_ID));
	btnCurrentDown.setIconXY(7, 17);
	Current.add(btnCurrentDown);

	txtDesCur.setPosition(0, 1, 80, 34);
	txtDesCur.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	txtDesCur.setLinespacing(0);
	txtDesCur.setTypedText(TypedText(T_CURRENTTEXT));
	DesCurTextBuffer[0] = 0;
	txtDesCur.setWildcard(DesCurTextBuffer);
	Current.add(txtDesCur);


	btnMultiplier.setPosition(291, 0, 60, 60);
	btnMultiplier.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
	btnMultiplier.setLabelText(TypedText(T_MULTIPLIERX));
	btnMultiplier.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
	btnMultiplier.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));

	txtMultiplier.setPosition(80, 17, 211, 27);
	txtMultiplier.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	txtMultiplier.setLinespacing(0);
	txtMultiplier.setTypedText(TypedText(T_MULTIPLIERTEXT));
	txtMultiplierBuffer[0] = 0;
	txtMultiplier.setWildcard(txtMultiplierBuffer);

	btnPower.setPosition(410, 203, 60, 60);
	btnPower.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_ICONS_POWER_48_ID), Bitmap(BITMAP_BLUE_ICONS_POWER_48_ID));
	btnPower.setIconXY(7, 7);

	imgPowerIndicator.setXY(420, 160);
	imgPowerIndicator.setBitmap(Bitmap(BITMAP_GREEN_BOLT_ID));
	
	grphVol.setXY(100, 90);
	grphVol.setup(100, 100, Color::getColorFrom24BitRGB(200, 0, 0));
	grphVol.setRange(0, 50, 0, 250);
	grphVol.setLineWidth(4);

	grphCur.setXY(280, 90);
	grphCur.setup(100, 100, Color::getColorFrom24BitRGB(200, 0, 0));
	grphCur.setRange(0, 50, 0, 100);
	grphCur.setLineWidth(4);

	txtCurVol.setPosition(115, 205, 80, 34);
	txtCurVol.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	txtCurVol.setLinespacing(0);
	txtCurVol.setTypedText(TypedText(T_VOLTAGETEXT));
	CurVolTextBuffer[0] = 0;
	txtCurVol.setWildcard(CurVolTextBuffer);

	txtCurCur.setPosition(295, 205, 80, 50);
	txtCurCur.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	txtCurCur.setLinespacing(0);
	txtCurCur.setTypedText(TypedText(T_CURRENTTEXT));
	CurCurTextBuffer[0] = 0;
	txtCurCur.setWildcard(CurCurTextBuffer);
	
	/* 
		ACTIONS 
	*/ 

	btnCurrentDown.setAction(buttonClickedCallback);
	btnCurrentUp.setAction(buttonClickedCallback);
	btnMultiplier.setAction(buttonClickedCallback);
	btnVoltageDown.setAction(buttonClickedCallback);
	btnVoltageUp.setAction(buttonClickedCallback);
	btnPower.setAction(buttonClickedCallback);

	/* 
		Adding items to screen, with bottom layers going first
	*/

	//add(backgroundImage);

	add(backgroundBox);
	add(imgVoltageGraph);
	add(imgCurrentGraph);

	add(imgPowerIndicator);
	add(Voltage);
	add(Current);
	add(btnMultiplier);
	add(txtMultiplier);
	add(btnPower);
	add(grphVol);
	add(grphCur);
	add(txtCurVol);
	add(txtCurCur);

	presenter->startupComplete();
}

void MainView::tearDownScreen()
{

}

void MainView::ButtonClicked(const AbstractButton & source)
{

	if (&btnVoltageUp == &source)
	{
		presenter->voltageUpPressed();
	}
	else if (&btnVoltageDown == &source)
	{
		presenter->voltageDownPressed();
	}
	else if (&btnCurrentUp == &source)
	{
		presenter->currentUpPressed();
	}
	else if (&btnCurrentDown == &source)
	{
		presenter->currentDownPressed();
	}
	else if (&btnMultiplier == &source)
	{
		presenter->multiplierPressed();
	}
	else if (&btnPower == &source)
	{
		presenter->powerActivated();
	}
}

void MainView::setVoltage(int VoltageValue)
{
	Unicode::snprintf(DesVolTextBuffer, 6, "%d,%d%d", VoltageValue / 100, (VoltageValue / 10) % 10, VoltageValue % 10);
	txtDesVol.invalidate();
}

void MainView::setCurVoltage(int VoltageValue)
{
	Unicode::snprintf(CurVolTextBuffer, 6, "%d,%d%d", VoltageValue / 100, (VoltageValue / 10) % 10, VoltageValue % 10);
	txtCurVol.invalidate();
}

void MainView::setCurrent(int CurrentValue)
{

	Unicode::snprintf(DesCurTextBuffer, 6, "%d,%d%d%d", CurrentValue / 1000, (CurrentValue / 100) % 10, (CurrentValue / 10) % 10, CurrentValue % 10);
	txtDesCur.invalidate();
}

void MainView::setCurCurrent(int CurrentValue)
{
	Unicode::snprintf(CurCurTextBuffer, 6, "%d,%d%d%d", CurrentValue / 1000, (CurrentValue / 100) % 10, (CurrentValue / 10) % 10, CurrentValue % 10);
	txtCurCur.invalidate();
}

void MainView::setMultiplier(int MultiplierValue)
{
	Unicode::snprintf(txtMultiplierBuffer, 4, "%d", MultiplierValue);
	txtMultiplier.invalidate();
}

void MainView::setPowerButton(int State)
{
	if (State == 2)
	{
		btnPower.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_ICONS_POWER_48_ID), Bitmap(BITMAP_BLUE_ICONS_POWER_48_ID));
		imgPowerIndicator.setBitmap(Bitmap(BITMAP_RED_BOLT_ID));
		imgPowerIndicator.invalidate();
	}
	else if (State == 1)
	{
		btnPower.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_ICONS_POWER_48_ID), Bitmap(BITMAP_BLUE_ICONS_POWER_48_ID));
		imgPowerIndicator.setBitmap(Bitmap(BITMAP_GREEN_BOLT_ID));
		imgPowerIndicator.invalidate();
	}
}

void MainView::setLimitEffect(bool belowUpper, bool aboveLower, int source)
{

	if (source == 1)
	{
		btnVoltageUp.setTouchable(belowUpper);
		btnVoltageDown.setTouchable(aboveLower);
		if ((belowUpper == false) && (aboveLower == true))
		{
			btnVoltageUp.setBitmaps(Bitmap(BITMAP_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_UP_ARROW_48_ID), Bitmap(BITMAP_UP_ARROW_48_ID));
		}
		else if ((belowUpper == true) && (aboveLower == false))
		{
			btnVoltageDown.setBitmaps(Bitmap(BITMAP_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_DOWN_ARROW_48_ID), Bitmap(BITMAP_DOWN_ARROW_48_ID));
		}
		else
		{
			btnVoltageUp.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_ICONS_UP_ARROW_48_ID), Bitmap(BITMAP_BLUE_ICONS_UP_ARROW_48_ID));
			btnVoltageDown.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_ICONS_DOWN_ARROW_48_ID), Bitmap(BITMAP_BLUE_ICONS_DOWN_ARROW_48_ID));
		}
	}
	else if (source == 2)
	{
		btnCurrentUp.setTouchable(belowUpper);
		btnCurrentDown.setTouchable(aboveLower);
		if ((belowUpper == false) && (aboveLower == true))
		{
			btnCurrentUp.setBitmaps(Bitmap(BITMAP_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_UP_ARROW_48_ID), Bitmap(BITMAP_UP_ARROW_48_ID));
		}
		else if ((belowUpper == true) && (aboveLower == false))
		{
			btnCurrentDown.setBitmaps(Bitmap(BITMAP_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_DOWN_ARROW_48_ID), Bitmap(BITMAP_DOWN_ARROW_48_ID));
		}
		else
		{
			btnCurrentUp.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_ICONS_UP_ARROW_48_ID), Bitmap(BITMAP_BLUE_ICONS_UP_ARROW_48_ID));
			btnCurrentDown.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), Bitmap(BITMAP_BLUE_ICONS_DOWN_ARROW_48_ID), Bitmap(BITMAP_BLUE_ICONS_DOWN_ARROW_48_ID));
		}
	}

	btnVoltageUp.invalidate();
	btnCurrentUp.invalidate();
	btnVoltageDown.invalidate();
	btnCurrentDown.invalidate();
}

void MainView::handleGraph(int Voltage, int Current)
{
	tickNumber++;

	if (tickNumber >= tickAmount)
	{
		tickNumber = 0;
		timeStamp++;

		if (timeStamp > 50)
		{
			timeStamp = 1;
			//grphCur.clear();
			//grphVol.clear();
			//grphVol.invalidate();
			//grphCur.invalidate();
		}
		
		grphVol.deleteValue(timeStamp);
		grphVol.deleteValue(timeStamp + 1);
		grphVol.deleteValue(timeStamp + 2);

		grphCur.deleteValue(timeStamp);
		grphCur.deleteValue(timeStamp + 1);
		grphCur.deleteValue(timeStamp + 2);

		grphVol.addValue(timeStamp, Voltage/10);
		grphCur.addValue(timeStamp, Current/10);
	}
}

