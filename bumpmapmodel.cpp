#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "includes/GLWrapper.h"
/*
#include "glfont.h"
#include <gl\glaux.h>
*/
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
/*
#include "vumeter.h" 
#include "loadingscreen.h"
#include "starfield.h"
#include "overlay.h"
#include "linenoice.h"
#include "copperbars.h"
#include "linemorph.h"
#include "vectorslime.h"

#include "crap.h";
*/

#include "includes/background.h"

//extern Window wnd;
//extern short escaped;
//extern CGcontext cgc;

using namespace GLWrapper;

void zokum_bumpmapmodel(DWORD progress, Model &mdl, Texture &tex, Texture &bump, 
					CGcontext &cgc,
					CGprogram &fprg,
					CGprogram &vprg
				  )
{
	float prog = (float) progress / 3.0f;
/*
	cgc = cgCreateContext();
	cgSetErrorCallback(errorCallback);
	CGprogram fprg = cgCreateProgramFromFile(cgc, CG_SOURCE, "shader.cg", CG_PROFILE_ARBFP1, "psMain", NULL);
	cgGLLoadProgram(fprg);
	CGprogram vprg = cgCreateProgramFromFile(cgc, CG_SOURCE, "shader.cg", CG_PROFILE_ARBVP1, "vsMain", NULL);
	cgGLLoadProgram(vprg);
*/
		// Setter opp matriser
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, 1.333f, 1.00, 1000.0);

		glMatrixMode(GL_MODELVIEW);
		
		glLoadIdentity();
		
		gluLookAt(	0,	0,	0, 
					0,	0,	100, 
					0,1,0);
		
		
		glTranslatef(0, 0, 555.0);

		glRotatef(prog, 0, 1, 0);

		// Skrur på shaderne
		cgGLEnableProfile(CG_PROFILE_ARBFP1);
		cgGLBindProgram(fprg);
		cgGLEnableProfile(CG_PROFILE_ARBVP1);
		cgGLBindProgram(vprg);

		// Binder teksturene til samplere
		CGparameter p1 = cgGetNamedParameter(fprg, "envmap");
		cgGLSetTextureParameter(p1, tex.handle);
		cgGLEnableTextureParameter(p1);
		CGparameter p2 = cgGetNamedParameter(fprg, "bumpmap");
		cgGLSetTextureParameter(p2, bump.handle);
		cgGLEnableTextureParameter(p2);

		mdl.draw();

		// Kobler teksturer fra samplere (VIKTIG!)
		cgGLDisableTextureParameter(p1);
		cgGLDisableTextureParameter(p2);

		// Skrur av shaderen
		cgGLDisableProfile(CG_PROFILE_ARBFP1);
		cgGLUnbindProgram(CG_PROFILE_ARBFP1);
		cgGLDisableProfile(CG_PROFILE_ARBVP1);
		cgGLUnbindProgram(CG_PROFILE_ARBVP1);
}

/*
void bumpmapmodel(Window &wnd, int duration, Model mdl, Texture tex, Texture bump)
{
	DWORD curtime = GetTickCount();
	DWORD endtime = GetTickCount() + duration;
	Timer timer;

	cgc = cgCreateContext();
	cgSetErrorCallback(errorCallback);
	CGprogram fprg = cgCreateProgramFromFile(cgc, CG_SOURCE, "shader.cg", CG_PROFILE_ARBFP1, "psMain", NULL);
	cgGLLoadProgram(fprg);
	CGprogram vprg = cgCreateProgramFromFile(cgc, CG_SOURCE, "shader.cg", CG_PROFILE_ARBVP1, "vsMain", NULL);
	cgGLLoadProgram(vprg);

	Model mod = mdl;

	while ((endtime > curtime) && !escaped)
	{
		escaped = GetAsyncKeyState(VK_ESCAPE);

		float time = timer.getTime();

		// Blanker skjermen
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Setter opp matriser
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, 1.333f, 1, 1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(50, 50, 100, 0,0,0, 0,1,0);
		glRotatef(timer.getTime()*60, 0, 1, 0);

		// Skrur på shaderne
		cgGLEnableProfile(CG_PROFILE_ARBFP1);
		cgGLBindProgram(fprg);
		cgGLEnableProfile(CG_PROFILE_ARBVP1);
		cgGLBindProgram(vprg);

		// Binder teksturene til samplere
		CGparameter p1 = cgGetNamedParameter(fprg, "envmap");
		cgGLSetTextureParameter(p1, tex.handle);
		cgGLEnableTextureParameter(p1);
		CGparameter p2 = cgGetNamedParameter(fprg, "bumpmap");
		cgGLSetTextureParameter(p2, bump.handle);
		cgGLEnableTextureParameter(p2);

		mod.draw();

		// Kobler teksturer fra samplere (VIKTIG!)
		cgGLDisableTextureParameter(p1);
		cgGLDisableTextureParameter(p2);

		// Skrur av shaderen
		cgGLDisableProfile(CG_PROFILE_ARBFP1);
		cgGLUnbindProgram(CG_PROFILE_ARBFP1);
		cgGLDisableProfile(CG_PROFILE_ARBVP1);
		cgGLUnbindProgram(CG_PROFILE_ARBVP1);



		wnd.update();											// Oppdaterer skjermen med det som har blit	tegnet
		curtime = GetTickCount();
	}
}
*/

