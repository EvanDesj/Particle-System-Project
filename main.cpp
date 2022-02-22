/*
Assignment 3:
    - Name: Mark Hutchison
    - Pronouns: he/him/his
    - Student ID: hutchm6
    - Email: [hutchm6@mcmaster.ca](mailto:hutchm6@mcmaster.ca)
    - Operating System: Arch Linux

    - Name: Evan Desjardine
    - Pronouns: he/him/his
    - Student ID: desjarde
    - Email: [desjarde@mcmaster.ca](mailto:desjarde@mcmaster.ca)
    - Operating System: Windows 10
*/

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

// include custom libraries
#include "particle.h"
#include "particleSystem.h"
#include "./shapes/cannon.h"
#include "./shapes/cube.h"
#include "./shapes/shape.h"
#include "./utils/colour.h"
#include "./utils/material.h"
#include "./utils/vector3.h"

// include standard libraries
#include <iostream>
#include <cmath>
#include <list>

// Define GLUT Constants
#define WINDOW_TITLE "3D Particles"

// Global Variables
#define RED Colour(1.0f, 0.0f, 0.0f, 1.0f)
#define GREEN Colour(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE Colour(0.0f, 0.0f, 1.0f, 1.0f)
#define STEEL_BLUE Colour(0.53f, 0.63f, 0.75f, 1.0f)

// Window size and globals
int windowWidth = 800;
int windowHeight = 600;
int mouseX = 0;
int mouseY = 0;

// Glut variables
int timerFunc = 20;
Vector3 camera = Vector3(40.0f, 50.0f, 40.0f);
float cameraDelta = 2.0f;

float light[4] = {
    camera.x + 5.0f,
    camera.y + 5.0f,
    camera.z + 5.0f,
    1.0f};
Colour ambient = Colour(0.5f, 0.5f, 0.5f, 1.0f);
Colour diffuse = Colour(0.4f, 0.3f, 0.4f, 1.0f);
Colour specular = Colour(1.0f, 1.0f, 1.0f, 1.0f);

// Simulation variables
float axisLength = 100.0f;
bool friction = true;
bool animation = true;
bool drawAxisPlease = false;
int numParticles = 25;
float cannonDelta = 2.0f;

// Global array of cubes
int numCubes = 5;
Cube cubes[5] = {
    Cube(Vector3(0.0f, 0.0f, 0.0f),
         Vector3(150.0f, 1.0f, 150.0f),
         Colour(0.36f, 0.27f, 0.54f, 1.0f),
         Material(
             Colour(0.36f, 0.27f, 0.54f, 1.0f),
             Colour(0.30f, 0.20f, 0.50f, 1.0f),
             Colour(0.45f, 0.35f, 0.70f, 1.0f),
             0.3f)),
    Cube(Vector3(-75.0f, 3.0f, 0.0f),
         Vector3(3.0f, 3.0f, 150.0f),
         Colour(0.25f, 0.65f, 0.78f, 1.0f),
         Material(
             Colour(0.25f, 0.65f, 0.78f, 1.0f),
             Colour(0.20f, 0.60f, 0.72f, 1.0f),
             Colour(0.30f, 0.70f, 0.85f, 1.0f),
             0.1f)),
    Cube(Vector3(75.0f, 3.0f, 0.0f),
         Vector3(3.0f, 3.0f, 150.0f),
         Colour(0.25f, 0.65f, 0.78f, 1.0f),
         Material(
             Colour(0.25f, 0.65f, 0.78f, 1.0f),
             Colour(0.20f, 0.60f, 0.72f, 1.0f),
             Colour(0.30f, 0.70f, 0.85f, 1.0f),
             0.1f)),
    Cube(Vector3(0.0f, 3.0f, -75.0f),
         Vector3(150.0f, 3.0f, 3.0f),
         Colour(0.25f, 0.65f, 0.78f, 1.0f),
         Material(
             Colour(0.25f, 0.65f, 0.78f, 1.0f),
             Colour(0.20f, 0.60f, 0.72f, 1.0f),
             Colour(0.30f, 0.70f, 0.85f, 1.0f),
             0.1f)),
    Cube(Vector3(0.0f, 3.0f, 75.0f),
         Vector3(150.0f, 3.0f, 3.0f),
         Colour(0.25f, 0.65f, 0.78f, 1.0f),
         Material(
             Colour(0.25f, 0.65f, 0.78f, 1.0f),
             Colour(0.20f, 0.60f, 0.72f, 1.0f),
             Colour(0.30f, 0.70f, 0.85f, 1.0f),
             0.1f))};

