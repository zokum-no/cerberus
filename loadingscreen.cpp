#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "includes/GLWrapper.h"

#include "includes/glfont.h"
#include "includes/glaux.h"	

#include "Cg/cg.h"
#include "Cg/cgGL.h"

#include <math.h>

#include <cstdlib>
#include <ctime>

#include "includes/bass.h"
#include "includes/basswrap.h"

#include "includes/main.h"
#include "includes/vumeter.h" 
#include "includes/loadingscreen.h"
#include "includes/overlay.h"

/* noen statiske ting */

using namespace GLWrapper;

extern Window wnd;

progresscounter *newcounter(float xstart, float ystart, float xend, float yend, float width, int entries, char *title, char *subtitle)
{
	progresscounter *progcnt = new progresscounter;

	progcnt->entries = entries;
	progcnt->progress = 0;
	progcnt->xend = xend;
	progcnt->xstart = xstart;
	progcnt->yend = yend;
	progcnt->ystart = ystart;
	progcnt->width = width;

	progcnt->barlength = abs(xstart) + abs(xend); // buggy, i know for den forustetter de er på - og +

	progcnt->title = _strdup(title);
	progcnt->subtitle = _strdup (subtitle);

	return progcnt;
}


void progressbar(progresscounter *prgcnt)
{
	
	glMatrixMode(GL_PROJECTION);							// Velger projection matrix
	glLoadIdentity();										// Setter projection matrix = identity matrix (ingen transformasjon)
	glMatrixMode(GL_MODELVIEW);								// Velger modelview matrix
	glLoadIdentity();										// Setter modelview matrix = identity matrix (ingen transformasjon)

	float progress = (float) (float) prgcnt->progress * (prgcnt->barlength / (float) prgcnt->entries);

//	char debugstring[256];
//	sprintf(debugstring,"Prog: %d, Barlen: %1.2f, Progress: %1.2f", prgcnt->progress, prgcnt->entries, prgcnt->barlength, progress); 

//	displaytext(-2,-1.5,1.5,debugstring);


	glBegin(GL_QUADS);								    // Begynner på trekant-tegning

	// øvre strek
	glVertex3f(prgcnt->xstart, prgcnt->ystart, 0);
	glVertex3f(prgcnt->xstart, prgcnt->ystart - prgcnt->width, 0);
	glVertex3f(prgcnt->xend, prgcnt->ystart - prgcnt->width, 0);
	glVertex3f(prgcnt->xend, prgcnt->ystart, 0);

	// nedre strek
	glVertex3f(prgcnt->xstart, prgcnt->yend, 0);
	glVertex3f(prgcnt->xstart, prgcnt->yend - prgcnt->width, 0);
	glVertex3f(prgcnt->xend, prgcnt->yend - prgcnt->width, 0);
	glVertex3f(prgcnt->xend, prgcnt->yend, 0);

	// vesntre sidestrek
	
	glVertex3f(prgcnt->xstart, prgcnt->ystart, 0);
	glVertex3f(prgcnt->xstart, prgcnt->yend, 0);
	glVertex3f(prgcnt->xstart + prgcnt->width, prgcnt->yend, 0);
	glVertex3f(prgcnt->xstart + prgcnt->width, prgcnt->ystart, 0);

	glVertex3f(prgcnt->xend, prgcnt->ystart, 0);
	glVertex3f(prgcnt->xend, prgcnt->yend, 0);
	glVertex3f(prgcnt->xend + prgcnt->width, prgcnt->yend, 0);
	glVertex3f(prgcnt->xend + prgcnt->width, prgcnt->ystart, 0);

	// dynamisk del

	glVertex3f(prgcnt->xstart, prgcnt->ystart, 0);
	glVertex3f(prgcnt->xstart, prgcnt->yend, 0);

	glVertex3f(prgcnt->xstart + progress, prgcnt->yend, 0);
	glVertex3f(prgcnt->xstart + progress, prgcnt->ystart, 0);

	glEnd();
	
	prgcnt->progress++;

}

void loadingscreen_fadeout(DWORD progress, Texture &pic)
{

	float offset = (float) progress / 3000.0;

	overlay_advanced(-1.0,1.0,-0.1,  	1.0, 1.0 - (offset / 2.0),-0.1,	pic,	
				0.0, 0.0 + (offset / 4.0), 1.0, 0.0 + (offset / 4.0) - 0.001);

	overlay_advanced(-1.0,	1.0 - (offset / 2.0),-0.1,	1.0, 0.45,-0.1,	pic,	
				0.0, 0.0 + (offset / 4.0), 1.0, 0.275);

	overlay_advanced(-1.0 - offset * 4.25, 0.45, -0.1, 1.0 - offset * 4.25, -1.0, -0.1, pic, 0, 0.275, 1.0, 1.0);
}

int loadingscreen(Window &wnd, progresscounter *prgcnt, Texture &bgpic)
{
	int error = 0;

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);


	glMatrixMode(GL_PROJECTION);							// Velger projection matrix
	glLoadIdentity();// Setter modelview matrix = identity matrix 
	
	gluPerspective(45.0f, 4.0f/3.0f, 1.0f, 1000.0f);		// Setter projection matrix = 45 graders perspektiv i 4/3 aspekt
		
	glMatrixMode(GL_MODELVIEW);								// Velger modelview matrix
	glLoadIdentity();// Setter modelview matrix = identity matrix 



	progressbar(prgcnt);	

	if (prgcnt->progress > prgcnt->entries)
	{	
		char debugstr[256];
		error = -1;
		sprintf(debugstr, "Entries skulle vært minst %d", prgcnt->progress);
		displaytext(-2,-2.5,1.5,debugstr);
	}
	else
		displaytext(-2,-2.5,1.5,prgcnt->title);

	if (prgcnt->subtitle)
		displaytext(-2,-4.5,1.5,prgcnt->subtitle);

	overlay(-1,1,1,-1, bgpic);

	wnd.update();
	return error;
}
