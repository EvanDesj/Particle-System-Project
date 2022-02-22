// Include header file for syntax
#include "vector3.h"

// include standard libraries needed
#include <cmath>

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
};
Vector3::Vector3(float *v)
{
    this->x = v[0];
    this->y = v[1];
    this->z = v[2];
};
Vector3::Vector3()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
};
float Vector3::length()
{
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
};
Vector3 Vector3::normalize()
{
    float length = this->length();
    return Vector3(this->x / length, this->y / length, this->z / length);
};
float Vector3::distanceTo(Vector3 v)
{
    return sqrt(pow(this->x - v.x, 2) + pow(this->y - v.y, 2) + pow(this->z - v.z, 2));
};
float Vector3::fastDistanceTo(Vector3 v)
{
    return pow(this->x - v.x, 2) + pow(this->y - v.y, 2) + pow(this->z - v.z, 2);
};
Vector3 Vector3::cross(Vector3 v)
{
    return Vector3(this->y * v.z - this->z * v.y,
                   this->z * v.x - this->x * v.z,
                   this->x * v.y - this->y * v.x);
};
Vector3 Vector3::operator+(Vector3 v)
{
    return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
};
Vector3 Vector3::operator-(Vector3 v)
{
    return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
};
Vector3 Vector3::operator*(Vector3 v)
{
    return Vector3(this->x * v.x, this->y * v.y, this->z * v.z);
};
Vector3 Vector3::operator*(float f)
{
    return Vector3(this->x * f, this->y * f, this->z * f);
};
Vector3 Vector3::operator/(Vector3 v)
{
    return Vector3(this->x / v.x, this->y / v.y, this->z / v.z);
};
Vector3 Vector3::operator/(float f)
{
    return Vector3(this->x / f, this->y / f, this->z / f);
};
bool Vector3::operator==(Vector3 v)
{
    return this->x == v.x && this->y == v.y && this->z == v.z;
};
bool Vector3::operator!=(Vector3 v)
{
    return this->x != v.x || this->y != v.y || this->z != v.z;
};
float Vector3::operator[](int i)
{
    switch (i)
    {
    case 0:
        return this->x;
    case 1:
        return this->y;
    case 2:
        return this->z;
    default:
        return 0;
    }
};