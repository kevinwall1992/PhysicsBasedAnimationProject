#include "Physics.h"
#include "LookupTable.h"

#include <minmax.h>
#include <iostream>
using std::cout;


namespace Physics
{
	ParticlePhysicsSystem *particle_physics_system;

	const int poly6_kernel_sampling_resolution[2]= {4, 1};

	typedef LookupTable<float, 2, poly6_kernel_sampling_resolution> Poly6LookupTable;
	Poly6LookupTable *poly6_kernel_lookup_table;


	FVector2i AccelerationGrid::ComputeParticleIndex(Particle *p)
	{
		//static FVector2f half_vector= MakeFVector2f(0.5f, 0.5f);

		FVector2f float_index= ((p->position- grid_low)/ grid_cell_size);
		return MakeFVector2i(floor(float_index[0]), floor(float_index[1]));//floors not actually necessary
	}

	/*FVector2i AccelerationGrid::GetParticleIndex(Particle *p)
	{
		return p->grid_index;
	}*/
	
	//try using grid index to get rough but fast approximation of which grid cells to look in
	//(or stop storing grid indices, because we dont use them)
	void AccelerationGrid::ComputeParticleNeighbors(Particle *p)
	{
		float max_distance= 1;

		p->neighbors.clear();

		float squared_max_distance= max_distance* max_distance;
		float grid_max_distance= max_distance/ grid_cell_size;
		float grid_squared_max_distance= pow(grid_max_distance, 2);//faster to be explicit? Also, having to calculate this every call...
		FVector2f grid_position= (p->position- grid_low)/ grid_cell_size;

		int min_x= floor(grid_position[0]- grid_max_distance);
		int max_x= floor(grid_position[0]+ grid_max_distance);
		int min_y= floor(grid_position[1]- grid_max_distance);
		int max_y= floor(grid_position[1]+ grid_max_distance);

		for(int i= min(max(min_x, 0), grid_size); i<= max(min(max_x, grid_size), 0); i++)
		{
			for(int j= min(max(min_y, 0), grid_size); j<= max(min(max_y, grid_size), 0); j++)
			{
				if(grid[i][j].size()== 0)
					continue;

				FVector2f clamped_position= MakeFVector2f(min(max(grid_position[0], i), i+ 1),
					                                      min(max(grid_position[1], j), j+ 1));//break this up over both loops
				if(grid_position.SquaredDistance(clamped_position)> grid_squared_max_distance)
					continue;
				
				for(unsigned int k= 0; k< grid[i][j].size(); k++)
				{
					if(p->static_)
						continue;
					if((p->position.SquaredDistance(grid[i][j][k]->position)>= squared_max_distance))//should be strictly greater than
						continue;

					p->neighbors.push_back(grid[i][j][k]);
				}
			}
		}


		if((min_x>= 0 && max_x< grid_size) && (min_y>= 0 && max_y< grid_size));
		else
		{
			for(unsigned int i= 0; i< unaccelerated_space.size(); i++)
				if(!p->static_)
					if(p->position.SquaredDistance(unaccelerated_space[i]->position)< squared_max_distance)
						p->neighbors.push_back(unaccelerated_space[i]);
		}

		//this is hacky... should NOT be in this function.
		p->force_partners= p->neighbors;
	}

	AccelerationGrid::AccelerationGrid(FVector2f grid_low_, float grid_cell_size_, int grid_size_)
	{
		grid_low= grid_low_;
		grid_cell_size= grid_cell_size_;
		grid_size= grid_size_;


		grid= new vector<Particle *> *[grid_size];

		int max_particles= 100;
		for(int i= 0; i< grid_size; i++)
			grid[i]= new vector<Particle *>[grid_size];
	}

	void AccelerationGrid::AddParticle(Particle *p)
	{
		FVector2i index= ComputeParticleIndex(p);
		if(!((index[0]< 0 || index[0]>= grid_size) || (index[1]< 0 || index[1]>= grid_size)))//could test out unlikely thing here
		{
			grid[index[0]][index[1]].push_back(p);
			p->grid_index= index;
		}
		else
		{
			unaccelerated_space.push_back(p);
			p->grid_index= MakeFVector2i(-1, -1);
			//p->static_= true;
		}

		
	}

