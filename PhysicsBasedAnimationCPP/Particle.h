#ifndef __PBA_PARTICLE__
#define __PBA_PARTICLE__

#include "FVector.h"

#include <vector>
using std::vector;

#define density_radius 1.0f
#define pressure_radius 1.0f
#define viscosity_radius 1.0f
struct Particle
{
	FVector2f position;
	FVector2f velocity;
	FVector2f acceleration;
	//FVector2f old_acceleration;

	float mass, gas_constant, rest_density;
	float pressure, density;
	float viscosity;

	bool static_;

	//implementation stuff
	vector<Particle *> neighbors;
	FVector2i grid_index;
	vector<Particle *> force_partners;//Try making this a hashtable
	
	Particle(FVector2f position);

	void Step(float time_step);
};

#endif