#include "Timing.h"

namespace Timing
{
	float frame_time_in_seconds= 0;

	unsigned int last_ticks;

	void Initialize()
	{
		last_ticks= SDL_GetTicks();
	}

	void Update()
	{
		unsigned int this_ticks= SDL_GetTicks();

		frame_time_in_seconds= (this_ticks- last_ticks)/ 1000.0f;

		last_ticks= this_ticks;
	}
}