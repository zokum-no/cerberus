#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "includes/GLWrapper.h"

#include "includes/glfont.h"
//#include <includes/glaux.h>

#include "Cg/cg.h"
#include "Cg/cgGL.h"

#include <math.h>

#include <cstdlib>
#include <ctime>

#include "includes/bass.h"
#include "includes/basswrap.h"

#include "includes/main.h"
/*
#include "vumeter.h" 
#include "loadingscreen.h"
#include "starfield.h"
#include "overlay.h"
#include "linenoice.h"
#include "copperbars.h"
#include "linemorph.h"
#include "vectorslime.h"
*/
void scrolltext(double x, double y, int progress, double speed, char *text)
{
	int i = 0;

	char s[2];
	char t;

	//xprog = 4.0;

	double size = 2;

GLFONT font;

		glEnable(GL_TEXTURE_2D);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (!glFontCreate(&font, "demofont.glf", 0))
			return;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 1024, 0, 768, -1, 1);

		glFontBegin(&font);
		glScalef(18.0 * size, 18.0 * size , 18.0 * size);

		double offset = 0.0;

	while (text[i] != '\0')
	{
		double xprog = x - (double) progress / speed;
		double yprog = y - sin( ((double)(progress + i*18) / 100));

		sprintf(s,"%c",text[i]);

		glFontTextOut(s, xprog + ((i * 1.2) + offset), yprog, 0);

		if		(s[0] == 'i')	offset = offset - 0.8;
		else if (s[0] == '1')	offset = offset - 0.7;
		else if (s[0] == 't')	offset = offset - 0.2;
		else if (s[0] == '\'')	offset = offset - 0.6;
		else if (s[0] == 'w')	offset = offset + 0.26;
		else if (s[0] == 'M')	offset = offset + 0.26;
		else if (s[0] == 'I')	offset = offset - 0.75;
		else if (s[0] == ':')	offset = offset - 0.75;
		else if (s[0] == '-')	offset = offset - 0.5;
		else if (s[0] == '|')	offset = offset - 0.84;


		


			
		i++;
	}
	glFontEnd();
	glFlush();

	//displaytext(0, 10, 2.2, s);

}

void displaytext(double x, double y, double size, char *text)
{
	GLFONT font;

	glMatrixMode(GL_PROJECTION);							// Velger projection matrix
	glLoadIdentity();										// Setter projection matrix = identity matrix (ingen transformasjon)
	glMatrixMode(GL_MODELVIEW);								// Velger modelview matrix
	glLoadIdentity();										// Setter modelview matrix = identity matrix (ingen transformasjon)

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	if (!glFontCreate(&font, "demofont.glf", 0))
		return;
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	glOrtho(0, 1024, 0, 768, -1, 1);

	glFontBegin(&font);
	glScalef(18.0 * size, 18.0 * size , 18.0 * size);
	//			x, fra høyre, y fra bunn, z ubrukt
	glTranslatef(5,6,0);
	glFontTextOut(text, x, y, 0);
	glFontEnd();
	
	glMatrixMode(GL_PROJECTION);							// Velger projection matrix
//glLoadIdentity();										// Setter projection matrix = identity matrix (ingen transformasjon)
//glMatrixMode(GL_MODELVIEW);								// Velger modelview matrix
	glLoadIdentity();		
	glFlush();


}