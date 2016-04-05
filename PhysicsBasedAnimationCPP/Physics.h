#ifndef __PBA_PHYSICS__
#define __PBA_PHYSICS__

#include "Particle.h"
#include "Math_.h"

#include <math.h>
#include <vector>
#include <unordered_map>
using std::vector;
using std::unordered_map;

namespace Physics
{
	class AccelerationGrid
	{
		vector<Particle *> **grid;//Think this is fine...
		vector<Particle *> unaccelerated_space;
		//unordered_map<Particle *, FVector2i> particle_indices;
		//unordered_map<Particle *, vector<Particle *>> particle_neighbors;

		FVector2f grid_low;
		float grid_cell_size;
		int grid_size;

		FVector2i ComputeParticleIndex(Particle *particle);
		//FVector2i GetParticleIndex(Particle *particle);
		void ComputeParticleNeighbors(Particle *particle);

	public:
		AccelerationGrid(FVector2f grid_low, float grid_cell_size, int grid_size);
		
		void AddParticle(Particle *particle);

		//should the grid keep its own list?
		void UpdateGrid(const vector<Particle *> &particles);

		//I like the idea of making neighbors usage have to go throughthis function, even if currently unnecessary
		//vector<Particle *> * GetNeighbors(Particle *particle, float max_distance);
	};

	//need to add lookup table
	inline float Poly6Kernel(float r, float h)
	{
		if(r> h)
			return 0;

		return (float)pow(pow(h, 2)- pow(r, 2), 3)* 315/ (64* M_PI* pow(h, 9));
	}

	inline float SpikyKernel(float r, float h)
	{
		if(r> h)
			return 0;

		return (float)pow(h- r, 3)* 15/ (M_PI* pow(h, 6));
	}

	inline float SpikyKernel_Derivative(float r, float h)
	{
		if(r> h)
			return 0;

		return (float)(15/ (M_PI* pow(h, 6)))* -3* pow(r- 1, 2);
	}

	inline float ViscosityKernel_SecondDerivative(float r, float h)
	{
		if(r> h)
			return 0;

		//return (float)(15/ (2* M_PI* pow(h, 3)))* ((-6.0/ (2* pow(h, 3)))* r+ (2.0/ pow(h, 2))+ (h/ pow(r, 3)));
		return 45* (h- r)/ (M_PI* pow(h, 6));
	}

	

	class ParticlePhysicsSystem
	{
		vector<Particle *> particles;
		AccelerationGrid *acceleration_grid;

		void UpdateParticleProperties();
		void GetForce(Particle *particle, FVector2f &force);//make sure we take advantage of symmetry

	public:
		ParticlePhysicsSystem();
		~ParticlePhysicsSystem();

		vector<Particle *> GetParticles();

		void Simulate(float total_time_step, int step_count);
	};

	extern ParticlePhysicsSystem *particle_physics_system;//test making this not a pointer


	void Initialize();
	void Update();
	void Conclude();
}

#endif