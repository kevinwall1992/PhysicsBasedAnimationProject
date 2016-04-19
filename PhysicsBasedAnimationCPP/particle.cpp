#include "Particle.h"

Particle::Particle(FVector2f position_)
{
	position= position_;
	velocity.ZeroOut();
	acceleration.ZeroOut();

	this->mass= 1.0f;
	this->gas_constant= 0.2;
	this->rest_density= 2.0f;
	this->viscosity= 0.05f;
	this->tension= 0.05f;
	this->heat= 0.0f;

	//this->foo= 0.0f;
	this->normal.ZeroOut();

	this->static_= false;
}

void Particle::Step(float t)
{
	if(!static_)
	{
		position+= (velocity* t);
		velocity+= (acceleration* t);
	}

	acceleration.ZeroOut();
}