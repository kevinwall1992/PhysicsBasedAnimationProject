//Graphics
//Contains and defines scene information
//Initializes SDL
//Handles changing of resolution, and taking screenshots

#ifndef __RHWS_GRAPHICS__
#define __RHWS_GRAPHICS__

#include "Math_.h"
#include "GraphicsHeaders.h"
#include "Input.h"

#include "Particle.h"

#include <vector>
using std::vector;

namespace Graphics
{
	static const int DEFAULT_SCREEN_WIDTH= 1000;
	static const int DEFAULT_SCREEN_HEIGHT= 600;

	extern unsigned int current_screen_width;
	extern unsigned int current_screen_height;
	extern float current_aspect_ratio;

	void Initialize();
	void Conclude();
	void ChangeResolution(unsigned int screen_width, unsigned int screen_height);
	void SaveScreenToFile();
	float GetCurrentAspectRatio();

	void Render(vector<Particle *> particles);
}

#endif