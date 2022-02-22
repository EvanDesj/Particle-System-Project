#ifndef COLOUR_H
#define COLOUR_H

class Colour
{
public:
    float r, g, b, a;
    Colour(float r, float g, float b, float a);
    Colour();
    float* getColour();
};

#endif