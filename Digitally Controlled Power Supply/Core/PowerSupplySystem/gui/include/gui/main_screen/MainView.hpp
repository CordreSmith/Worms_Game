#ifndef MAIN_VIEW_HPP
#define MAIN_VIEW_HPP

#include <mvp/View.hpp>
#include <gui/main_screen/MainPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/ButtonWithIcon.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <gui/common/Graph.hpp>

using namespace touchgfx;

class MainView : public View<MainPresenter>
{
public:
	MainView()
		: buttonClickedCallback(this, &MainView::ButtonClicked)
	{
	}
    virtual ~MainView();
    virtual void setupScreen();
    virtual void tearDownScreen();

	void ButtonClicked(const AbstractButton& source);

	void setVoltage(int VoltageValue);
	void setCurVoltage(int VoltageValue);
	void setCurrent(int CurrentValue);
	void setCurCurrent(int CurrentValue);
	void setMultiplier(int MultiplierValue);
	void setPowerButton(int State); // 1 = Deactivated
									// 2 = Activated
	void setLimitEffect(bool belowUpper, bool aboveLower, int source); // 0 = Voltage
																	   // 1 = Current
	void handleGraph(int Voltage, int Current);
	
protected:
	/*Visual Members*/

	Box backgroundBox;
	Image imgVoltageGraph;
	Image imgCurrentGraph;
	Image imgPowerIndicator;
	Container Voltage;
	ButtonWithIcon btnVoltageDown;
	ButtonWithIcon btnVoltageUp;
	TextAreaWithOneWildcard txtDesVol;
	Container Current;
	ButtonWithIcon btnCurrentUp;
	ButtonWithIcon btnCurrentDown;
	ButtonWithIcon btnPower;
	TextAreaWithOneWildcard txtDesCur;
	TextAreaWithOneWildcard txtCurVol;
	TextAreaWithOneWildcard txtCurCur;
	TextAreaWithOneWildcard txtMultiplier;
	ButtonWithLabel btnMultiplier;
	Graph grphVol;
	Graph grphCur;

	/*Wildcard Buffers*/

	static const uint16_t DESVOLTEXT_SIZE = 6;
	touchgfx::Unicode::UnicodeChar DesVolTextBuffer[DESVOLTEXT_SIZE];
	static const uint16_t DESCURTEXT_SIZE = 6;
	touchgfx::Unicode::UnicodeChar DesCurTextBuffer[DESCURTEXT_SIZE];
	static const uint16_t CURCURTEXT_SIZE = 6;
	touchgfx::Unicode::UnicodeChar CurCurTextBuffer[CURCURTEXT_SIZE];
	static const uint16_t CURVOLTEXT_SIZE = 6;
	touchgfx::Unicode::UnicodeChar CurVolTextBuffer[CURVOLTEXT_SIZE];
	static const uint16_t MULTIPLIERTEXT_SIZE = 4;
	touchgfx::Unicode::UnicodeChar txtMultiplierBuffer[MULTIPLIERTEXT_SIZE];

	/* Callbacks */

	Callback<MainView, const AbstractButton&> buttonClickedCallback;

	/* Private Functions */

	int tickNumber;
	int tickAmount; // Change value to set every how many ticks their should be a value plot.
	int timeStamp;
};

#endif
