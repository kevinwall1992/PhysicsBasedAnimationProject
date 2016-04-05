#include "Math_.h"

float DegreesToRadians(float degrees)
{
	return (float)((M_PI/ 180)* degrees);
}

float RadiansToDegrees(float radians)
{
	return (float)((180/ M_PI)* radians);
}

int Factorial(unsigned int value)
{
	int product= 1;

	for(unsigned int i= 2; i< value; i++)
		product*= i;

	return product;
}

int Pow(int base, unsigned int exponent)
{
	int product= 1;

	for(unsigned int i= 0; i< exponent; i++)
		product*= base;

	return product;
}