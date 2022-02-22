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
#include "./shapes/cannon.h"
#include "./shapes/shape.h"
#include "./utils/colour.h"
#include "./utils/material.h"
#include "./utils/vector3.h"
#include <list>
#include <iostream>
#include <cmath>

#define max_life 100

Particle::Particle(
    Vector3 position, Vector3 direction, Vector3 rotationAngle,
    float speed, float size, Colour colour, Material material, float age, State state)
{
    this->position = position;
    this->direction = direction;
    this->rotationAngle = rotationAngle;
    this->speed = speed;
    this->size = size;
    this->colour = colour;
    this->material = material;
    this->age = age;
    this->state = state;
    this->immunity = 5.0f;
};

void Particle::draw()
{
    // print statement to verify drawing
    glPushMatrix();
    // Translate to the position of the particle
    glTranslatef(this->position.x, this->position.y, this->position.z);
    // Rotate the particle
    glRotatef(this->rotationAngle.x, 1.0f, 0.0f, 0.0f);
    glRotatef(this->rotationAngle.y, 0.0f, 1.0f, 0.0f);
    glRotatef(this->rotationAngle.z, 0.0f, 0.0f, 1.0f);
    // Apply lighting from material
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient.getColour());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse.getColour());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular.getColour());
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
    // Colour the particle
    glColor3f(this->colour.r, this->colour.g, this->colour.b);
    //glutSolidSphere(this->size, 10, 10);
    glutSolidCube(this->size);
    glPopMatrix();
};

void Particle::update(Vector3 gravity, float deltaTime, bool friction)
{
    Vector3 next_pos = this->position + (this->direction * (this->speed * deltaTime));
    if (next_pos.y - (this->size / 2) < 1 && next_pos.x > -75 && next_pos.z > -75)
    {
        this->direction.y *= -1; //reverse y direction
        this->state = State::EXPLODED;
        if (friction)
        {
            this->speed *= 0.9; //reduce speed by 10%
        }
    }
    else if (next_pos.y - (this->size / 2) < -3)
    { //fell off the stage
        this->state = State::DEAD;
    }
    else
    {
        this->position = next_pos;
    }

    //update rotation angles so particles spin
    this->rotationAngle.x = (float)((int)(this->rotationAngle.x + 5) % 360);
    this->rotationAngle.y = (float)((int)(this->rotationAngle.y + 5) % 360);
    this->rotationAngle.z = (float)((int)(this->rotationAngle.z + 5) % 360);

    //decrease age, check for death
    this->age += deltaTime;
    this->immunity -= deltaTime;
    if (this->age >= max_life)
    {
        this->state = State::DEAD;
    }
    // apply gravity to direction
    this->direction = this->direction - (gravity * deltaTime);
};

bool Particle::collision(Vector3 center, Vector3 size)
{

    //check if particle is within the bounds of the object
    if ((this->position.x > center.x - size.x / 2 &&
        this->position.x < center.x + size.x / 2) &&
        (this->position.y > center.y - size.y / 2 &&
        this->position.y < center.y + size.y / 2) &&
        (this->position.z > center.z - size.z / 2 &&
        this->position.z < center.z + size.z / 2))
    {
        this->direction.x = center.x - this->position.x;
        this->direction.z = center.z - this->position.z;

        return true;
    }
    return false;
};

float Particle::distance(Vector3 other)
{
    return this->position.distanceTo(other);
};