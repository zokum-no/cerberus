/*

Outracks GLWrapper
Tiny OpenGL wrapping framework for demo use.

You are free to redistribute, change and use this code without charge.
The code is provided "as is", with no warranties or support.

*/


#include "includes/GLWRAPPER.H"

namespace GLWrapper
{
	Timer::Timer()
	{
		//loadPoints();
		QueryPerformanceFrequency(&frequency);
		factor = 1.0f / frequency.QuadPart;
		restart();
	}

	Timer::~Timer()
	{

	}

	void Timer::restart()
	{
		startG = getG();
	}

	float Timer::getG()
	{
		QueryPerformanceCounter(&currentTime);
		return ((float)currentTime.QuadPart * factor);
	}
	float Timer::getTime()
	{
		g = (float)getG()-startG;
		return g;
	}

	float Timer::getInterval()
	{
		return float(((float)getG()-startG) - g);
	}
}