#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "includes/GLWrapper.h"

/*
#include "glfont.h"
#include <gl\glaux.h>

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
#include "includes/main.h"
// linemorph(curtime, 2000, 1, 1.0f);
void linemorph(DWORD progress, int drawing)
{
	float SPI = 6.28;

	DWORD morphtime = 2000;

//	progress = progress / 2;

	if (morphtime < progress)
		return;

	/*
	2 figurer

	6 x-punkter
	6 y-punkter

	*/
	float figure_a[6][2];
	float figure_b[6][2];
	float figure_c[6][2];

	float source_scale;
	float dest_scale;


	if (drawing == 1)
	{
		source_scale = 0.8;
		dest_scale = 0.4;
	}
	else if (drawing == 2)
	{
		source_scale = 0.4;
		dest_scale = 0.9;
	}
	else if (drawing == 3)
	{
		source_scale = 0.9;
		dest_scale = 0.4;
	}
	else	return;


	figure_a[0][0] = 0.0;
	figure_a[0][1] = 0.5;

	figure_a[1][0] =  0.5;
	figure_a[1][1] =  0.0;

	figure_a[2][0] = 1.0;
	figure_a[2][1] = -0.5;

	figure_a[3][0] =  0.0;
	figure_a[3][1] =  -0.5;

	figure_a[4][0] = -1.0;
	figure_a[4][1] = -0.5;

	figure_a[5][0] =  -0.5;
	figure_a[5][1] =  0.0;

	figure_b[0][0] = sin(SPI / 6.0 * 2.0);
	figure_b[0][1] = cos(SPI / 6.0 * 2.0);

	figure_b[1][0] = sin(SPI / 6.0 * 3.0);
	figure_b[1][1] = cos(SPI / 6.0 * 3.0);

	figure_b[2][0] = sin(SPI / 6.0 * 4.0);
	figure_b[2][1] = cos(SPI / 6.0 * 4.0);

	figure_b[3][0] = sin(SPI / 6.0 * 5.0);
	figure_b[3][1] = cos(SPI / 6.0 * 5.0);

	figure_b[4][0] = sin(SPI / 6.0 * 6.0);
	figure_b[4][1] = cos(SPI / 6.0 * 6.0);

	figure_b[5][0] = sin(SPI / 6.0 * 1.0);
	figure_b[5][1] = cos(SPI / 6.0 * 1.0);

	
	
	figure_c[0][0] = -0.5;
	figure_c[0][1] = 0.5;

	figure_c[1][0] = 0.5;
	figure_c[1][1] = 0.5;

	figure_c[2][0] = 0.1;
	figure_c[2][1] = 0.0;

	figure_c[3][0] = 0.5;
	figure_c[3][1] = -0.5;

	figure_c[4][0] = -0.5;
	figure_c[4][1] = -0.5;

	figure_c[5][0] = -0.1;
	figure_c[5][1] = 0.0;


	float tid = (float) progress / (float) morphtime;

	int vertices = 0;

	float xdiff;
	float ydiff;

	float x1;
	float x2;
	float y1;
	float y2;

	glBegin(GL_LINE_LOOP);
	while (vertices != 6)
	{	
		if (drawing == 1)
		{
			xdiff = (figure_b[vertices][0] * dest_scale) - (figure_a[vertices][0] * source_scale);
			ydiff = (figure_b[vertices][1] * dest_scale) - (figure_a[vertices][1] * source_scale);
			glVertex3f(((figure_a[vertices][0] * source_scale) + (xdiff * tid)), 
						(figure_a[vertices][1] * source_scale) + (ydiff * tid), -1.0f);
		}
		else if (drawing == 2)
		{
			xdiff = (figure_c[vertices][0] * dest_scale) - (figure_b[vertices][0] * source_scale);
			ydiff = (figure_c[vertices][1] * dest_scale) - (figure_b[vertices][1] * source_scale);
			glVertex3f(((figure_b[vertices][0] * source_scale) + (xdiff * tid)), 
						(figure_b[vertices][1] * source_scale) + (ydiff * tid), -1.0f);
		}
		else if (drawing == 3)
		{
			xdiff = (figure_b[vertices][0] * dest_scale) - (figure_c[vertices][0] * source_scale);
			ydiff = (figure_b[vertices][1] * dest_scale) - (figure_c[vertices][1] * source_scale);
			glVertex3f(((figure_c[vertices][0] * source_scale) + (xdiff * tid)), 
						(figure_c[vertices][1] * source_scale) + (ydiff * tid), -1.0f);
		}
		vertices++;
	}
	glEnd();
}