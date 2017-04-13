#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "includes/GLWrapper.h"

// #include "glfont.h"
// #include <gl\glaux.h>

#include "Cg/cg.h"
#include "Cg/cgGL.h"

#include <math.h>

#include <cstdlib>
#include <ctime>
/*
#include "bass.h"
#include "basswrap.h"
*/
#include "includes/main.h"
#include "includes/vumeter.h" 
/*
#include "loadingscreen.h"
#include "starfield.h"
#include "overlay.h"
#include "linenoice.h"
#include "copperbars.h"
#include "linemorph.h"
#include "vectorslime.h"
*/
extern Window wnd;
extern short escaped;

void bloom(Window &wnd, int duration, Model &mod, Texture tex, RenderTarget target1, RenderTarget target2, Texture vu, bool usevu)
{	
	DWORD curtime = GetTickCount();
	DWORD endtime = GetTickCount() + duration;
	DWORD starttime = GetTickCount();

	
	while ((endtime > curtime) && !escaped)
	{
		escaped = GetAsyncKeyState(VK_ESCAPE);
		
		if (usevu) 	vumeter(-1,1,1,-1,16,vu,511);

		DWORD difftime = curtime - starttime;

		float time =  (float) (((double) difftime) / 1000);

		// Starter rendring til target1
		target1.startRenderTo();

		// Blanker skjermen
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Setter opp matriser
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(65.0, 1.333f, 1, 1900);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//gluLookAt(400, 200, 100, 0,0,0, 0,1,0);
		
		gluLookAt(400, 200, 100, 0,sin(time) * 90 ,sin(time) * 90, 0,1,0);

		glRotatef(80, 0, 1, 0);
		glRotatef(260, 1, 0, 0);
		glTranslatef(50,0,110);
		
		glRotatef(time*70, 0, 0, 1);

		// Tegner modellen
		EnableSEM();
		tex.enable();
		mod.draw();
		DisableSEM();

		// Avslutter rendring til target1
		target1.endRenderTo();

		// Kopierer bildet på target1 over til target2 som er mindre
		target1.blitTo(target2);

		// Blurer target 2
//		target2.gaussianBlur(3, 1, 1, 1.1);
		target2.gaussianBlur(3, 1, 1, 0.6+time/24);

		// Blitter target1 tilbake på skjermen
		target1.blit();

		// Blitter det blura bildet addititvt på toppen
		target2.blitAdd();

		if (usevu) 	vumeter(-1,1,1,-1,16,vu,511);

		wnd.update();

		curtime = GetTickCount();
	}
}

void newbloom(int progress, Model &mod, Texture &tex, RenderTarget &target1, RenderTarget &target2, float bloomfactor)
{	
	
//	DWORD curtime = GetTickCount();
//	DWORD endtime = GetTickCount() + duration;
//	DWORD starttime = GetTickCount();

//		glPushMatrix();

		float time =  (float) (((double) progress) / 1000);

		// Starter rendring til target1
		target1.startRenderTo();

		// Blanker skjermen
//		glClearColor(0,0,0,1);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Setter opp matriser
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(65.0, 1.333f, 1, 1900);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		gluLookAt(10, 10, 100, 0,0,0, 0,1,0);
		
//		gluLookAt(400, 200, 100, 0,sin(time) * 90 ,sin(time) * 90, 0,1,0);

//		glRotatef(80, 0, 1, 0);
//		glRotatef(260, 1, 0, 0);
//		glTranslatef(0,0,11);
		
		glRotatef(time*70, 0, 0, 1);

		// Tegner modellen
		EnableSEM();
			tex.enable();
			mod.draw();
			tex.disable();
		DisableSEM();

		// Avslutter rendring til target1
		target1.endRenderTo();

		// Kopierer bildet på target1 over til target2 som er mindre
		target1.blitTo(target2);

		// Blurer target 2
//		target2.gaussianBlur(3, 1, 1, 1.1);

		target2.gaussianBlur(3, 1, 1, bloomfactor);

		// Blitter target1 tilbake på skjermen
		target1.blit();

		// Blitter det blura bildet addititvt på toppen
		target2.blitAdd();

//		wnd.update();

//		glPopMatrix();


		target1.endRenderTo();
}
