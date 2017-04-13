#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
/*#include "GLWrapper.h"

#include "glfont.h"
*/

//#include <gl\glaux.h>
/*
#include "Cg/cg.h"
#include "Cg/cgGL.h"
*/
#include <math.h>

#include <cstdlib>
#include <ctime>
/*
#include "bass.h"
#include "basswrap.h"
*/
/*
#include "includes/main.h"
#include "vumeter.h" 
#include "loadingscreen.h"
#include "starfield.h"
#include "overlay.h"
#include "linenoice.h"
#include "copperbars.h"
#include "linemorph.h"
#include "vectorslime.h"
*/

void tunnel_init(float rings[2][800], int r_cnt)
{
	for (int i = 0; i != r_cnt ; i++)
	{
		rings[0][i] = (float) i * 0.1;
		rings[1][i] = (float) i * 0.1;
	}
}

void tunnel(int progress, float rings[2][800], int r_cnt, bool fadein, int style, float tintr, float tintg, float tintb)
{
	char text[256];

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, 1.333f, 0.01, 1900);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0.1, 0,0,-2.2, 0,1,0);

	float fx, fy, fz, color;
	int dots = 50;
	
	fz = -1.0;

	for (int i = 0; i != r_cnt ; i++)
	{
		rings[0][i] = rings[1][i] - ((float) progress / 1500.0);
	}

	/*
	if (rings[r_cnt-1] > 1.0)
	{
		for (int i = 0; i != r_cnt ; i++)
		{
			rings[r_cnt-i] = rings[r_cnt - i - 1 ];
		}
		rings[0] = 0;
	}
*/

	int ring_nr = r_cnt;
	
	while (ring_nr)
	{
		glColor3f(1, 1, 1);
		dots = 50;

		fz = rings[0][ring_nr] * -1.9;
	
		float pointsize = (10.0 - (fz * 4.0 * -1) );

		if (pointsize < 1.0) pointsize = 1.0;

		glPointSize(pointsize);

		float point_color = 1.0 - (rings[0][ring_nr]) * 0.4 ;

		if (fadein && progress < 4000)
			point_color = point_color * ((float) progress / 4000.0);

		glColor3f(point_color * tintr, point_color * tintg, point_color * tintb);

		glBegin(style);
		while (dots)
		{
			fx = sin(( (float)dots / 50.0) * 6.28) * 0.5;
			fy = cos(( (float)dots / 50.0) * 6.28) * 0.5;
			glVertex3f(fx, fy, fz);
			dots--;
		}
		glEnd();
		ring_nr--;
	}
	glColor3f(1.0,1.0,1.0);
}