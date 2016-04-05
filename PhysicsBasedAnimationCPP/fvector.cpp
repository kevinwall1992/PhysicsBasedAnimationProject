#include "FVector.h"

FVector2f MakeFVector2f(float d0, float d1)
{
	FVector2f v;
	v[0]= d0;
	v[1]= d1;

	return v;
}

FVector2i MakeFVector2i(int i0, int i1)
{
	FVector2i v;
	v[0]= i0;
	v[1]= i1;

	return v;
}