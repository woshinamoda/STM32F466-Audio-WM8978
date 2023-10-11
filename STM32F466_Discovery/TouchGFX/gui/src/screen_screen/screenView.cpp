#include <gui/screen_screen/screenView.hpp>

screenView::screenView()
{

}

void screenView::setupScreen()
{
    screenViewBase::setupScreen();
}

void screenView::tearDownScreen()
{
    screenViewBase::tearDownScreen();
}




void screenView::Dis_Volum(uint8_t vol)
{
	Unicode::snprintf(Now_VolumBuffer, 100, "%02d", vol);
	Now_Volum.resizeHeightToCurrentText();
	Now_Volum.invalidate();
}



void screenView::Dis_Click_Fre(uint8_t fre)
{
	uint8_t click_fre;
	switch(fre)
	{
		case 1: click_fre = 15; break;
		case 2: click_fre = 30; break;
		case 3: click_fre = 35; break;
		case 4: click_fre = 40; break;
		case 5: click_fre = 45; break;	

		case 6: click_fre = 40; break;	
		case 7: click_fre = 45; break;	
		case 8: click_fre = 50; break;	
		case 9: click_fre = 55; break;	
		case 10: click_fre = 60; break;			
	}
//	Unicode::snprintf(Now_Click_FreTXTBuffer, 100, "%02d", click_fre);
//	Now_Click_FreTXT.resizeHeightToCurrentText();
//	Now_Click_FreTXT.invalidate();

	if(fre <= 5)
	{
	Unicode::snprintf(Now_Click_FreTXTBuffer, 100, "%02d", click_fre);
	Now_Click_FreTXT.resizeHeightToCurrentText();
		
	Now_Click_FreTXT.setVisible(1);
	Now_Sine_FreTXT.setVisible(0);
		
	Now_Click_FreTXT.invalidate();	
	Now_Sine_FreTXT.invalidate();			
	}
	if(fre >= 6)
	{
	Unicode::snprintf(Now_Sine_FreTXTBuffer, 100, "%02d", click_fre);
	Now_Sine_FreTXT.resizeHeightToCurrentText();

	Now_Sine_FreTXT.setVisible(1);
	Now_Click_FreTXT.setVisible(0);
		
	Now_Click_FreTXT.invalidate();	
	Now_Sine_FreTXT.invalidate();				
	}
	
	
	
}






void screenView::handleTickEvent()
{

}