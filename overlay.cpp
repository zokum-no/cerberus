#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "includes/GLWrapper.h"

#include "includes/main.h"
#include "includes/overlay.h"

/*
void overlay_squarefade(DWORD progress, float x1, float y1, float x2, float y2, Texture &tex)
{
	progress = progress / 10.0;		// 10-dels fart

	float tid = (float) progress / 1000.0;

	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);							// Velger projection matrix
	glLoadIdentity();										// Setter projection matrix = identity matrix (ingen transformasjon)
	glMatrixMode(GL_MODELVIEW);								// Velger modelview matrix
	glLoadIdentity();										// Setter modelview matrix = identity matrix (ingen transformasjon)

	tex.enable();

	glBegin(GL_QUADS);

	int x = 40;
	int y = 30;

	while (x)
	{
		while (y)
		{
//			glTexCoord2f(1.0 / 80.0 * x)

			y--;
		}
		x--;
	}


	glTexCoord2f(0.5, 0.0);		glVertex3f(xbeina, ybeina, 0.0);
	glTexCoord2f(0.5, 0.5);		glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(0.5 + tid, 0.0);	glVertex3f(xbeinb, ybeinb, 0.0);

	glEnd();

	tex.disable();

	//	printDebug("xa: %1.2f, ya: %1.2f, xb: %1.2f, yb: %1.2f, tid: %1.2f\n ", xbeina, ybeina, xbeinb, ybeinb, tid + 0.5);

}
*/

void overlay(double x1, double y1, double x2, double y2, Texture tex)
{
	overlay_advanced(x1, y1, 0.01, x2, y2, 0.01, tex, 0.0, 0.0, 1.0, 1.0);

}


void overlay_advanced_stretchfade(DWORD progress, int style, double x1, double y1, double z1, double x2, double y2, double z2, Texture tex, double xtex1, double ytex1, double xtex2, double ytex2)
{
	float tid = (float) progress / 1000.0;

	if (style == 1)
	{
		if (  ytex2 <= (ytex2 * tid / 2.0) ) return;

		overlay_advanced(x1, y1, z1, x2, y2 - (y2 * tid) , z2, tex, xtex1, ytex1, xtex2, ytex2 - (ytex2 * tid / 2.0));
		overlay_advanced(x1, y2 - (y2 * tid), z1, x2, y2, z2, tex, xtex1, ytex2 - (ytex2 * tid / 2.0), xtex2, ytex2 - (ytex2 * tid / 2.0) + 0.00001);
	}
}

void overlay_advanced(double x1, double y1, double z1, double x2, double y2, double z2, Texture tex, double xtex1, double ytex1, double xtex2, double ytex2)
{

	

//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Tømmer framebuffer og depth buffer

glMatrixMode(GL_PROJECTION);							// Velger projection matrix
glLoadIdentity();										// Setter projection matrix = identity matrix (ingen transformasjon)
glMatrixMode(GL_MODELVIEW);								// Velger modelview matrix
glLoadIdentity();										// Setter modelview matrix = identity matrix (ingen transformasjon)


//glColor3f(1.0, 1.0, 1.0);
	tex.enable();											// Skrur på teksturen
glBegin(GL_QUADS);								    // Begynner på trekant-tegning

glTexCoord2f(xtex1,ytex1); glVertex3f(x1,y1,z1);		// Første punkt i trekanten (rød)
glTexCoord2f(xtex1,ytex2); glVertex3f(x1,y2,z1);		// Andre punkt i trekanten (grønn)
glTexCoord2f(xtex2,ytex2); glVertex3f(x2,y2,z1);		// Tredje punkt i trekanten (blå)
glTexCoord2f(xtex2,ytex1); glVertex3f(x2,y1,z1);		// Tredje punkt i trekanten (blå)


glEnd();												// Forteller OpenGL at vi er ferdige med å tegne trekanter
tex.disable();

//wnd.update();											// Oppdaterer skjermen med det som har blit tegnet

}