Cannon cannon = Cannon(
    Vector3(0.0f, 0.0f, 0.0f),
    Vector3(0.0f, 0.0f, 0.0f),
    Vector3(1.0f, 1.0f, 1.0f),
    Colour(0.1f, 0.1f, 0.1f, 1.0f),
    Material(
        Colour(0.0f, 0.0f, 0.0f, 1.0f),
        Colour(0.01f, 0.01f, 0.01f, 1.0f),
        Colour(0.50f, 0.50f, 0.50f, 1.0f),
        32.0f));

ParticleSystem particleSys = ParticleSystem(
    cannon.center + Vector3(0.0f, 8.0f, 0.0f), // Origin (25 units above the ground)
    Vector3(0.0f, 3.8f, 0.0f)                  // Gravity vector
);

// Top-level event handler
void eventCommandHandler(int value)
{
    switch (value)
    {
    case 0:
        exit(0);
        break;
    case 1:
        friction = !friction;
        break;
    case 2:
        animation = !animation;
        break;
    case 3:
        particleSys.~ParticleSystem();
        for (int i = 0; i < numParticles; i++)
        {
            particleSys.addParticle(
                particleSys.generateParticle(particleSys.origin, State::ALIVE, false)
            );
        }
        break;
    case 4:
        // if lighting, turn lights off
        if (glIsEnabled(GL_LIGHTING))
        {
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
        }
        // if not lighting, turn lights on
        else
        {
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
        }
        break;
    case 5:
        camera.x += cameraDelta;
        break;
    case 6:
        camera.x -= cameraDelta;
        break;
    case 7:
        camera.z += cameraDelta;
        break;
    case 8:
        camera.z -= cameraDelta;
        break;
    case 9:
        camera.y += cameraDelta;
        break;
    case 10:
        camera.y -= cameraDelta;
        break;
    case 11:
        if (cannon.rotation.x < -10) {
            cannon.rotation.x += cannonDelta;
            particleSys.setDirection(cannon);
        }
        break;
    case 12:
        if (cannon.rotation.x > -170) {
            cannon.rotation.x -= cannonDelta;
            particleSys.setDirection(cannon);
        }
        break;
    case 13:
        cannon.rotation.y -= cannonDelta;
        particleSys.setDirection(cannon);
        break;
    case 14:
        cannon.rotation.y += cannonDelta;
        particleSys.setDirection(cannon);
        break;
    case 15:
        drawAxisPlease = !drawAxisPlease;
        break;
    default:
        break;
    }
};

// Draw Axis
void drawAxis()
{
    // Push Matrix
    glPushMatrix();
    glLineWidth(2.0);
    glBegin(GL_LINES);
    // Red X-Axis
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, RED.getColour());
    glMaterialfv(GL_FRONT, GL_SPECULAR, RED.getColour());
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
    glColor3fv(RED.getColour());
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(axisLength, 0.0, 0.0);
    glPopMatrix();
    // Blue Y-Axis
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, BLUE.getColour());
    glMaterialfv(GL_FRONT, GL_SPECULAR, BLUE.getColour());
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
    glColor3fv(BLUE.getColour());
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, axisLength, 0.0);
    glPopMatrix();
    // Green Z-Axis
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, GREEN.getColour());
    glMaterialfv(GL_FRONT, GL_SPECULAR, GREEN.getColour());
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
    glColor3fv(GREEN.getColour());
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, axisLength);
    glPopMatrix();
    glEnd();
    // Pop Matrix
    glPopMatrix();
};

// Display Callback Function
void display()
{
    // Clear and prepare
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Position Camera
    gluLookAt(
        camera.x, camera.y, camera.z,  // Camera Position
        0.0, 10.0, 0.0,                // Look at x, y, z
        0.0, 10.0, 0.0                 // Up x, y, z
    );

    // Draw Axis
    if (drawAxisPlease)
    {
        drawAxis();
    }

    // Draw cubes
    for (int i = 0; i < numCubes; i++)
    {
        cubes[i].draw();
    }

    cannon.draw();

    // Draw particles
    // for (ParticleSystem i : particleSystems) {
    //     i.draw(cubes, numCubes, animation);
    // }
    particleSys.draw(cubes, numCubes, animation);

    // Swap Buffers
    glutSwapBuffers();
};

