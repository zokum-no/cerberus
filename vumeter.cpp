#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "includes/GLWrapper.h"

#include "includes/glfont.h"
// #include <gl\glaux.h>

#include "Cg/cg.h"
#include "Cg/cgGL.h"

#include <math.h>

#include <cstdlib>
#include <ctime>

using namespace GLWrapper;

#include "includes/bass.h"
#include "includes/basswrap.h"

#include "includes/main.h"
#include "includes/vumeter.h"

extern float sync[512];
extern float oldsync[512];
extern float ancientsync[512];



void vumeter(double x1, double y1, double x2, double y2, int bars, Texture tex, int cutoff)
{
	double yfactor;

	tex.enable();											// Skrur på teksturen
	glBegin(GL_QUADS);								    // Begynner på 4kant-tegning

	int i = 0;
	
	if (y1 > 0)
	{
		yfactor = y1 - y2;
	}
	else
	{
		yfactor = y1 + y2;
	}

	bars++;

	double xwidth = x2 - x1;
	double ywidth = y1 - y2;
				
	double barwidth = xwidth / (double) (bars + 1);

	double barheightmin = ywidth * 0.1;

	double spacer = barwidth * 0.1;
	
	int res = cutoff / bars - 1;
	//res = 4;

	double oldy = y1;
	double gain = 18;
	double yscaler;

	while (i != bars + 1)
	{
		if (i == 0) sync[0] = sync[0] * 0.10;
		if (i == 1) sync[1 * res] = sync[0] * 0.35;
		if (i == 2) sync[2 * res] = sync[0] * 0.45;
		if (i == 3) sync[3 * res] = sync[0] * 0.55;

		if (sync[i * res] > oldsync[i * res])
		{
			oldsync[i * res] = sync[i * res];	
			y1 = y2 + (double) sync[i * res] * gain;
		}
		else
		{
			oldsync[i * res] = oldsync[i * res] * 0.97;
			y1 = y2 + (double) oldsync[i * res] * gain;
		}

		if (y1 > oldy) y1 = oldy;

		yscaler = 1 - (y1 - y2) / yfactor;

		// lager det mer digitalt

		glTexCoord2f(0,0 + yscaler); glVertex3f(x1 + (i * barwidth),y1, 0.1f);		// Første punkt i trekanten (rød)
		glTexCoord2f(0,1); glVertex3f(x1 + (i * barwidth),y2, 0.1f);		// Andre punkt i trekanten (grønn)
		glTexCoord2f(1,1); glVertex3f(x1 + ((i+1) * barwidth - spacer),y2, 0.1f);		// Tredje punkt i trekanten (blå)
		glTexCoord2f(1,0 + yscaler); glVertex3f(x1 + ((i+1) * barwidth - spacer),y1, 0.1f);		// Tredje punkt i trekanten (blå)
		
		i++;
	}

	char debugstring[256];

//	sprintf(debugstring,"Yfactor: %1.2f %1.2f", yfactor, yscaler); 

//	displaytext(-2,-2.5,1.5,debugstring);
	glEnd();												// Forteller OpenGL at vi er ferdige med å tegne trekanter
	tex.disable();

// displaytext(1,5.5,1.5,debugstring);
}
