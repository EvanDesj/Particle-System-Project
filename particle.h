#ifndef PARTICLE_H
#define PARTICLE_H

// Include required libraries
#include "./utils/colour.h"
#include "./utils/material.h"
#include "./utils/vector3.h"

enum State
{
    ALIVE,
    BOUNCE,
    EXPLODED,
    DEAD
};

class Particle
{
public:
    Vector3 position;
    Vector3 direction;
    Vector3 rotationAngle;
    float speed;
    float size;
    Colour colour;
    Material material;
    float age;
    float immunity;
    State state;
    Particle(Vector3 position, Vector3 direction, Vector3 rotationAngle, float speed, float size, Colour colour,
             Material material, float age, State state);
    void draw();
    void update(Vector3 gravity, float deltaTime, bool friction);
    bool collision(Vector3 center, Vector3 size);
    float distance(Vector3 other);
};
#endif