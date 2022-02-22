#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

// Include required libraries
#include "particle.h"
#include "./shapes/cannon.h"
#include "./shapes/shape.h"
#include "./utils/colour.h"
#include "./utils/material.h"
#include "./utils/vector3.h"
#include <list>

// Particle System
class ParticleSystem
{
public:
    Vector3 origin;
    Vector3 gravity;
    std::list<Particle> particles;
    std::list<Particle> exploded;
    ParticleSystem(Vector3 origin, Vector3 gravity);
    ~ParticleSystem();
    Particle generateParticle(Vector3 origin, State state, bool exploded);
    void addParticle(Particle particle);
    void addExploded(Particle particle);
    void draw(Shape *objects, int numObjects, bool animation);
    //std::list<ParticleSystem>
    void update(bool friction);
    void setDirection(Cannon cannon);
};

#endif