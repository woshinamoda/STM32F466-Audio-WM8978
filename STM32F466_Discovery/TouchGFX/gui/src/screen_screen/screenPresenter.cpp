#include <gui/screen_screen/screenView.hpp>
#include <gui/screen_screen/screenPresenter.hpp>

screenPresenter::screenPresenter(screenView& v)
    : view(v)
{

}

void screenPresenter::activate()
{

}

void screenPresenter::deactivate()
{

}



void screenPresenter::Dis_Volum(uint8_t vol)
{
	view.Dis_Volum(vol);
}


void screenPresenter::Dis_Click_Fre(uint8_t fre)
{
	view.Dis_Click_Fre(fre);
}