#include "Particle.h"
#include <minmax.h>

Particle::Particle(FVector2f position_)
{
	position= position_;
	velocity.ZeroOut();
	acceleration.ZeroOut();

	this->mass= 1.0f;
	this->gas_constant= 0.2;
	this->rest_density= 2.0f;
	this->base_viscosity= 6.0f;
	this->tension= 0.005f;

	this->conduction= 0.2f;
	expansion_factor= 1.2f;
	this->heat= 0.0f;
	this->heat_delta= 0.0f;

	this->foo= 0.0f;
	this->normal.ZeroOut();

	this->static_= false;
}

void Particle::Step(float t)
{
	if(!static_)
	{
		position+= (velocity* t);
		velocity+= (acceleration* t);
		
		heat+= heat_delta;
	}

	acceleration.ZeroOut();
	heat_delta= 0.0f;
}

float Particle::GetViscosity()
{
	return (0.05f* base_viscosity)+ (0.95f* base_viscosity* (1- min(1.0f, heat/ 10.0f)));
}
