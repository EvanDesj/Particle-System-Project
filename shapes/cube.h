#ifndef CUBE_H
#define CUBE_H

// Include required libraries
#include "shape.h"
#include "../utils/colour.h"
#include "../utils/material.h"
#include "../utils/vector3.h"

class Cube : public Shape
{
public:
    Vector3 center;
    Vector3 size;
    Colour colour;
    Material material;

    Cube(Vector3 center, Vector3 size, Colour colour, Material material);

    void draw();
};

#endif