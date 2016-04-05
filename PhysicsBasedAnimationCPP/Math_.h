#ifndef __RHWS_MATH__
#define __RHWS_MATH__

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef FLT_MAX
#define FLT_MAX std::numeric_limits<float>::max()
#endif

//add none type
struct Axis{enum Enum {X, Y, Z, Count};};

float DegreesToRadians(float degrees);
float RadiansToDegrees(float radians);

int Factorial(unsigned int value);
int Pow(int base, unsigned int exponent);

#endif