// Animate Callback Function
void animate(int v)
{
    // Update particlesparticleSys
    particleSys.update(friction);

    // for (ParticleSystem i : particleSystems) {
    //     particleSystems = i.update(friction, particleSystems);
    // }

    // Redraw
    glutPostRedisplay();

    // Call this function again in 20 milliseconds
    glutTimerFunc(timerFunc, animate, 0);
};

// Reshape Callback Function
void reshape(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
};

// Keyboard Callback Function
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
    case 'q':
        eventCommandHandler(0);
        break;
    case 'f':
        eventCommandHandler(1);
        break;
    case ' ':
        eventCommandHandler(2);
        break;
    case 'r':
        eventCommandHandler(3);
        break;
    case 'l':
        eventCommandHandler(4);
        break;
    case '+':
        eventCommandHandler(9);
        break;
    case '-':
        eventCommandHandler(10);
        break;
    case 'a':
        eventCommandHandler(11);
        break;
    case 'd':
        eventCommandHandler(12);
        break;
    case 'w':
        eventCommandHandler(13);
        break;
    case 's':
        eventCommandHandler(14);
        break;
    case 'p':
        eventCommandHandler(15);
        break;
    default:
        break;
    }
    glutPostRedisplay();
};

// Special Keyboard Callback Function
void specialKeyboard(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        eventCommandHandler(5);
        break;
    case GLUT_KEY_DOWN:
        eventCommandHandler(6);
        break;
    case GLUT_KEY_LEFT:
        eventCommandHandler(7);
        break;
    case GLUT_KEY_RIGHT:
        eventCommandHandler(8);
        break;
    default:
        break;
    }
    glutPostRedisplay();
};

// Mouse Callback Function
void mouse(int button, int state, int x, int y)
{
    y = windowHeight - y;
    glutPostRedisplay();
};

// Mouse Motion Callback Function
void mouseMotion(int x, int y)
{
    y = windowHeight - y;
    mouseX = x;
    mouseY = y;
};

// Mouse Passive Motion Callback Function
void mousePassiveMotion(int x, int y)
{
    y = windowHeight - y;
    mouseX = x;
    mouseY = y;
};

// Menu Callback Function
void menu(int value)
{
    eventCommandHandler(value);
    glutPostRedisplay();
};

// Glut Initialization Function
void init()
{
    // Prepare the window
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(WINDOW_TITLE);
    // Set background colour
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Turn on the lights!
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // Generate Light
    glLightfv(GL_LIGHT0, GL_POSITION, light);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient.getColour());
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse.getColour());
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular.getColour());
    // Set up the viewport
    glViewport(0, 0, windowWidth, windowHeight);
    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set-Up 3D Perspective
    gluPerspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 500.0f);
};

// Print Program Instructions
void printInstructions()
{
    using namespace std;
    cout << "Program Instructions:" << endl;
    cout << "q/esc: Quit" << endl;
    cout << "Space: Toggle Particle Spawning" << endl;
    cout << "r: Reset Particle System" << endl;
    cout << "l: Toggle Light" << endl;
    cout << "f: Toggle Friction" << endl;
    cout << "w: Adjust Cannon x-rotation" << endl;
    cout << "s: Adjust Cannon x-rotation" << endl;
    cout << "a: Adjust Cannon z-rotation" << endl;
    cout << "d: Adjust Cannon z-rotation" << endl;
    cout << "Arrow Key Up: Move Camera X-Axis" << endl;
    cout << "Arrow Key Down: Move Camera X-Axis" << endl;
    cout << "Arrow Key Left: Move Camera Z-Axis" << endl;
    cout << "Arrow Key Right: Move Camera Z-Axis" << endl;
    cout << "+: Move Camera Y-Axis" << endl;
    cout << "-: Move Camera Y-Axis" << endl;
    cout << "p: Toggle Axis" << endl;
};

// Main program
int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    // Initialize the window
    init();
    // Print instructions
    printInstructions();
        // Initialize the particles
        for (int i = 0; i < numParticles; i++)
    {
        particleSys.addParticle(
            particleSys.generateParticle(particleSys.origin, State::ALIVE, false)
        );
    }
    // Bind callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mousePassiveMotion);
    // Animate function
    glutTimerFunc(timerFunc, animate, 0);
    // Set-Up 3D
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    // Start the main loop
    glutMainLoop();
};