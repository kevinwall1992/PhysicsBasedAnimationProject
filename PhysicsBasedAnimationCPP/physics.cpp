#include "Physics.h"
#include "LookupTable.h"

#include <minmax.h>
#include <iostream>
#include <random>
using std::cout;


namespace Physics
{
	ParticlePhysicsSystem *particle_physics_system;

	const int poly6_kernel_sampling_resolution[1]= {20};
	const int poly6_kernel_derivative_sampling_resolution[1]= {20};
	const int poly6_kernel_second_derivative_sampling_resolution[1]= {20};
	const int spiky_kernel_derivative_sampling_resolution[1]= {20};
	//const int compute_force_sampling_resolution[6]= {4, 4, 8, 6, 6, 1};

	typedef LookupTable<float, 1, poly6_kernel_sampling_resolution> Poly6LookupTable;
	Poly6LookupTable *poly6_kernel_lookup_table;
	typedef LookupTable<float, 1, poly6_kernel_derivative_sampling_resolution> Poly6DerivativeLookupTable;
	Poly6DerivativeLookupTable *poly6_kernel_derivative_lookup_table;
	typedef LookupTable<float, 1, poly6_kernel_second_derivative_sampling_resolution> Poly6SecondDerivativeLookupTable;
	Poly6SecondDerivativeLookupTable *poly6_kernel_second_derivative_lookup_table;
	typedef LookupTable<float, 1, spiky_kernel_derivative_sampling_resolution> SpikyDerivativeLookupTable;
	SpikyDerivativeLookupTable *spiky_kernel_derivative_lookup_table;
	//typedef LookupTable<FVector2f, 6, compute_force_sampling_resolution> ComputeForceLookupTable;
	//ComputeForceLookupTable *compute_force_lookup_table;


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
		float max_distance= kernel_radius;

		p->neighbors.clear();

		if(!p->static_); else
			return;

		/*vector<Particle *> particles= particle_physics_system->GetParticles();
		for(unsigned int i= 0; i< particles.size(); i++)
		{
			if(!particles[i]->static_ && p->position.Distance(particles[i]->position)< max_distance)
				p->neighbors.push_back(particles[i]);
		}*/
		
		float squared_max_distance= max_distance* max_distance;
		float grid_max_distance= max_distance/ grid_cell_size;
		float grid_squared_max_distance= pow(grid_max_distance, 2);//faster to be explicit? Also, having to calculate this every call...
		FVector2f grid_position= (p->position- grid_low)/ grid_cell_size;

		int min_x= floor(grid_position[0]- grid_max_distance);
		int max_x= floor(grid_position[0]+ grid_max_distance);
		int min_y= floor(grid_position[1]- grid_max_distance);
		int max_y= floor(grid_position[1]+ grid_max_distance);

