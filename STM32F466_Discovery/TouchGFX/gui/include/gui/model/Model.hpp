#ifndef MODEL_HPP
#define MODEL_HPP

#include "main.h"
extern uint8_t volum;
extern uint8_t play_cnt;


class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
