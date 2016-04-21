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
		vector<Particle *> **grid;//Try turning this into flat array of Particle arrays
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
	float Poly6Kernel(float r);
	float Poly6Kernel_Derivative(float r);
	float Poly6Kernel_SecondDerivative(float r);
	float SpikyKernel(float r);
	float SpikyKernel_Derivative(float r);
	float ViscosityKernel_SecondDerivative(float r);

	float Poly6Kernel_Lookup(float r);
	void InitializeLookups();

	class ParticlePhysicsSystem
	{
		vector<Particle *> particles;
		AccelerationGrid *acceleration_grid;

		void UpdateParticleProperties();
		void ComputeAcceleration(Particle *particle);//make sure we take advantage of symmetry
		void ComputeHeatTransfer(Particle *particle);
		void Collide(Particle *particle);

	public:
		ParticlePhysicsSystem();
		~ParticlePhysicsSystem();

		vector<Particle *> GetParticles();

		void Simulate(float total_time_step, int step_count);
	};

	extern ParticlePhysicsSystem *particle_physics_system;//test making this not a pointer

	extern const int poly6_kernel_sampling_resolution[1];//make resolution 1
	extern const int poly6_kernel_derivative_sampling_resolution[1];
	extern const int poly6_kernel_second_derivative_sampling_resolution[1];
	extern const int spiky_kernel_derivative_sampling_resolution[1];
	//extern const int compute_force_sampling_resolution[6];

	void Initialize();
	void Update();
	void Conclude();
}

#endif