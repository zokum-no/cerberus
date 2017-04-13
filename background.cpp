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
#include "includes/starfield.h"
#include "includes/overlay.h"
/*#include "includes/linenoice.h"
#include "includes/copperbars.h"
*/
#include "includes/linemorph.h"

extern int lastTime;

void fade(int starttime, int curtime, int endtime, bool down, float r, float g, float b)
{
	float shadeR = 0;
	float shadeG = 0;
	float shadeB = 0;

	double progress = endtime - curtime;
	double total = endtime - starttime;

	if (down)
	{
		shadeR = r - (r * ( progress / total));
		shadeG = g - (g * ( progress / total));
		shadeB = b - (b * ( progress / total));
	}
	else
	{
		shadeR = (r * (progress / total));
		shadeG = (g * (progress / total));
		shadeB = (b * (progress / total));
	}
	glClearColor(shadeR,shadeG,shadeB,0);		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void imagecorners(Texture tex1, Texture tex2, int progress, int lengde)
{
			float prg = (double) progress / (double) lengde;

			overlay(-1,1,0-prg,0+prg,tex2);
			overlay(0+prg,0-prg,1,-1,tex2);
			
			overlay(0+prg,1,1,0+prg,tex1);
			overlay(-1,0-prg,0-prg,-1,tex1);
}

void imagecornersmove(Texture tex1, Texture tex2, int progress)
{
			float prg = (double) progress / (double) 2000;

			overlay(-1+prg,1-prg,-0.5+prg,0.5-prg,tex2);
			overlay(0.5-prg,1-prg,1-prg,0.5-prg,tex2);
			overlay(-1+prg,-0.5+prg,-0.5+prg,-1+prg,tex2);
			overlay(0.5-prg,-0.5+prg,1-prg,-1+prg,tex2);
}

void imagecornersrotate(Texture tex1, int progress)
{
			float prg = (double) progress / (double) 12000;
			int i = 0;

			double rot = 0;

			for (int i = 0; i++; i != 5)
			{
				if (i == 0) rot = 0;
				if (i == 1) rot = 1.57;
				if (i == 2) rot = 3.14;
				if (i == 3) rot = 4.71;
	
				overlay(0.5-prg,-0.5+prg,1-prg,-1+prg,tex1);
			}

//			overlay(-1+prg,1-prg,-0.5+prg,0.5-prg,tex1);
//			overlay(0.5-prg,1-prg,1-prg,0.5-prg,tex1);
//			overlay(-1+prg,-0.5+prg,-0.5+prg,-1+prg,tex1);
//			overlay(0.5-prg,-0.5+prg,1-prg,-1+prg,tex1);
}

extern short escaped;  // crap

// extern Window wnd;

void effekt1(Window &wnd, int duration)
{
	DWORD curtime = GetTickCount();
	DWORD endtime = GetTickCount() + duration;

	while ((endtime > curtime) && !escaped)
	{
		escaped = GetAsyncKeyState(VK_ESCAPE);

		glClearColor(0,0,0,0);									// Setter bakgrunnsfargen til sort
		glClear(GL_COLOR_BUFFER_BIT	| GL_DEPTH_BUFFER_BIT);		// Tømmer framebuffer og depth buffer

		glMatrixMode(GL_PROJECTION);							// Velger projection matrix
		glLoadIdentity();										// Setter projection matrix	= identity matrix (ingen transformasjon)
		glMatrixMode(GL_MODELVIEW);								// Velger modelview	matrix
		glLoadIdentity();										// Setter modelview	matrix = identity matrix (ingen	transformasjon)

		glBegin(GL_TRIANGLES);									// Begynner	på trekant-tegning
		glVertex3f(	0.0f, 0.5f,	0.0f);							// Første punkt	i trekanten
		glVertex3f(-0.5f,-0.5f,	0.0f);							// Andre punkt i trekanten
		glVertex3f(	0.5f,-0.5f,	0.0f);							// Tredje punkt	i trekanten
		glEnd();												// Forteller OpenGL	at vi er ferdige med å tegne trekanter

		wnd.update();											// Oppdaterer skjermen med det som har blit	tegnet
		curtime = GetTickCount();
	}
}

void noice()
{
	glBegin(GL_POINTS);
	float fx = 0;
	float fy = 0;
	int i = 0;
	
	srand(GetTickCount() );

	while (i != 2000)
	{
		glColor3f(1.0, 1.0, 1.0);
		fx = (float) rand()/RAND_MAX * 50;
		fy = (float) rand()/RAND_MAX * 50;
		glVertex2f(fx, fy);
		i++;
	}
	glEnd();
}

void drumincrease(Window &wnd, int duration, Model mdl, Texture tex, bool picnoice)
{
	DWORD curtime = GetTickCount();
	DWORD endtime = GetTickCount() + duration;

	Timer timer;

	bool basstoggle = true;

	float toggletime = timer.getTime();

	while ((endtime > curtime) && !escaped)
	{
		escaped = GetAsyncKeyState(VK_ESCAPE);
		float timenow = timer.getTime();

		glClearColor(0,0,0,0);									// Setter bakgrunnsfargen til sort
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Tømmer framebuffer og depth buffer

		glMatrixMode(GL_PROJECTION);							// Velger projection matrix
		glLoadIdentity();										// Nullstiller projection matrix
		gluPerspective(45.0f, 4.0f/3.0f, 1.0f, 1000.0f);		// Setter projection matrix = 45 graders perspektiv i 4/3 aspekt

		glMatrixMode(GL_MODELVIEW);								// Velger modelview matrix
		glLoadIdentity();										// Setter modelview matrix = identity matrix 

		glPushMatrix();

		int c = ((int) (timenow * 2) %4);
		if (c == 0)			
				gluLookAt(400,200,200, 0,0,0, 0,1,0);				// Setter opp kamera på posisjon (200,200,200) som 
																	// ser mot (0,0,0) og (0,1,0) angir hva som er "opp" (upvector)
			else if (c==1)
				gluLookAt(-400,200,200, 0,0,0, 0,1,0);
			else if (c==2)
				gluLookAt(400,-200,200, 0,0,0, 0,1,0);
			else if (c==3)
				gluLookAt(-400,-200,200, 0,0,0, 0,1,0);

			if (picnoice) { noice(); }

		glRotatef(timenow*320, (timenow * -1240), (timenow * 1601),1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		tex.enable();
		EnableSEM();

//		if (picnoice) { noice(); }

		if ( sync[0] > 0.54)
			toggletime = timenow +0.1;

		 //if (sync[0] > 0.54)	printf("%f\n",sync[0]);

		if (timenow < toggletime)
		{
			float faen = 5 + (((toggletime - timenow) * 50));
			glScalef(faen,faen,faen);
		}
		else
			glScalef(5,5,5);
/*
		if (sync[20] > 0.5) basstoggle = !basstoggle;
		
		if (basstoggle)	glScalef(7.5f, 7.5f, 7.5f);
		else			glScalef(17.5f, 17.5f, 17.5f);
*/
		mdl.draw();

		tex.disable();
		DisableSEM();
		glDisable(GL_BLEND);

		glPopMatrix();

		if (picnoice)
		{	
			gluLookAt(400,0,0, 0,0,0, 0,1,0);
			noice(); 
		}

		wnd.update();
		curtime = GetTickCount();
	}

}

void cubegraphics(Window &wnd, int duration, Model mdl, Texture tex, float speed)
{
	DWORD curtime = GetTickCount();
	DWORD endtime = GetTickCount() + duration;
	Timer timer;

	while ((endtime > curtime) && !escaped)
	{
		escaped = GetAsyncKeyState(VK_ESCAPE);

		float time = timer.getTime();

		glClearColor(0,0,0,0);									// Setter bakgrunnsfargen til sort
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Tømmer framebuffer og depth buffer

		glMatrixMode(GL_PROJECTION);							// Velger projection matrix
		glLoadIdentity();										// Nullstiller projection matrix
		gluPerspective(45.0f, 4.0f/3.0f, 1.0f, 1000.0f);		// Setter projection matrix = 45 graders perspektiv i 4/3

		glMatrixMode(GL_MODELVIEW);								// Velger modelview matrix
		glLoadIdentity();										// Setter modelview matrix = identity matrix 
		gluLookAt(100,100,100, 0,0,0, 0,1,0);					// Setter opp kamera på posisjon (100,100,100) som 
		// ser mot (0,0,0) og (0,1,0) angir hva som er "opp" (upv

		tex.enable();											// Skrur på tekstur
		glRotatef(time*260.0f*speed, time*290.0f*speed,0,1);							// Roterer objektet med 20 grader i sekundet om z-aksen
		mdl.draw();											// Tegner kuben
		tex.disable();											// Skrur av tekstur

		wnd.update();											// Oppdaterer skjermen med det som har blit	tegnet
		curtime = GetTickCount();
	}

}

void explode(Window &wnd, int duration, Model mdl, Texture tex, float distance)
{
	DWORD curtime = GetTickCount();
	DWORD endtime = GetTickCount() + duration;

	Model mdl_original = mdl;
	Timer timer;

	while ((endtime > curtime) && !escaped)
	{
		escaped = GetAsyncKeyState(VK_ESCAPE);

		float timenow = timer.getTime();
		float deltaTime = timer.getInterval();


		// Sinus Deform
		// -------------------------------------------------------
		float freqx = 5.0f+sin(timenow)*4.0f;
		float freqy = 5.0f+sin(timenow*1.3f)*4.0f;
		float freqz = 5.0f+sin(timenow*1.1f)*4.0f;
		float amp = 35.0f+sin(timenow*1.4)*30.0f;

		for (int i = 0; i < mdl.meshArray[0]->vertexCount; i++)
		{
			vertex* v = &mdl.meshArray[0]->vertexArray[i];
			vertex* o = &mdl_original.meshArray[0]->vertexArray[i];
			vertex* n = &mdl_original.meshArray[0]->normalArray[i];

			float f = sin(n->x*freqx + timenow) * sin(n->y*freqy + timenow) * sin(n->z*freqz + timenow);
			v->x = o->x + n->x * amp * f;
			v->y = o->y + n->y * amp * f;
			v->z = o->z + n->z * amp * f;
		}
		mdl.calcNormals();
		// -------------------------------------------------------


		glClearColor(0,0,0,0);									
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		

		glMatrixMode(GL_PROJECTION);							
		glLoadIdentity();										
		gluPerspective(45.0f, 4.0f/3.0f, 1.0f, 1000.0f);		

		glMatrixMode(GL_MODELVIEW);								
		glLoadIdentity();			
		gluLookAt(5,15,100+distance, 0,0,0, 0,1,0);	
		glRotatef(timenow*30.0f, 1,1,1);

		tex.enable();
		EnableSEM();
		mdl.draw();		
		tex.disable();
		DisableSEM();

		wnd.update();	
		curtime = GetTickCount();
	}
}

void modelzoom(Window &wnd, int duration, Model mdl, Texture tex, int pattern, float offset)
{
	DWORD curtime = GetTickCount();
	DWORD endtime = GetTickCount() + duration;
	DWORD starttime = GetTickCount();

	float perx;
	float pery;
	float perz;
	float dist;

	if (pattern == 1)
	{
		perx = 25.0f;
		pery = 9.0f/3.0f;
		perz = 6.0f;
		dist = 1800.0f;
	//	glRotatef(90,90,0,1);
	}
	else
	{
		perx = 45.0f;
		pery = 4.0f/3.0f;
		perz = 1.0f;
		dist = 1000.0f;
	}

	while ((endtime > curtime) && !escaped)
	{
		escaped = GetAsyncKeyState(VK_ESCAPE);
		DWORD difftime = curtime - starttime;

		float time =  (float) (((double) difftime) / 1000);

		glClearColor(0,0,0,0);									// Setter bakgrunnsfargen til sort

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Tømmer framebuffer og depth buffer
		
		glMatrixMode(GL_PROJECTION);							// Velger projection matrix
		glLoadIdentity();										// Nullstiller projection matrix
		gluPerspective(perx, pery, perz, 1900.0f);		// Setter projection matrix = 45 graders perspektiv i 4/3 aspekt
		
		glMatrixMode(GL_MODELVIEW);								// Velger modelview matrix
		glLoadIdentity();// Setter modelview matrix = identity matrix 

		if (pattern == 1)
		{
			if		((time >= 0)		&& (time < 3))	{gluLookAt(-900 +((time - 0) * 300) ,0,0, 0,0,0, 0,1,0);}
			else if ((time >= 3)		&& (time < 6))	{
				gluLookAt(-900 -((time - 5) * 300) ,0,0, 0,0,0, 0,1,0);
				glRotatef((-time + offset)*64,255,255,1);
			}
			else
				{gluLookAt(-600,0,0, 0,0,0, 0,1,0);}
		}
		else
		{
			gluLookAt(-700 +((time) * 160) ,0,0, 0,0,0, 0,1,0);
			glRotatef(-(time + offset)*64,255,255,1);
		}
	//	else if ((time >= 6)		&& (time < 9))	{gluLookAt(-600 +((time - 9) * 300) ,0,0, 0,0,0, 0,1,0);}
	//	else if ((time >= 9)		&& (time < 12))	{gluLookAt(-600 +((time - 12)* 200) ,0,0, 0,0,0, 0,1,0);}

		tex.enable();									// Skrur på tekstur
		EnableSEM();									// Skrur på Spherical Environment Mapping


		glRotatef(270,0,90,1);
		glRotatef(270,90,0,1);

		mdl.draw();
		
		tex.disable();									// Skrur av tekstur
		DisableSEM();									// Skrur av Spherical Environment Mapping

		wnd.update();									// Oppdaterer skjermen med det som har blit	tegnet
		curtime = GetTickCount();
	}
}

void background(int starttime, int curtime)
{
		if		(curtime <= (starttime + 40000)) glClearColor(0,0,0,0);
		else if	(curtime <= (starttime + 42000)) fade(starttime+40000, curtime, starttime + 42000, false, 1.0, 1.0, 1.0);
		else if	(curtime <= (starttime + 49000)) glClearColor(0,0,0,0);
		else if	(curtime <= (starttime + 76000)) glClearColor(0,0,0,0);
//		else if	(curtime <= (starttime + 78000)) fade(starttime+76000, curtime, starttime + 78000, false, 0.5, 0.0, 0.0);
		else if	(curtime <= (starttime + 110000)) glClearColor(0,0,0,0);
		else
		{
			float col;

			if (oldsync[2] > sync[2])
				oldsync[2] = oldsync[2] - 0.001;
			else
				oldsync[2] = sync[2];

			col = oldsync[2];
						
			glClearColor(col,col,col,0);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;

		if		(curtime <= (starttime + 2000))		fade(starttime, curtime, starttime + 2000, false, 0.7, 0.1, 0.1);							
		else if (curtime <= (starttime + 4000))		fade(starttime+2000, curtime, starttime + 4000, false, 0.1, 0.5, 0.1);							
		else if (curtime <= (starttime + 7400))		fade(starttime+4000, curtime, starttime + 7400, false, 0.1, 0.1, 0.5);
		else if	(curtime <= (starttime + 9500))		fade(starttime+7400, curtime, starttime + 9500, false, 0.5, 0.1, 0.1);							
		else if (curtime <= (starttime + 11500))	fade(starttime+9500, curtime, starttime + 11500, false, 0.1, 0.4, 0.1);							
		else if (curtime <= (starttime + 15000))	fade(starttime+11000, curtime, starttime + 15000, false, 0.1, 0.1, 0.4);
		else if (curtime <= (starttime + 42300))	fade(starttime+15000, curtime, starttime + 25000, false, 0.0, 0.0, 0.0);
		else if (curtime <= (starttime + 56000))
		{
			glClearColor(sync[1],0,0,1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else if (curtime <= (starttime + 65000))
		{
			glClearColor(0,0,sync[1],1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else if (curtime <= (starttime + 125000))
		{
			glClearColor(0,0,0,1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else if (curtime <= (starttime + 130000))		fade(starttime+125000, curtime, starttime + 130000, false, 1.1, 1.1, 1.1);
		else
		{
			glClearColor(1,1,0,0);									// Setter bakgrunnsfargen til sort
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Tømmer framebuffer og depth buffer
		}
}

int noice_pattern = 0;

void noice(Texture a, Texture b, Texture c, Texture d)
{
	noice_pattern++;

	if		(noice_pattern == 0)	a.enable();
	else if (noice_pattern == 1)	b.enable();
	else if (noice_pattern == 2)	c.enable();
	else							
	{
		d.enable();
		noice_pattern = -1;
	}
	
	Texture faen("jessica2.jpg");
	faen.enable();

	glBegin(GL_QUADS);								    // Begynner på trekant-tegning
		glTexCoord2f(0,0); glVertex3f(-1.0,  1.0, -2.0f);		// Første punkt i trekanten (rød)
		glTexCoord2f(0,1); glVertex3f( 1.0,  1.0, -2.0f);		// Andre punkt i trekanten (grønn)
		glTexCoord2f(1,1); glVertex3f( 1.0, -1.0, -2.0f);		// Tredje punkt i trekanten (blå)
		glTexCoord2f(1,0); glVertex3f(-1.0, -1.0, -2.0f);		// Fjerde punkt i stripa (blå)
	glEnd();												// Forteller OpenGL at vi er ferdige med å tegne trekanter
	
	if		(noice_pattern == 0)	a.disable();
	else if (noice_pattern == 1)	b.disable();
	else if (noice_pattern == 2)	c.disable();
	else							d.disable();
}
