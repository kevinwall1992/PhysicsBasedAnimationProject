#ifndef __RHWS_INPUT__
#define __RHWS_INPUT__

#include "Graphics.h"
#include "Point.h"
#include <vector>

using std::vector;

namespace Input
{
	struct ContextID{enum Enum {DesignContext, CameraContext};};

	class Context
	{
		static vector<Context *> complete_contexts_list;

		static void UpdateCommonVariables();
		static void HandleSDLEvents();
	protected:
		static vector<Context *> active_contexts;
		static bool game_over_requested;

		static int key_count;
		static const Uint8 *keys;

		static Uint8 mouse_state;

		static Point2i mouse_screen_coordinates;
		static Point2i mouse_screen_coordinates_last_frame;
		static Point2i mouse_screen_displacement;


		static void AddToCompleteContextsList(Context *context);


		Context();

		//The following functions (until next comment) do NOT implement functionality
		//Therefore, you do NOT need to call the parent function when overriding

		//Events
		virtual void OnMouseEvent();
		virtual void OnMouseDown_Left();
		virtual void OnMouseDown_Right();
		virtual void OnMouseDown_Wheel();
		virtual void OnMouseUp_Left();
		virtual void OnMouseUp_Right();
		virtual void OnMouseUp_Wheel();
		virtual void OnMouseMotion(Point2i mouse_screen_displacement);
		virtual void OnMouseWheelScroll(int mouse_scroll_value);
	
		virtual void OnKeyDown(SDL_Keycode sdl_key);
		virtual void OnKeyUp(SDL_Keycode sdl_key);

		//States
		virtual void OnMouseIsDown_Left();
		virtual void OnMouseIsDown_Right();
		virtual void OnMouseIsDown_Wheel();

		virtual void OnKeyIsDown(SDL_Keycode sdl_key);

		//These functions, on the other hand, DO implement functionality
		//And the intent is, if you override, to call the parent function, 
		//unless you are explicitely trying to completely alter the implementation
		virtual void OnSDLQuit();

		virtual void HandleSDLEvent(SDL_Event event_);
		virtual void HandleSDLMouseEvent(SDL_Event event_);
		virtual void HandleGameState();

	public:
		static bool GameOverRequested();

		static ContextID::Enum GetActiveContextIDs();
		static void HandleInput();
		static void FreeContexts();
	};


	class CameraContext: public Context
	{
		static CameraContext *static_instance;

	protected:
		void OnMouseUp_Left();
		void OnMouseUp_Right();
		void OnMouseMotion(Point2i mouse_screen_displacement);
		void OnKeyUp(SDL_Keycode sdl_key);
		void OnKeyIsDown(SDL_Keycode sdl_key);

		void HandleGameState();

	public:
		CameraContext();

		static bool IsPaused();
	};

	class HFSContext: public Context
	{
		static HFSContext *static_instance;

	protected:
		void OnKeyDown(SDL_Keycode sdl_key);

	public:
		HFSContext();
	};

	Point2i GetMouseScreenCoordinates();

}

#endif