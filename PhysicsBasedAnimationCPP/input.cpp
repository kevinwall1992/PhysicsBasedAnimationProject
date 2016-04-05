#include "Input.h"
#include "Timing.h"
#include "Point.h"
#include "Ray.h"

#include <iostream>

namespace Input
{
	vector<Context *> Context::complete_contexts_list= vector<Context *>();

	vector<Context *> Context::active_contexts= vector<Context *>();
	bool Context::game_over_requested= false;

	int Context::key_count;
	const Uint8 * Context::keys= SDL_GetKeyboardState(&key_count);

	Uint8 Context::mouse_state= 0;

	Point2i Context::mouse_screen_coordinates= Point2i();
	Point2i Context::mouse_screen_coordinates_last_frame= Point2i();
	Point2i Context::mouse_screen_displacement= Point2i();

	//This handles updating all the varaibles that are common among the various contexts
	void Context::UpdateCommonVariables()
	{
		SDL_PumpEvents();

		mouse_screen_coordinates_last_frame= mouse_screen_coordinates;

		int mouse_x, mouse_y;
		mouse_state= SDL_GetMouseState(&mouse_x, &mouse_y);
		mouse_screen_coordinates= Point2i(mouse_x, mouse_y);

		mouse_screen_displacement= mouse_screen_coordinates- mouse_screen_coordinates_last_frame;

		//mouse_world_ray= GetMouseWorldRay();
	}


	void Context::AddToCompleteContextsList(Context *context)
	{
		complete_contexts_list.push_back(context);
	}


	Context::Context()
	{

	}

	void Context::OnSDLQuit()
	{
		game_over_requested= true;
	}

	void Context::OnMouseEvent(){};
	void Context::OnMouseDown_Left(){};
	void Context::OnMouseDown_Right(){};
	void Context::OnMouseDown_Wheel(){};
	void Context::OnMouseUp_Left(){};
	void Context::OnMouseUp_Right(){};
	void Context::OnMouseUp_Wheel(){};
	void Context::OnMouseMotion(Point2i mouse_screen_displacement){};
	void Context::OnMouseWheelScroll(int mouse_scroll_value){};

	void Context::OnKeyDown(SDL_Keycode sdl_key){};
	void Context::OnKeyUp(SDL_Keycode sdl_key){};

	void Context::OnMouseIsDown_Left(){};
	void Context::OnMouseIsDown_Right(){};
	void Context::OnMouseIsDown_Wheel(){};

	void Context::OnKeyIsDown(SDL_Keycode sdl_key){};

	void Context::HandleSDLEvent(SDL_Event event_)
	{
		//if(event_.type== SDL_MOUSEMOTION || event_.type == 1 ||  event_.type == 0x00000011 || event_.type == 0x00000010)
		//	return;

		switch (event_.type) 
		{
			case SDL_QUIT:			
				OnSDLQuit();
				break;

			case SDL_KEYDOWN:
				OnKeyDown(event_.key.keysym.sym);
				break;

			case SDL_KEYUP:
				OnKeyUp(event_.key.keysym.sym);
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
				HandleSDLMouseEvent(event_);
			
		}
	}

	void Context::HandleSDLMouseEvent(SDL_Event event_)
	{
		switch(event_.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch(event_.button.button)
				{
					case SDL_BUTTON_LEFT: OnMouseDown_Left(); break;
					case SDL_BUTTON_RIGHT: OnMouseDown_Right(); break;
					case SDL_BUTTON_MIDDLE: OnMouseDown_Wheel(); break;
				}

				break;

			case SDL_MOUSEBUTTONUP:
				switch(event_.button.button)
				{
					case SDL_BUTTON_LEFT: OnMouseUp_Left(); break;
					case SDL_BUTTON_RIGHT: OnMouseUp_Right(); break;
					case SDL_BUTTON_MIDDLE: OnMouseUp_Wheel(); break;
				}

				break;

			case SDL_MOUSEWHEEL:
				OnMouseWheelScroll(event_.wheel.y);
				break;

			case SDL_MOUSEMOTION:
				OnMouseMotion(mouse_screen_displacement);

				break;
		}
	}

	bool Context::GameOverRequested()
	{
		return game_over_requested;
	}

	void Context::HandleInput()
	{
		UpdateCommonVariables();

		HandleSDLEvents();

		for(unsigned int i= 0; i< active_contexts.size(); i++)
			active_contexts[i]->HandleGameState();
	}

	void Context::HandleSDLEvents()
	{
		SDL_Event event_;

		while(SDL_PollEvent(&event_))
		{
			for(unsigned int i= 0; i< active_contexts.size(); i++)
				active_contexts[i]->HandleSDLEvent(event_);
		}
	}

	void Context::HandleGameState()
	{
		if(mouse_state& SDL_BUTTON_LEFT)
			OnMouseIsDown_Left();

		if(mouse_state& SDL_BUTTON_RIGHT)
			OnMouseIsDown_Left();

		if(mouse_state& SDL_BUTTON_MIDDLE)
			OnMouseIsDown_Wheel();


		for(int i= 0; i< key_count; i++)
			if(keys[i])
				OnKeyIsDown(SDL_GetKeyFromScancode((SDL_Scancode)i));
	}

	void Context::FreeContexts()
	{
		for(unsigned int i= 0; i< active_contexts.size(); i++)
			delete complete_contexts_list[i];
	}


	CameraContext * CameraContext::static_instance= new CameraContext();

	void CameraContext::OnMouseUp_Left()
	{
		
	}

	void CameraContext::OnMouseUp_Right()
	{
		
	}

	void CameraContext::OnMouseMotion(Point2i mouse_screen_displacement)
	{
		if(mouse_state& SDL_BUTTON_MIDDLE)
		{
			//camera->yaw-= DegreesToRadians(mouse_screen_displacement.GetX()/ 1.0f);
			//camera->pitch+= DegreesToRadians(mouse_screen_displacement.GetY()/ 1.0f);
		}
	}

	bool is_paused= false;
	void CameraContext::OnKeyUp(SDL_Keycode sdl_key)
	{
		switch(sdl_key)
		{
		case SDLK_q: 
			break;

		case SDLK_SPACE:
			break;
		}
	}

	void CameraContext::OnKeyIsDown(SDL_Keycode sdl_key)
	{
		switch(sdl_key)
		{
		case SDLK_a: 
			break;
		}
	}

	void CameraContext::HandleGameState()
	{
		Context::HandleGameState();
	}


	CameraContext::CameraContext()
	{
		Context::AddToCompleteContextsList(this);
		Context::active_contexts.push_back(this);
	}

	bool CameraContext::IsPaused()
	{
		return is_paused;
	}


	Point2i GetMouseScreenCoordinates()
	{
		int x, y;

		SDL_GetMouseState(&x, &y);

		return Point2i(x, y);
	}

	HFSContext * HFSContext::static_instance= new HFSContext();

	HFSContext::HFSContext()
	{
		Context::AddToCompleteContextsList(this);
		Context::active_contexts.push_back(this);
	}

	void HFSContext::OnKeyDown(SDL_Keycode sdl_key)
	{
		
	}
}
