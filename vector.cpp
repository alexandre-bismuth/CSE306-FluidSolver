#include "headers/vector.hpp"
#include "headers/utils.hpp"

// Code from Ray Tracer project - https://github.com/alexandre-bismuth/CSE306-RayTracer

Vector::Vector(double x, double y, double z)
{
    data[0] = x;
    data[1] = y;
    data[2] = z;
}

double Vector::norm2() const { return sqr(data[0]) + sqr(data[1]) + sqr(data[2]); }

double Vector::norm() const { return sqrt(norm2()); }

void Vector::normalize()
{
    double n = norm();
    data[0] /= n;
    data[1] /= n;
    data[2] /= n;
}

double Vector::operator[](int i) const { return data[i]; }
double &Vector::operator[](int i) { return data[i]; }

Vector operator+(const Vector &a, const Vector &b) { return Vector(a[0] + b[0], a[1] + b[1], a[2] + b[2]); }
Vector operator-(const Vector &a, const Vector &b) { return Vector(a[0] - b[0], a[1] - b[1], a[2] - b[2]); }
Vector operator-(const Vector &a) { return Vector(-a[0], -a[1], -a[2]); }
Vector operator*(const double a, const Vector &b) { return Vector(a * b[0], a * b[1], a * b[2]); }
Vector operator*(const Vector &a, const double b) { return Vector(a[0] * b, a[1] * b, a[2] * b); }
Vector operator*(const Vector &a, const Vector &b) { return Vector(a[0] * b[0], a[1] * b[1], a[2] * b[2]); }
Vector operator/(const Vector &a, const double b) { return Vector(a[0] / b, a[1] / b, a[2] / b); }

void operator+=(Vector &a, const Vector &b) { a = a + b; }

double dot(const Vector &a, const Vector &b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }

Vector cross(const Vector &a, const Vector &b)
{
    return Vector(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}

Vector normalize(const Vector &a) { return a / sqrt(sqr(a[0]) + sqr(a[1]) + sqr(a[2])); }