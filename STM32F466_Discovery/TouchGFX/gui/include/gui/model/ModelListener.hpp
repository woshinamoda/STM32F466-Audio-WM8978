#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }
		
	virtual void Dis_Volum(uint8_t vol){};				
	virtual void Dis_Click_Fre(uint8_t fre){};
		
		
		
		
		
		
		
		
		
		
		
		
		
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
