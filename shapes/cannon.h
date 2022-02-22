#ifndef CANNON_H
#define CANNON_H

// Include required libraries
#include "shape.h"
#include "../utils/colour.h"
#include "../utils/material.h"
#include "../utils/vector3.h"

class Cannon : public Shape
{
public:
    Vector3 rotation;
    Vector3 center;
    Vector3 size;
    Colour colour;
    Material material;

    Cannon(
        Vector3 rotation,
        Vector3 center,
        Vector3 size,
        Colour colour,
        Material material
    );

    void draw();
};

#endif