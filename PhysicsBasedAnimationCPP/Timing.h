#ifndef __RHWS_TIMING__
#define __RHWS_TIMING__

#include <SDL.h>

namespace Timing
{
	extern float frame_time_in_seconds;

	void Initialize();
	void Update();
}

#endif