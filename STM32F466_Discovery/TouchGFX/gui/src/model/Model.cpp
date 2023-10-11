#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : modelListener(0)
{

}

static uint8_t T_Volum;
static uint8_t T_click_cnt;


void Model::tick()
{
	T_Volum = volum;
	T_click_cnt = play_cnt;
	modelListener->Dis_Volum(T_Volum);
	modelListener->Dis_Click_Fre(T_click_cnt);	
	
	
	
}
























