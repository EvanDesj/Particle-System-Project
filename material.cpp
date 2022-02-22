// include libraries
#include "colour.h"
#include "material.h"

Material::Material()
{
    // default values
    this->ambient = Colour(0.2f, 0.2f, 0.2f, 1.0f);
    this->diffuse = Colour(0.8f, 0.8f, 0.8f, 1.0f);
    this->specular = Colour(0.0f, 0.0f, 0.0f, 1.0f);
    this->shininess = 0.0f;
}

Material::Material(Colour ambient, Colour diffuse, Colour specular, float shininess)
{
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
}