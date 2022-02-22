#ifndef SHAPE_H
#define SHAPE_H

// Include required libraries
#include "../utils/colour.h"
#include "../utils/material.h"
#include "../utils/vector3.h"

class Shape
{
public:
    Vector3 center;
    Vector3 size;
    Colour colour;
    Material material;

    Shape(Vector3 center, Vector3 size, Colour colour, Material material);

    void draw();
};

#endif