	void AccelerationGrid::UpdateGrid(const vector<Particle *> &particles)
	{
		for(int i= 0; i< grid_size; i++)
			for(int j= 0; j< grid_size; j++)
				grid[i][j].clear();
		unaccelerated_space.clear();

		for(unsigned int i= 0; i< particles.size(); i++)
		{
			Particle *p =  particles[i];
			AddParticle(p);
		}

		for(unsigned int i= 0; i< particles.size(); i++)
		{
			Particle *p =  particles[i];
			ComputeParticleNeighbors(p);
		}
	}

	/*vector<Particle *> * AccelerationGrid::GetNeighbors(Particle *p, float max_distance)
	{
		return &particle_neighbors[p];
	}*/


	float Poly6Kernel(float r, float h)
	{
		if(r> h)
			return 0;

		return (float)pow(pow(h, 2)- pow(r, 2), 3)* 315/ (64* M_PI* pow(h, 9));
	}

	float SpikyKernel(float r, float h)
	{
		if(r> h)
			return 0;

		return (float)pow(h- r, 3)* 15/ (M_PI* pow(h, 6));
	}

	float SpikyKernel_Derivative(float r, float h)
	{
		if(r> h)
			return 0;

		return (float)(15/ (M_PI* pow(h, 6)))* -3* pow(r- 1, 2);
	}

	float ViscosityKernel_SecondDerivative(float r, float h)
	{
		if(r> h)
			return 0;

		//return (float)(15/ (2* M_PI* pow(h, 3)))* ((-6.0/ (2* pow(h, 3)))* r+ (2.0/ pow(h, 2))+ (h/ pow(r, 3)));
		return 45* (h- r)/ (M_PI* pow(h, 6));
	}

	
	float Poly6Kernel_Oracle(float input[2])
	{
		return Poly6Kernel(input[0], input[1]);
	}
	float Poly6Kernel_Lookup(float r, float h)
	{
		float foo= Poly6Kernel(r, h);
		float bar= poly6_kernel_lookup_table->Lookup(MakeFVector2f(r, h));

		return bar;
	}

	void FreeLookups()
	{
		delete poly6_kernel_lookup_table;
	}


	void ParticlePhysicsSystem::UpdateParticleProperties()
	{
		for(unsigned int i= 0; i< particles.size(); i++)
		{
			Particle *p= particles[i];

			float density= 0;
			for(unsigned int j= 0; j< p->neighbors.size(); j++)
				density+= p->mass* Poly6Kernel_Lookup(p->position.Distance(p->neighbors[j]->position), density_radius);

			p->density= density;
			p->pressure= p->gas_constant* (p->density- p->rest_density);
		}
	}

