#include "Particle.h"

Particle::Particle(FVector2f position_)
{
	position= position_;
	velocity.ZeroOut();

	this->mass= 1.0f;
	this->gas_constant= 0.2;
	this->rest_density= 2.0f;
	this->viscosity= 1.5f;

	this->static_= false;
}

void Particle::Step(float t)
{
	if(static_)
		return;

	position+= (velocity* t);
    velocity+= (acceleration* t);
}