// Include the header file for syntax
#include "cannon.h"
#include "shape.h"
#include "../utils/colour.h"
#include "../utils/material.h"
#include "../utils/vector3.h"
#include <cmath>

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

Cannon::Cannon(
    Vector3 rotation,
    Vector3 center,
    Vector3 size,
    Colour colour,
    Material material
) : Shape(center, size, colour, material)
{
    this->rotation = rotation;
    this->center = center;
    this->size = size;
    this->colour = colour;
    this->material = material;
};

void Cannon::draw() {
    Vector3 up = Vector3(0.0f, 8.0f, 0.0f);
    up = Vector3(
        up.x,
        up.y * cos(this->rotation.x * (M_PI / 180.0f)) - up.z * sin(this->rotation.x * (M_PI / 180.0f)),
        up.y * sin(this->rotation.x * (M_PI / 180.0f)) + up.z * cos(this->rotation.x * (M_PI / 180.0f)));
    up = Vector3(
        up.x * cos(this->rotation.z * (M_PI / 180.0f)) - up.y * sin(this->rotation.z * (M_PI / 180.0f)),
        up.x * sin(this->rotation.z * (M_PI / 180.0f)) + up.y * cos(this->rotation.z * (M_PI / 180.0f)),
        up.z);
    up = Vector3(
        up.x * cos(this->rotation.y * (M_PI / 180.0f)) + up.z * sin(this->rotation.y * (M_PI / 180.0f)),
        up.y,
        up.z * cos(this->rotation.y * (M_PI / 180.0f)) - up.x * sin(this->rotation.y * (M_PI / 180.0f)));
    // Not working
    glPushMatrix();
    // Apply transformations
    glTranslatef(this->center.x, this->center.y, this->center.z);
    // Apply lighting from material
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient.getColour());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse.getColour());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular.getColour());
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
    // Colour the cannon
    glColor3f(this->colour.r, this->colour.g, this->colour.b);
    // Connect cannon center and up vector
    // change line thickness
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glVertex3f(this->center.x, this->center.y, this->center.z);
    glVertex3f(up.x, up.y, up.z);
    glEnd();
    glTranslatef(up.x, up.y, up.z);
    glutWireSphere(0.5f, 10, 10);
    glPopMatrix();
};