	float foo= 0;
	void ParticlePhysicsSystem::ComputeAcceleration(Particle *p)
	{
		//if(p->static_)
		//	return;

		static FVector2f center= MakeFVector2f(0.001f, 0.001f);
		FVector2f force;//you could try static as an experiment
		force.ZeroOut();

		for(unsigned int i= 0; i< p->force_partners.size(); i++)
		{
			if(p->force_partners[i]== nullptr)
				continue;

			Particle *n= p->neighbors[i];
			if(n== p)
				continue;

			FVector2f partner_force;
			partner_force.ZeroOut();

			float weight= n->mass / n->density;
			FVector2f attraction_vector= (p->position- n->position).Normalized();//curious whether this creates two vectors or one

			float distance= p->position.Distance(n->position);
			//Pressure
			float pressure_magnitude= (p->pressure+ n->pressure)* (weight* SpikyKernel_Derivative(distance, pressure_radius)/ -2);
			//if(pressure_magnitude< 0)
			//	cout << "Pressure is negative!";
			//else
			//	cout << "Pressure is positive";
			partner_force+= (attraction_vector* (pressure_magnitude));

			//Viscosity
			partner_force+= (p->velocity- n->velocity)* (p->viscosity* weight* ViscosityKernel_SecondDerivative(distance, viscosity_radius)* -1);


			force+= partner_force;
			n->acceleration+= partner_force/ (n->mass* -1);

			p->force_partners[i]= nullptr;
			for(unsigned int j= 0; j< n->force_partners.size(); j++)
			{
				if(n->force_partners[j]== p)
				{
					n->force_partners[j]= nullptr;
					break;
				}
			}
		}

		//Gravity
		force+= (center- p->position).Normalized()* 0.05f;

		//Friction
		//force+= p->velocity* -0.05f;

		//Test
		if(foo> 16.0f && foo< 16.3f)
		{
			p->static_= false;
		}

		if(p->mass< 1.5f)
		{
			//FVector2f flow_force= MakeFVector2f(0.0f, -10.5f);
			//force+= flow_force* (max(0.5- abs(p->position[0]), 0.01)/ 0.5)* (max(5- abs(p->position[1]), 0)/ 5);
			//force+= flow_force* (max(10- abs(p->position[0]), 0.01)/ 10)* (max(5- abs(p->position[1]), 0)/ 5);
			//force+= flow_force* (max(0.5- abs(p->position[0]+ 2), 0.0f)/ 0.5);
			//force+= flow_force* (max(0.5- abs(p->position[0]- 2), 0.0f)/ 0.5)* -1;
		}

		p->acceleration+= force/ p->mass;
	}

	ParticlePhysicsSystem::ParticlePhysicsSystem()
	{
		acceleration_grid= new AccelerationGrid(MakeFVector2f(-100.0f, -100.0f), 1.0f, 200);

		for(int i= -40; i<= 40; i++)
		{
			for(int j= -40; j<= 40; j++)
			{
				Particle *p= new Particle(MakeFVector2f(i/ 1.8f, j/ 1.8f));
				particles.push_back(p);
				acceleration_grid->AddParticle(p);
			}
		}

		//if(false)
		for(int i= -6; i<= 0; i++)
		{
			for(int j= 80; j<= 140; j++)
			{
				Particle *p= new Particle(MakeFVector2f(i/ 2.0f, j/ 2.0f));
				p->static_= true;
				p->velocity= MakeFVector2f(0.0f, -10.0f);
				p->mass= 1.0f;
				p->gas_constant/= p->mass;
				p->rest_density/= p->mass;
				particles.push_back(p);
				acceleration_grid->AddParticle(p);
			}
		}
	}

	ParticlePhysicsSystem::~ParticlePhysicsSystem()
	{
		delete acceleration_grid;

		for(unsigned int i= 0; i< particles.size(); i++)
			delete particles[i];
	}

	vector<Particle *> ParticlePhysicsSystem::GetParticles()
	{
		return particles;
	}

	void ParticlePhysicsSystem::Simulate(float total_time_step, int step_count)
	{
		FVector2f force;//opt
		for(int step= 0; step< step_count; step++)
		{
			acceleration_grid->UpdateGrid(particles);
			UpdateParticleProperties();

			for(unsigned int i= 0; i< particles.size(); i++)
			{
				ComputeAcceleration(particles[i]);
			}

			for(unsigned int i= 0; i< particles.size(); i++)
				particles[i]->Step(total_time_step/ step_count);

			foo+= total_time_step/ step_count;
		}
	}

	void InitializeLookups()
	{
		{
			FVector2f low= MakeFVector2f(0.0f, 1.0f);
			FVector2f high= MakeFVector2f(1.0f, 1.0f);
			FVector2i resolution= MakeFVector2i(10, 1);
			poly6_kernel_lookup_table= new Poly6LookupTable(Poly6Kernel_Oracle, low, high);
		}
	}

	void Initialize()
	{
		particle_physics_system= new ParticlePhysicsSystem();
		InitializeLookups();
	}

	void Update()
	{
		particle_physics_system->Simulate(0.0333f* 2, 1);
	}

	void Conclude()
	{
		delete poly6_kernel_lookup_table;
		delete particle_physics_system;
	}

}