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

#include "includes/bass.h"
#include "includes/basswrap.h"

#include "includes/main.h"
/*
#include "includes/vumeter.h" 
#include "loadingscreen.h"
#include "starfield.h"
#include "overlay.h"
#include "linenoice.h"
#include "copperbars.h"
#include "linemorph.h"
*/
#include "includes/vectorslime.h"

extern int lastTime;


void vectorslime(int progress, Model &mdl, Model &mdl_original, float *distance_buffer, vertex *rotation_buffer, Texture tex, float speed)
{
	// fyller distanse og rotasjonsbuffer
	for (int i = 0; i < mdl.meshArray[0]->vertexCount; i++)
	{
		vertex v = mdl.meshArray[0]->vertexArray[i];
		distance_buffer[i] = sqrtf(sqrtf(v.x*v.x+v.y*v.y+v.z*v.z));
		rotation_buffer[i].x = 0;
		rotation_buffer[i].y = 0;
		rotation_buffer[i].z = 0;
	}

	float time = (float) progress / 50.0;
	float deltaTime = speed;

	// Vectorslime
	// -------------------------------------------------------
	vertex u;
	u.x = sin(time*0.2)*13.0f;		
	u.y = sin(time*0.1)*13.0f;
	u.z = sin(time*0.12)*13.0f;
	for (int i = 0; i < mdl.meshArray[0]->vertexCount; i++)
	{
		vertex* v = &mdl.meshArray[0]->vertexArray[i];
		float* d = &distance_buffer[i];
		vertex* r = &rotation_buffer[i];

		float ammount = 100000.0f;
		r->x += ((u.x - r->x) / (*d)) * ammount * deltaTime;
		r->y += ((u.y - r->y) / (*d)) * ammount * deltaTime;
		r->z += ((u.z - r->z) / (*d)) * ammount * deltaTime;
		vertex k = mdl_original.meshArray[0]->vertexArray[i];
		k = rotatex(k, r->x);
		k = rotatey(k, r->y);
		k = rotatez(k, r->z);
		*v = k;
	}
	mdl.calcNormals();
		// -------------------------------------------------------

/*
		glClearColor(0,0,0,0);									
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
*/		
	glMatrixMode(GL_PROJECTION);							
	glLoadIdentity();										
	gluPerspective(45.0f, 4.0f/3.0f, 1.0f, 1000.0f);		
		
	glMatrixMode(GL_MODELVIEW);								
	glLoadIdentity();			
	gluLookAt(200,200,200, 0,0,0, 0,1,0);	
																
	tex.enable();
	EnableSEM();
	mdl.draw();		
	tex.disable();
	DisableSEM();

}


void weird_vectorslime(int progress, Model &mdl, Model &mdl_original, float *distance_buffer, vertex *rotation_buffer, Texture tex, float amount)
{
	float time		= (float) progress / 1000.0;
	float deltaTime = (float) (progress - lastTime) / 1000.0;

	lastTime = progress;

//	printDebug("%f - %f\n ", time, deltaTime);

	// Vectorslime
	// -------------------------------------------------------
	

	
	for (int i = 0; i < mdl.meshArray[0]->vertexCount; i++)
	{
		float t = time*1.5-distance_buffer[i]*0.1;
		vertex u, trans;
		u.x = sin(t*0.2)*13.0f;		
		u.y = sin(t*0.1+2)*13.0f;
		u.z = sin(t*0.12+1)*13.0f;
		trans.x = sin(t*1.01)*100.0f;
		trans.y = sin(t*1.13+3)*100.0f;
		trans.z = sin(t*1.2+4)*100.0f;

		float pulse = powf(sinf(t), 100.0);

		vertex* v = &mdl.meshArray[0]->vertexArray[i];
		

		vertex k = mdl_original.meshArray[0]->vertexArray[i];
		k = rotatex(k, u.x);
		k = rotatey(k, u.y);
		k = rotatez(k, u.z);
		k.x += trans.x;
		k.y += trans.y;
		k.z += trans.z;
		k.x += mdl.meshArray[0]->normalArray[i].x*pulse*10;
		k.y += mdl.meshArray[0]->normalArray[i].y*pulse*10;
		k.z += mdl.meshArray[0]->normalArray[i].z*pulse*10;
		*v = k;
	}
	mdl.calcNormals();
	// -------------------------------------------------------

	glMatrixMode(GL_PROJECTION);							
	glLoadIdentity();										
	gluPerspective(45.0f, 4.0f/3.0f, 1.0f, 1000.0f);		
		
	glMatrixMode(GL_MODELVIEW);								
	glLoadIdentity();			
		
	gluLookAt(200,200,200, 0,0,0, 0,1,0);	
																
	tex.enable();
	EnableSEM();
	mdl.draw();		
	tex.disable();
	DisableSEM();
}