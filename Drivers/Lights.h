#ifndef __LIGHTS_H
#define __LIGHTS_H

#include "main.h"

enum Color
{
    Red,
    Green,
    Blue,
};
extern enum Color lightColor; 

void initLights();
void turnOnLights(enum Color color);
void turnOffLights(enum Color color);


#endif // DEBUG
