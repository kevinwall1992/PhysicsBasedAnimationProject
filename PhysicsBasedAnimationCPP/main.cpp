#include "Graphics.h"
#include "Timing.h"
#include "Physics.h"

#include <iostream>

void Initialize()
{
	Timing::Initialize();
	Graphics::Initialize();
	Physics::Initialize();
}

void Conclude()
{
	Physics::Conclude();
	Graphics::Conclude();
	Input::Context::FreeContexts();
}

void Update()
{
	Timing::Update();

	Input::Context::HandleInput();

	Physics::Update();

	if(!Input::CameraContext::IsPaused());

	//Need to move into input somehow; this is not a frame update
	//Although, we could make it a frame update (at least when in real-time mode)
	//By using the elapsed time to determine timeline step size
	//HFS_State::Update();
}

const int frame_wait= 10;

int main (int argument_count, char *arguments[])
{	
	Initialize();

	vector<Particle *> particles;

	//Drawing *drawing= Architect().MakeDrawingOf(HFS_State::GetShip());

	float frame_time_total= 0;
	int frames= 0;
	int total_frames= 0;
	float total_time= 0;

	while (true)
	{
		frame_time_total+= Timing::frame_time_in_seconds;
		frames++;
		total_frames++;
		total_time+= Timing::frame_time_in_seconds;

		if((frames% frame_wait)== 0)
		{
			std::cout << "FC: " << frames << " FPS: " << 1/ (frame_time_total/ frame_wait) << " Frames: " << total_frames << std::endl;
			
			//if(false)
			if(total_frames> 2060)
			{
				std::cout << "total time: " << total_time;
				//SDL_Delay(5000);
				break;
			}

			frame_time_total= 0;
			frames= 0;
		}

		Update();
		if(Input::Context::GameOverRequested())
			break;

		//std::cout << Physics::particle_physics_system->GetParticles()[47]->density << std::endl;
		Graphics::Render(Physics::particle_physics_system->GetParticles());
		//SDL_Delay(100);
	}

	Conclude();

	return 0;
}