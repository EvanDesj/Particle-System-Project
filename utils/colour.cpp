// Include the header file for syntax
#include "colour.h"

// Include required libraries
#include <random>

Colour::Colour(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 1.0f;
};
Colour::Colour()
{
    this->r = (float)(rand() % 255) / 255.0;
    this->g = (float)(rand() % 255) / 255.0;
    this->b = (float)(rand() % 255) / 255.0;
    this->a = (float)rand() / RAND_MAX;
};
float* Colour::getColour()
{
    float* colour = new float[4];
    colour[0] = this->r;
    colour[1] = this->g;
    colour[2] = this->b;
    colour[3] = this->a;
    return colour;
};