// Include the header file for syntax
#include "./cube.h"
#include "../utils/colour.h"
#include "../utils/material.h"
#include "../utils/vector3.h"

Shape::Shape(Vector3 center, Vector3 size, Colour colour, Material material)
{
    this->center = center;
    this->size = size;
    this->colour = colour;
    this->material = material;
};

void Shape::draw()
{
    // Do nothing
};