void zokum_bumpyball(int starttime, int curtime, int duration, Model &mdl, Model &mdl_original, Texture &tex, int style)

//void bumpyball(int progress, Texture tex, int style)
{
	float freqx = 0;
	float freqy = 0;
	float freqz = 0;
	float amp = 0;

	while (curtime < starttime + duration)
	{
		curtime = GetTickCount();
		background(starttime, curtime);

		float timenow = (curtime - starttime) / 1000.0;

		if (style == 0)
	{
		freqx = 3.0f+tan(timenow)*4.0f;
		freqy = 7.0f+cos(timenow*1.3f)*6.0f;
		freqz = 2.0f+tan(timenow*1.1f)*6.0f;
		amp = 32.0f+sin(timenow*1.4)*30.0f;	
	}
		else if (style == 2)
	{
/*		if (time (NULL) < halvtid)
		{
			freqx = 2.0f+sin(timenow)*4.0f;
			freqy = 5.0f+cos(timenow*1.3f)*6.0f;
			freqz = 2.0f+sin(timenow*1.1f)*6.0f;
			amp = 32.0f+sin(timenow*1.4) *30.0f * (0.9 + sync[0]*8); 
		}
		else if (time(NULL) > halvtid && time(NULL) < cooldown)
		{
			freqx = 3.0f+tan(timenow)*4.0f;
			freqy = 7.0f+cos(timenow*1.3f)*6.0f;
			freqz = 2.0f+tan(timenow*1.1f)*6.0f;
			amp = 32.0f+sin(timenow*1.4) *30.0f * (0.9 + sync[0]*20); 
		}
		else
		{
			freqx = 21.0f+tan(timenow)*1.0f;
			freqy = 2.0f+cos(timenow*1.3f)*2.0f;
			freqz = 9.0f+tan(timenow*1.1f)*8.0f;
			amp = 32.0f+sin(timenow*1.4) *30.0f * (0.9 + sync[0]*20); 				
		}
					
*/
		//glTranslatef(sync[0]*30.0f, sync[3]*100.0f, sync[4]*100.0f);
	}
		else if ((style == 1))
	{
		freqx = 1.0f+sin(timenow)*4.0f;
		freqy = 2.0f+cos(timenow*1.3f)*6.0f;
		freqz = 5.0f+sin(timenow*1.1f)*6.0f;
		amp = 32.0f+sin(timenow*1.4)*30.0f;
	}


	
		for (int i = 0; i < mdl.meshArray[0]->vertexCount; i++)
		{
			vertex *v = &mdl.meshArray[0]->vertexArray[i];
			vertex *o = &mdl_original.meshArray[0]->vertexArray[i];
			vertex *n = &mdl_original.meshArray[0]->normalArray[i];
	
			float f = sin(n->x*freqx + timenow) * sin(n->y*freqy + timenow) * sin(n->z*freqz + timenow);

			v->x = o->x + n->x * amp * f;
			v->y = o->y + n->y * amp * f;
			v->z = o->z + n->z * amp * f;
		}
		mdl.calcNormals();

		glMatrixMode(GL_PROJECTION);							
		glLoadIdentity();										
		gluPerspective(45.0f, 4.0f/3.0f, 1.0f, 1000.0f);		
		
		glMatrixMode(GL_MODELVIEW);								
		glLoadIdentity();			

		gluLookAt(100,150,200, 0,0,0, 0,1,0);	
		glRotatef(timenow*30.0f, 1,1,1);

																
		tex.enable();
		EnableSEM();
		mdl.draw();	
		tex.disable();
		DisableSEM();
/*		wnd.update();
		if (GetAsyncKeyState(VK_ESCAPE)) return;
		*/
	}
}