		for(int i= min(max(min_x, 0), grid_size); i<= max(min(max_x, grid_size- 1), 0); i++)
		{
			for(int j= min(max(min_y, 0), grid_size); j<= max(min(max_y, grid_size- 1), 0); j++)
			{
				if(grid[i][j].size()== 0)
					continue;

				FVector2f clamped_position= MakeFVector2f(min(max(grid_position[0], i), i+ 1),
					                                      min(max(grid_position[1], j), j+ 1));//break this up over both loops
				if(grid_position.SquaredDistance(clamped_position)> grid_squared_max_distance)
					continue;
				
				for(unsigned int k= 0; k< grid[i][j].size(); k++)
				{
					if(grid[i][j][k]->static_)
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
				if(!unaccelerated_space[i]->static_)
					if(p->position.SquaredDistance(unaccelerated_space[i]->position)< squared_max_distance)
						p->neighbors.push_back(unaccelerated_space[i]);
		}

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

	#define h kernel_radius
	float Poly6Kernel(float r)
	{
		if(r> h)
			return 0;

		return (float)pow(pow(h, 2)- pow(r, 2), 3)* 315/ (float)(64* M_PI* pow(h, 9));
	}

	float Poly6Kernel_Derivative(float r)
	{
		if (r> h)
			return 0;

		float original= (float)pow(pow(h, 2)- pow(r, 2), 2)* r* -6* 315/ (float)(64* M_PI* pow(h, 9));
		float modified= (float)pow(pow(h, 10)- pow(r, 10), 2)* r* -1* 315/ (float)(64* M_PI* pow(h, 9));

		return modified* 0.65f+ original* 0.35f;
	}

	float Poly6Kernel_SecondDerivative(float r)
	{
		if (r> h)
			return 0;

		float original= -(float)((5* pow(h, 4)- 6* pow(h, 2)* (r, 2)+ pow(r, 4))* 6* 315/ (64* M_PI* pow(h, 9)));
		//float modified= (float)(6- (pow(h, 4)- 6* pow(h, 2)* pow(r/ 2.72f, 2)+ 9.5* pow(r* (2/ 2.72f), 10))* 6* 315/ (64* M_PI* pow(h, 9)));
		float modified= (float)(7- pow(h, 4.8)- pow(h, 3)* (pow(h, 7)- 6* pow(h, 6.9)* pow(r/ 2.72f, 2)+ 9.5* pow(r* (2/ 2.72f), 10))* 6* 315/ (64* M_PI* pow(h, 9)));

		return modified* 0.45f+ original* 0.55f;
	}

	float SpikyKernel(float r)
	{
		if(r> h)
			return 0;

		return (float)pow(h- r, 3)* 15/ (M_PI* pow(h, 6));
	}

	float SpikyKernel_Derivative(float r)
	{
		if(r> h)
			return 0;

		return (float)(15/ (M_PI* pow(h, 6)))* -3* pow(r- 1, 2);
	}

	float ViscosityKernel_SecondDerivative(float r)
	{
		if(r> h)
			return 0;

		//return (float)(15/ (2* M_PI* pow(h, 3)))* ((-6.0/ (2* pow(h, 3)))* r+ (2.0/ pow(h, 2))+ (h/ pow(r, 3)));
		return 45* (h- r)/ (M_PI* pow(h, 6));
	}
	#undef h
	
	float Poly6Kernel_Oracle(FVector<float, 1> input_vector)
	{
		return Poly6Kernel(input_vector.v[0]);
	}
	float Poly6Kernel_Lookup(float r)
	{
		FVector<float, 1> input_vector;
		input_vector[0]= r;

		return poly6_kernel_lookup_table->Lookup(input_vector);
	}

	float Poly6Kernel_Derivative_Oracle(FVector<float, 1> input_vector)
	{
		return Poly6Kernel_Derivative(input_vector.v[0]);
	}
	float Poly6Kernel_Derivative_Lookup(float r)
	{
		FVector<float, 1> input_vector;
		input_vector[0]= r;

		return poly6_kernel_derivative_lookup_table->Lookup(input_vector);
	}

	float Poly6Kernel_SecondDerivative_Oracle(FVector<float, 1> input_vector)
	{
		return Poly6Kernel_SecondDerivative(input_vector.v[0]);
	}
	float Poly6Kernel_SecondDerivative_Lookup(float r)
	{
		FVector<float, 1> input_vector;
		input_vector[0]= r;

		return poly6_kernel_second_derivative_lookup_table->Lookup(input_vector);
	}

	float SpikyKernel_Derivative_Oracle(FVector<float, 1> input_vector)
	{
		return SpikyKernel_Derivative(input_vector.v[0]);
	}
	float SpikyKernel_Derivative_Lookup(float r)
	{
		FVector<float, 1> input_vector;
		input_vector[0]= r;

		return spiky_kernel_derivative_lookup_table->Lookup(input_vector);
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
			{
				Particle *n= p->neighbors[j];
				density+= (1+ (p->expansion_factor- 1)* p->heat/ 100.0f)* n->mass* Poly6Kernel_Lookup(p->position.Distance(n->position));
			}

			p->density= density;
			p->pressure= p->gas_constant* (p->density- p->rest_density);
		}
	}

	//Could get better direction sampling by using angle instead of displacement
	//Same for relative velocity
	/*FVector2f ComputeForce_Oracle(FVector<float, 6> input_vector)
	{
		FVector2f force;

		FVector2f displacement;
		displacement[0]= input_vector[0];
		displacement[1]= input_vector[1];
		float pressure= input_vector[2];
		FVector2f relative_velocity;
		relative_velocity[0]= input_vector[3];
		relative_velocity[1]= input_vector[4];
		float viscosity= input_vector[5];

		FVector2f attraction_vector= displacement.Normalized();
		float distance= min(pressure_radius, displacement.Magnitude());//this is because of grid sampling issues
		float pressure_magnitude= max(0.0f, pressure* (SpikyKernel_Derivative(distance, pressure_radius)/ -1));
		float viscosity_magnitude= min(0.0f, (viscosity* ViscosityKernel_SecondDerivative(distance, viscosity_radius)* -1));
		FVector2f viscosity_force= relative_velocity* viscosity_magnitude;

		force= (attraction_vector* pressure_magnitude)+ viscosity_force;

		return force;
	}

	FVector2f ComputeForce_Lookup(FVector2f displacement, float pressure, FVector2f relative_velocity, float viscosity)
	{
		FVector<float, 6> input_vector;
		input_vector[0]= displacement[0];
		input_vector[1]= displacement[1];
		input_vector[2]= pressure;
		input_vector[3]= relative_velocity[0];
		input_vector[4]= relative_velocity[1];
		input_vector[5]= viscosity;

		return compute_force_lookup_table->Lookup(input_vector);
	}*/

	float foo= 0;
	void ParticlePhysicsSystem::ComputeAcceleration(Particle *p)
	{
		static float max_pressure= 0.0f;

		if(!p->static_); else
			return;

		static FVector2f center= MakeFVector2f(0.001f, 0.001f);
		FVector2f force;//you could try static as an experiment
		force.ZeroOut();

		FVector2f color_field_gradient;
		color_field_gradient.ZeroOut();
		float color_field_laplacian_magnitude= 0;
		/*for(unsigned int i= 0; i< p->neighbors.size(); i++)
		{
			Particle *n= p->neighbors[i];

			float partner_weight= n->mass/ n->density;
			float distance= p->position.Distance(n->position);

			if(p!= n)
			{
				color_field_gradient+= (p->position- n->position).Normalized()* partner_weight* Poly6Kernel_Derivative(distance);
			}
			color_field_laplacian_magnitude+= partner_weight* Poly6Kernel_SecondDerivative(distance);
		}*/

		float weight= p->mass / p->density;
		for(unsigned int i= 0; i< p->force_partners.size(); i++)
		{
			//non-symmetric

			Particle *n= p->neighbors[i];

			//including self

			float partner_weight= n->mass/ n->density;
			FVector2f displacement= (p->position- n->position);
			float distance= displacement.Magnitude();
			color_field_laplacian_magnitude+= partner_weight* Poly6Kernel_SecondDerivative_Lookup(distance);

			if(!(n== p)); else
				continue;

			//exlcuding self

			//can do less computation if we divide by distance, etc
			FVector2f attraction_vector= displacement/ distance;//curious whether this creates two vectors or one
			color_field_gradient+= attraction_vector* partner_weight* Poly6Kernel_Derivative_Lookup(distance);

			if(p->force_partners[i]== nullptr)
				continue;

			//symmetric

			FVector2f partner_force;
			partner_force.ZeroOut();

			//Pressure
			//max_pressure= max(max_pressure, (p->pressure+ n->pressure)/ 2);
			float pressure_magnitude= max(0.0f, (p->pressure+ n->pressure)* (SpikyKernel_Derivative(distance)/ -2));
			partner_force+= (attraction_vector* (pressure_magnitude));

			//Viscosity
			partner_force+= (p->velocity- n->velocity)* (((p->GetViscosity()+ n->GetViscosity())/ 2) * ViscosityKernel_SecondDerivative(distance)* -1);


			force+= partner_force* partner_weight;
			n->acceleration+= partner_force* (-weight/ n->mass);

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
		float color_field_gradient_magnitude= color_field_gradient.Magnitude();
		p->foo= 0.0f;
		if(color_field_laplacian_magnitude> 50)
			color_field_laplacian_magnitude= 50+ (color_field_laplacian_magnitude- 50)/ 2;
		p->normal= color_field_gradient* -color_field_laplacian_magnitude;
		p->foo= 0.0f+ 1.0f* (color_field_laplacian_magnitude* (color_field_gradient_magnitude* 0.95f)+ 0.05f)/ 90.0f;
		if(color_field_gradient_magnitude> 0.4f)
		{
			//p->foo= color_field_laplacian_magnitude/ 200.0f;
			//p->foo= color_field_gradient_magnitude/ 2.0f;
			force+= color_field_gradient* ((p->tension* color_field_laplacian_magnitude/ color_field_gradient_magnitude)* ((color_field_gradient_magnitude* 0.95f)+ 0.05f));
		}

		//Gravity
		force+= (center- p->position).Normalized()* 0.05f;
		//force+= MakeFVector2f(0.0f, -0.05f);

		//Friction
		force+= p->velocity* -0.0005f;


		//if(foo< 100);else
		//	cout << max_pressure << std::endl;

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

	void ParticlePhysicsSystem::ComputeHeatTransfer(Particle *p)
	{
		float heat_force= 0.0f;
		for (unsigned int j = 0; j < p->neighbors.size(); j++)
		{
			Particle *n= p->neighbors[j];

			heat_force+= p->conduction* n->conduction* (n->heat- p->heat)* (p->mass/ p->density)* Poly6Kernel_Lookup(p->position.Distance(n->position));
		}
		p->heat_delta+= heat_force/ p->mass;
	}

	void ParticlePhysicsSystem::Collide(Particle *p)
	{
		FVector2f center= MakeFVector2f(0.0f, 19.0f);
		float radius= 38.0f;

		FVector2f displacement= center- p->position;
		if(abs(displacement[0])> radius)
			displacement= MakeFVector2f(0.0f, radius+ 1);
		float distance= displacement.Magnitude();
		FVector2f direction= displacement/ distance;
		FVector2f tangent= MakeFVector2f(direction[1]* -1, direction[0]);
		if((direction[1]> 0) && distance> radius)
		{
			p->position+= direction* (distance- radius);
			p->velocity-= direction* direction.Dot(p->velocity)* 2;
			p->velocity-= tangent* tangent.Dot(p->velocity)* 0.05f;

		}


	}

	ParticlePhysicsSystem::ParticlePhysicsSystem()
	{
		acceleration_grid= new AccelerationGrid(MakeFVector2f(-100.0f, -100.0f), 1.0f, 200);

		if(false)
		for(int i= -20; i<= 20; i++)
		{
			for(int j= -20; j<= 20; j++)
			{
				Particle *p= new Particle(MakeFVector2f(i/ 2.0f, j/ 2.0f));
				particles.push_back(p);
				acceleration_grid->AddParticle(p);
			}
		}

		if(false)
		for(int i= -15; i<= -13; i++)
		{
			for(int j= 60; j<= 120; j++)
			{
				Particle *p= new Particle(MakeFVector2f(i/ 1.5f, j/ 1.5f));
				p->static_= true;
				p->velocity= MakeFVector2f(-0.1f, -1.5f);
				p->mass= 1.0f;
				p->gas_constant/= p->mass;
				p->rest_density/= p->mass;
				p->heat= 100.0f;
				particles.push_back(p);
				acceleration_grid->AddParticle(p);
			}
		}

		if(false)
		for(int i= 2; i<= 2; i++)
		{
			for(int j= -260; j<= -50; j++)
			{
				Particle *p= new Particle(MakeFVector2f(i/ 2.0f, j/ 2.0f));
				p->static_= true;
				p->velocity= MakeFVector2f(0.0f, 0.0f);
				p->mass= 1.0f;
				p->gas_constant/= p->mass;
				p->rest_density/= p->mass;
				p->heat= 0.0f;
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
		static float last_spawn= 0.0f;

		FVector2f force;//opt
		for(int step= 0; step< step_count; step++)
		{
			/*for(unsigned int i= 0; i< particles.size(); i++)
				if(foo> 62.0f && foo< 62.3f)
					particles[i]->static_= false;*/

			if((foo- last_spawn)> 0.1f)
			{
				Particle *p= new Particle(MakeFVector2f(-20.0f+ (rand()% 500)/ 100.0f, 60));
				p->velocity= MakeFVector2f(0.0f, -1.0f);
				particles.push_back(p);
				acceleration_grid->AddParticle(p);
				last_spawn= foo;
			}

			acceleration_grid->UpdateGrid(particles);
			UpdateParticleProperties();

			for(unsigned int i= 0; i< particles.size(); i++)
			{
				ComputeAcceleration(particles[i]);
				//ComputeHeatTransfer(particles[i]);
			}
			//particles[260]->heat_delta+= 1.0f;

			for(unsigned int i= 0; i< particles.size(); i++)
				particles[i]->Step(total_time_step/ step_count);

			//for(unsigned int i= 0; i< particles.size(); i++)
			//	Collide(particles[i]);

			foo+= total_time_step/ step_count;
		}
	}

	void InitializeLookups()
	{
		{
			//FVector<float, 6> low; low[0]=-0.9999f; low[1]= -0.9999; low[2]= 0; low[3]= -6.9999f; low[4]= -6.9999f; low[5]= 0.45f;
			//FVector<float, 6> high; high[0]= 1; high[1]= 1; high[2]= 10.5f; high[3]= 7; high[4]= 7; high[5]= 0.55f;
			//compute_force_lookup_table= new ComputeForceLookupTable(ComputeForce_Oracle, low, high);
		}

		{
			FVector<float, 1> low; low[0]= 0.0f;
			FVector<float, 1> high; high[0]= kernel_radius;
			poly6_kernel_lookup_table= new Poly6LookupTable(Poly6Kernel_Oracle, low, high);
		}

		{
			FVector<float, 1> low; low[0]= 0.0f;
			FVector<float, 1> high; high[0]= kernel_radius;
			poly6_kernel_derivative_lookup_table= new Poly6DerivativeLookupTable(Poly6Kernel_Derivative_Oracle, low, high);
		}

		{
			FVector<float, 1> low; low[0]= 0.0f;
			FVector<float, 1> high; high[0]= kernel_radius;
			poly6_kernel_second_derivative_lookup_table= new Poly6SecondDerivativeLookupTable(Poly6Kernel_SecondDerivative_Oracle, low, high);
		}

		{
			FVector<float, 1> low; low[0]= 0.0f;
			FVector<float, 1> high; high[0]= kernel_radius;
			spiky_kernel_derivative_lookup_table= new SpikyDerivativeLookupTable(SpikyKernel_Derivative_Oracle, low, high);
		}
	}

	void Initialize()
	{
		particle_physics_system= new ParticlePhysicsSystem();
		InitializeLookups();
	}

	void Update()
	{
		particle_physics_system->Simulate(0.0333f* 2.0f, 1);
	}

	void Conclude()
	{
		delete poly6_kernel_lookup_table;
		delete particle_physics_system;
	}

}