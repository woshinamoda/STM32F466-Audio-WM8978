#ifndef SCREENVIEW_HPP
#define SCREENVIEW_HPP

#include <gui_generated/screen_screen/screenViewBase.hpp>
#include <gui/screen_screen/screenPresenter.hpp>

class screenView : public screenViewBase
{
public:
    screenView();
    virtual ~screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
			
			
	virtual void Dis_Volum(uint8_t vol);
	virtual void Dis_Click_Fre(uint8_t fre);
			
			
protected:
    virtual void handleTickEvent();
    int number;
		int Fnumber;
};

#endif // SCREENVIEW_HPP
