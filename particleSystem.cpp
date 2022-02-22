// Include OpenGL/GLUT Libraries
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif
// If windows, include <windows.h> to get the API functions
#ifdef _WIN32
#include <windows.h>
#endif

// Include required libraries
#include "particle.h"
#include "particleSystem.h"
#include "./shapes/cannon.h"
#include "./shapes/shape.h"
#include "./utils/colour.h"
#include "./utils/material.h"
#include "./utils/vector3.h"
#include <list>
#include <iostream>
#include <cmath>

ParticleSystem::ParticleSystem(Vector3 origin, Vector3 gravity)
{
    this->origin = origin;
    this->gravity = gravity;
    this->exploded = std::list<Particle>();
    this->particles = std::list<Particle>();
};

ParticleSystem::~ParticleSystem()
{
    this->particles.clear();
    this->exploded.clear();
};

Particle ParticleSystem::generateParticle(Vector3 origin, State state, bool exploding)
{
    float x = (rand() % 200 - 100) / 100.0;
    float z = (rand() % 200 - 100) / 100.0;
    x += origin.x;
    z += origin.z;
    // pick vertical modifier
    float y = (float)(rand() % 20 + 5);
    if (exploding)
    {
        y = y / 2;
    }

    // Pick 3 random floating point numbers between -1 and 1
    float xAngle = (float)(rand() % 360 + 1);
    float yAngle = (float)(rand() % 360 + 1);
    float zAngle = (float)(rand() % 360 + 1);

    // Pick random speed between 0.1 and 1
    float speed = (float)(((rand() % 5) / 10.0) + 0.5);

    // Pick random size between 1 and 3
    float size = (float)(((rand() % 10) / 10.0) + 0.25);

    // Make material colours
    Colour ambient, diffuse, specular;
    if (!exploding)
    {
        ambient = Colour();
        diffuse = Colour();
        specular = Colour();
    }
    else
    {
        float base_r = (float)(rand() % 70) / 255.0;
        float base_g = (float)(rand() % 70) / 255.0;
        float base_b = (float)(rand() % 70) / 255.0;
        float delta_dark = (float)((int)(rand() % 4) + 5) / 100.0;
        float delta_light = (float)((int)(rand() % 4) + 5) / 100.0;
        ambient = Colour(base_r, base_g, base_b, 1.0f);
        diffuse = Colour(base_r - delta_dark, base_g - delta_dark, base_b - delta_dark, 1.0f);
        specular = Colour(base_r + delta_light, base_g + delta_light, base_b + delta_light, 1.0f);
    }

    // Make material
    Material material = Material(ambient, diffuse, specular, (float)rand() / RAND_MAX);

    float age = (float)(rand() % 5 + 60);

    // Add new particle to the list at the origin
    return Particle(
        origin,
        Vector3(x, y, z),
        Vector3(xAngle, yAngle, zAngle),
        speed,
        size,
        Colour(),
        material,
        age,
        state);
};

void ParticleSystem::addParticle(Particle particle)
{
    this->particles.push_back(particle);
};

void ParticleSystem::addExploded(Particle particle)
{
    this->exploded.push_back(particle);
};

void ParticleSystem::draw(Shape *objects, int numObjects, bool animation)
{
    for (std::list<Particle>::iterator it = this->particles.begin(); it != this->particles.end(); ++it)
    {
        switch (it->state)
        {
        case State::ALIVE:
            it->draw();
            // Check for collisions with objects
            for (int i = 0; i < numObjects; i++)
            {
                if (it->collision(objects[i].center, objects[i].size))
                {
                    it->state = State::BOUNCE;
                }
            }
            break;
        case State::EXPLODED:
            it->draw();
            // generate 5 more particles in position of explosion
            for (int i = 0; i < 5; i++)
            {
                this->addExploded(this->generateParticle(
                    it->position,
                    State::ALIVE, true));
            }
            it->state = State::ALIVE;
            break;
        case State::DEAD:
            it = this->particles.erase(it);
            if (animation)
            {
                this->addParticle(
                    this->generateParticle(this->origin, State::ALIVE, false));
            }
        }
    }

    for (std::list<Particle>::iterator it = this->exploded.begin(); it != this->exploded.end(); ++it)
    {
        switch (it->state)
        {
        case State::ALIVE:
            it->draw();
            // Check for collisions with objects
            for (int i = 0; i < numObjects; i++)
            {
                if (it->collision(objects[i].center, objects[i].size))
                {
                    it->state = State::DEAD;
                }
            }
            break;
        case State::EXPLODED:
            it->draw();
            it->state = State::DEAD;
            break;
        case State::DEAD:
            it = this->exploded.erase(it);
        }
    }
};

//std::list<ParticleSystem>
void ParticleSystem::update(bool friction)
{
    // Iterate through all particles
    for (std::list<Particle>::iterator p1 = this->particles.begin(); p1 != this->particles.end(); ++p1)
    {
        p1->update(this->gravity, 0.1f, friction);
        std::list<Particle>::iterator p2 = p1;
        std::advance(p2, 1);
        for (p2; p2 != this->particles.end(); ++p2)
        {
            if (
                (p1->immunity <= 0 && p2->immunity <= 0) &&
                p1->collision(
                    p2->position,
                    Vector3(p2->size, p2->size, p2->size)))
            {
                // draw a vector from p1 to p24
                Vector3 v = p2->position - p1->position;
                // update p1's direction to reflect the collision
                p1->direction.x = v.x * 5.0f;
                p1->direction.z = v.z * 5.0f;
                // update p2's direction to reflect the collision
                v = v * -1;
                p2->direction.x = v.x * 5.0f;
                p2->direction.z = v.z * 5.0f;
            }
        }
    }
    for (std::list<Particle>::iterator p1 = this->exploded.begin(); p1 != this->exploded.end(); ++p1)
    {
        p1->update(this->gravity, 0.1f, friction);
    }
};

void ParticleSystem::setDirection(Cannon cannon)
{
    // std::cout << cannon.rotation.x << ", " << cannon.rotation.y << ", " << cannon.rotation.z << std::endl;
    Vector3 up = Vector3(0.0f, 8.0f, 0.0f);
    up = Vector3(
        up.x,
        up.y * cos(cannon.rotation.x * (M_PI / 180.0f)) - up.z * sin(cannon.rotation.x * (M_PI / 180.0f)),
        up.y * sin(cannon.rotation.x * (M_PI / 180.0f)) + up.z * cos(cannon.rotation.x * (M_PI / 180.0f)));
    up = Vector3(
        up.x * cos(cannon.rotation.z * (M_PI / 180.0f)) - up.y * sin(cannon.rotation.z * (M_PI / 180.0f)),
        up.x * sin(cannon.rotation.z * (M_PI / 180.0f)) + up.y * cos(cannon.rotation.z * (M_PI / 180.0f)),
        up.z);
    up = Vector3(
        up.x * cos(cannon.rotation.y * (M_PI / 180.0f)) + up.z * sin(cannon.rotation.y * (M_PI / 180.0f)),
        up.y,
        up.z * cos(cannon.rotation.y * (M_PI / 180.0f)) - up.x * sin(cannon.rotation.y * (M_PI / 180.0f)));
    // std::cout << up.x << ", " << up.y << ", " << up.z << std::endl;
    this->origin = up;
};