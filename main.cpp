#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "includes\glwrapper.h"

#include "includes\glfont.h"
#include "includes\glaux.h"

#include <cg\cg.h>
#include <cg\cgGL.h>

#include <math.h>

#include <cstdlib>
#include <ctime>

// my constants
#include "includes\constants.h"

// audio lib
#include "includes\bass.h"
#include "includes\basswrap.h"

#include "includes\main.h"

// effects etc
/*#include "vumeter.h" */
#include "includes/loadingscreen.h"
#include "includes/starfield.h"
#include "includes/overlay.h"
/*#include "linenoice.h"
#include "copperbars.h"
*/
#include "includes/linemorph.h"
/*
#include "vectorslime.h"
*/
#include "includes/tunnel.h"
/*
#include "tetris.h"
#include "plasma.h"
*/
#include "includes/bumpmapmodel.h"
/*
#include "imagetricks.h"
#include "text.h"
*/

// old stuff, unsorted crap code
// #include "crap.h"

/* noen statiske ting */
float sync[512];
float oldsync[512];
float ancientsync[512];

short escaped = false;

DWORD musicLoop;
DWORD musicLength = 195000;

using namespace GLWrapper;

int lastTime = 0;
GLuint	base;

//Window wnd = NULL;

CGcontext cgc;

extern DWORD lastpro;

void errorCallback() {
	char* buf = new char[65536];
	char* bufnow = buf;
	bufnow += sprintf(bufnow, "%s\n", cgGetErrorString(cgGetError()));
	if(CGerror e = cgGetError()) bufnow += sprintf(bufnow, cgGetErrorString(e)); bufnow += sprintf(bufnow, "\n");
	const char* compilerError = cgGetLastListing(cgc);
	if (compilerError)	bufnow += sprintf(bufnow, compilerError); bufnow += sprintf(bufnow, "\n\n");
	MessageBox(NULL, buf, "CG-Error", 0);
	exit(0);
}

void printDebug(const char *format, ...) {
	va_list args;
	va_start(args, format);

	char buf[2048];
	vsprintf(buf, format, args);

	va_end(args);

	OutputDebugStringA(buf);
}

bool music;

int CALLBACK WinMain(IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nShowCmd) {
	char loadingtext[128] = "Loading Tryggvification 9";
	char windowTitle[128] = "Tryggvification 9";

	LPWSTR *szArglist;
	int nArgs;
	int i;

	// Development defaults
	/*
	bool fullscreen = false;
	int windowWidth = 960;
	int windowHeight = 540;*/

	// Compo show defaults, 16:9
	bool fullscreen = true;
	int windowWidth = 1920;
	int windowHeight = 1080;

	int colorBPP = 32;

	// Command line parsing. Set demo resolution and fullscreen mode etc.
	char hack[1024];
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if( NULL == szArglist )	{
//		wprintf("CommandLineToArgvW failed\n");
		return 1;
	} else for( i=0; i<nArgs; i++) 	{	
		sprintf(hack, "%ws", szArglist[i]);
		if		( strcmp(hack, "--fullscreen") == 0)	{
			fullscreen = true;
		}
		if		( strcmp(hack, "--window") == 0)	{
			fullscreen = false;
		}

		else if	(strncmp(hack, "--width=",8) == 0) {
			if(strlen(hack+8))
			{
				windowWidth=atoi(hack+8);
			}
		}
		else if (strncmp(hack, "--height=",9) == 0) {
			if (strlen(hack+9))
				windowHeight = atoi(hack+9);
		}
	}
	//	   printf("%d: %ws\n", i, szArglist[i]);

	// Free memory allocated for CommandLineToArgvW arguments.
	LocalFree(szArglist);

	// Set up the window that wil be used for rendering the demo
	Window wnd(windowWidth, windowHeight, colorBPP, fullscreen, windowTitle);

	// Important, enables Z-buffer
	glEnable(GL_DEPTH_TEST);

	// Projection matrix (things look smaller farther away...
	glMatrixMode(GL_PROJECTION);					
	
	// Identity matrix, for the math people, it's a matrix with 1 on diagnoal, all else 0
	/*
	1 0 0 0 <- this is what it looks like :)
	0 1 0 0
	0 0 1 0
	0 0 0 1	
	*/
	glLoadIdentity();// Setter modelview matrix = identity matrix 
	
	// Setting modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();// Setter modelview matrix = identity matrix 

	// Sets up perspective etc
	gluPerspective(140, 960.0 / 540.0, 0.01, 2.0);
	
	// Where the camera is pointed
	gluLookAt(0,0,0, 0,0,-2, 0,1,0);

	// Load one "texture" used for the loading screen fullscreen picture.
	Texture bgpic("data/loadingscreen.jpg");

	// THe amount of entries being loaded, used for generating the progress bar
#define LOAD_ENTRIES 35

	// Progress bar related 
	progresscounter *load = newcounter(-0.838, -0.85, 0.44, -0.9, 0.003, LOAD_ENTRIES, loadingtext, "Textures...");
	
	/* All the textures are loaded, update loadingscreen afer loading something */

	load->subtitle = _strdup("Textures");
	loadingscreen(wnd, load, bgpic);
	
	loadingscreen(wnd, load, bgpic);
	Texture bumpmap("data/bumpmap.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture doom("data/doom.jpg");
	
	loadingscreen(wnd, load, bgpic);
	Texture blue("data/blue.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture c64_1("data/c64_1.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture c64_2("data/c64_2.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture c64_3("data/c64_3.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture c64_4("data/c64_4.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture c64_5("data/c64_5.png");

	loadingscreen(wnd, load, bgpic);
	Texture c64_6("data/c64_6.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture door("data/door.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture star("data/star.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture greets("data/greets.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture history("data/history.png");

	loadingscreen(wnd, load, bgpic);
	Texture credits("data/credits.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture bottom("data/bottom.png");

	loadingscreen(wnd, load, bgpic);
	Texture vubar("data/vubar.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture ansigirl("data/ansigirl.jpg");

	loadingscreen(wnd, load, bgpic);
	Texture rushed("data/rushed.png");

	/* All the models used in the demo*/
	/* Some are loaded sveral times, for comparison to a deformed version etc...*/

	free(load->subtitle);
	load->subtitle = _strdup("Models...");
	
	loadingscreen(wnd, load, bgpic);
	Model spikeball1("data/spikeball.ebr");

	loadingscreen(wnd, load, bgpic);
	Model spikeball1_org("data/spikeball.ebr");

	loadingscreen(wnd, load, bgpic);
	Model slimy("data/slimy.ebr");

	loadingscreen(wnd, load, bgpic);
	Model slimy_org("data/slimy.ebr");
	
	loadingscreen(wnd, load, bgpic);
	Model rarkube("data/rarkube.ebr");

	loadingscreen(wnd, load, bgpic);
	Model hisphere("data/hisphere.ebr");

	loadingscreen(wnd, load, bgpic);
	Model hisphere_org("data/hisphere.ebr");

	/* cg, shaders, outdated, but it works... */
	free(load->subtitle);
	load->subtitle = _strdup("Setting up shaders");
	loadingscreen(wnd, load, bgpic);

	CGcontext cgc = cgCreateContext();
	cgSetErrorCallback(errorCallback);
	CGprogram fprg = cgCreateProgramFromFile(cgc, CG_SOURCE, "data/shader.cg", CG_PROFILE_ARBFP1, "psMain", NULL);
	cgGLLoadProgram(fprg);
	CGprogram vprg = cgCreateProgramFromFile(cgc, CG_SOURCE, "data/shader.cg", CG_PROFILE_ARBVP1, "vsMain", NULL);
	cgGLLoadProgram(vprg);

	/* Render targets, used for bloom-effects */
	free(load->subtitle);
	load->subtitle = _strdup("Render targets");
	loadingscreen(wnd, load, bgpic);


// Creating two render targets
	loadingscreen(wnd, load, bgpic);
	RenderTarget target1(windowWidth,windowHeight);
	loadingscreen(wnd, load, bgpic);
	RenderTarget target2(128,128);
	loadingscreen(wnd, load, bgpic);

	bool reset_hack = false;

/* precalcs, not much done here, put more in here if you need some :) */
	free(load->subtitle);
	load->subtitle = _strdup("Precalcing...");
	loadingscreen(wnd, load, bgpic);

	loadingscreen(wnd, load, bgpic);
	float* distance_buffer = new float[spikeball1.meshArray[0]->vertexCount];
	vertex* rotation_buffer = new vertex[spikeball1.meshArray[0]->vertexCount];

	// Star field, buggy but if used correctly, it looks 'ok'
	float mystarfield [STAR_NUM][6];
	float endstarfield [STAR_NUM][6];
	initstarfield(mystarfield);
	initstarfield(endstarfield);

/*
	for (int i = 0; i < spikeball1.meshArray[0]->vertexCount; i++)
	{		vertex v = spikeball1.meshArray[0]->vertexArray[i];
		distance_buffer[i] = sqrtf(sqrtf(v.x*v.x+v.y*v.y+v.z*v.z));
		rotation_buffer[i].x = 0;
		rotation_buffer[i].y = 0;
		rotation_buffer[i].z = 0;
	}
*/
	float rings[2][800];
	float rings2[2][800];
	tunnel_init(rings,60);
	tunnel_init(rings2,800);

/* 
***********************************************************************
where to start in the demo, in seconds 
Use this to debug the demo, start at any point in the demo.
***********************************************************************
*/
	int offset = 0;

/* music */
	free(load->subtitle);
	load->subtitle = _strdup("Music...");
	loadingscreen(wnd, load, bgpic);

/* Here we start the music :)*/	
	wnd.update();
	wnd.BASS_playSong("azen.ogg", 0.0f + (float) offset);
	
/* setting up timers */
	DWORD curtime = GetTickCount();
	DWORD starttime = GetTickCount();
	DWORD endtime = GetTickCount() + 18000000000;

	musicLength = 144000;
	musicLoop = curtime + musicLength;

/* checking load screen consistency */
	if (loadingscreen(wnd, load, bgpic)) {
		// error is reported, please modify code...
		Sleep(2000);	
	}

	while ((curtime < endtime) && !escaped) {	
		curtime = GetTickCount() - starttime + (offset * 1000);

		glMatrixMode(GL_PROJECTION);							// Velger projection matrix
		glLoadIdentity();										// Setter projection matrix = identity matrix (ingen transformasjon)
		glMatrixMode(GL_MODELVIEW);								// Velger modelview matrix
		glLoadIdentity();										// Setter modelview matrix = identity matrix (ingen transformasjon)

		// Set background color to black, ie no red green or blue
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set current "draw" color to white...
		glColor3f(1.0, 1.0, 1.0); // just to make sure...

		// Fetch audio data from the music for effects in synch with music
		wnd.BASS_getSyncData(sync);

//		overlay(-1.0, -0.75,1.0,-1.0, bottom);

		bool edges = 0;

		if 	(curtime < 2000) { 
			float prog = (float) curtime / 2000.0;

			glClearColor(0.48 * prog,0.44 * prog,0.85 * prog, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		else if (curtime < 11000) {	
			overlay(-1.0,1.0, 1.0,-1.0, c64_1);
		}
		else if (curtime < 16000) {
			tunnel((int) curtime - 10000, rings, 32, false, 1, 0.5, 0.3, 0.6);
		}
		else if (curtime < 23000) {
				edges = 1;
				float prog = (float)(curtime - 9500) / 8500;
				glColor3f(prog*2, prog, 1.0);
				vectorslime(curtime, spikeball1, spikeball1_org, distance_buffer, rotation_buffer, blue, 0.00001);
		}
		else if (curtime < 25000) {

			/*overlay_advanced(-1.0,-0.75 - bot,-0.1,
				1.0,-1.0 - bot ,-0.1,
				bottom,	0.01,0.01,0.99,0.99);*/
			overlay_advanced_stretchfade(curtime - 23000, 
				1, 
				-1.0, 1.0, 0.1, 
				1.0, -1.0, 0.1, 
				doom, 
				0.0, 0.0, 
				1.0, 1.0);
			
			//overlay(-1.0,1.0, 1.0,-1.0, c64_6);
			
		}
		else if (curtime < 33000) {
				//glColor3f(prog*2, prog, 1.0);
				vectorslime(curtime, spikeball1, spikeball1_org, distance_buffer, rotation_buffer, doom, 0.00001);
		}
		else if (curtime < 37000) {
				edges = 1;
				tunnel((int) curtime - 33000, rings, 32, false, 1, 0.8, 0.1, 0.2);
		}
		else if (curtime < 40000) {
				overlay(-1, 1, 1, -1, rushed);
		}
		else if (curtime < 44000) {
				starfield(curtime-40000, endstarfield, star, 0);
		}
		else if (curtime < 48000) {
				overlay(-1,1, 1, -1, credits);
		}
		else if (curtime < 50000) {
			//scrolltext(0, 0, curtime - 66000, 0.1, "Code, ansi and gfx by Zokum, music by Azen");
			linemorph(curtime-48000, 1);
			edges = 1;
		}
		else if (curtime < 52000) {
			//scrolltext(0, 0, curtime - 66000, 0.1, "Code, ansi and gfx by Zokum, music by Azen");
			linemorph(curtime-50000, 2);
			edges = 1;
		}
		else if (curtime < 54000) {
			//scrolltext(0, 0, curtime - 66000, 0.1, "Code, ansi and gfx by Zokum, music by Azen");
			linemorph(curtime-52000, 3);
			edges = 1;
/*		}
		else if (curtime < 60000) {
			vumeter(-1, -1, 1, -0.1, 35, vubar, 35);
			edges = 1;
			*/
		} else if (curtime < 70000) {
			zokum_bumpmapmodel(curtime, slimy, blue, bumpmap, 
					cgc,
					fprg,
					vprg
				  );
			edges = 1;
		} else if (curtime < 72000) {
			overlay_advanced_stretchfade(curtime - 70000, 
				1, 
				-1.0, 1.0, 0.1, 
				1.0, -1.0, 0.1, 
				ansigirl, 
				0.0, 0.0, 
				1.0, 1.0);

		} else if (curtime < 80000) {
			zokum_bumpyball(72000, curtime, 8000, hisphere, hisphere_org, blue, 1);
			edges = 1;
		}
		else if (curtime < 86000) {
				edges = 1;
				newbloom(curtime, rarkube, door, target1, target2, 1.0);
		}
		// That was the last effect of the 2016 demo, the stuff below is old stuff, used in other demos...

/*		else if	(curtime < 9500)
		{
			int	curtimemodulus = curtime % 450;
				
			if		(curtimemodulus < 75)
				overlay(-1.0,1.0, 1.0,-1.0, c64_6);
			else if	(curtimemodulus < 150)
				overlay(-1.0,1.0, 1.0,-1.0, c64_5);
			else if	(curtimemodulus < 225)
				overlay(-1.0,1.0, 1.0,-1.0, c64_4);
			else if	(curtimemodulus < 300)
				overlay(-1.0,1.0, 1.0,-1.0, c64_3);
			else if	(curtimemodulus < 375)
				overlay(-1.0,1.0, 1.0,-1.0, c64_2);
			else	
				overlay(-1.0,1.0, 1.0,-1.0, c64_1);
		}
*/
		/*
		else if (curtime < 18000)
		{
			float prog = (float)(curtime - 9500) / 8500;

			int	curtimemodulus = curtime % 1000;

			if		((curtimemodulus) < 150)
			{
				glColor4f(1.0, 1.0, 1.0, 0.5);
				overlay(-1.0,1.0, 1.0,-1.0, c64_2);
			}
			else if	((curtimemodulus < 650) && (curtimemodulus > 500) )
			{
				glColor4f(1.0, 1.0, 1.0, 0.5);
				overlay(-1.0,1.0, 1.0,-1.0, c64_5);
			}
			else
			{
				glColor3f(prog*2, prog, 1.0);
				vectorslime(curtime, spikeball1, spikeball1_org, distance_buffer, rotation_buffer, blue, 0.00001);
			}
			edges = 1;
		}
		else if	(curtime < 25000)
		{
			DWORD progress = (curtime - 18000);
			progress = (DWORD) (((double)progress) + ((double) progress * 0.2));
			slideImages(progress, 10000, true, c64_1, c64_2, c64_3, c64_4, c64_5, c64_6, c64_1, c64_2, c64_3);

			edges = 1;

		}
		else if (curtime < 39000)
		{
			
			int	curtimemodulus = curtime % 1000;
			DWORD progress = curtime - starttime - 25000;
			
			if		((curtimemodulus) < 150)
			{
				overlay(-1.0,1.0, 1.0,-1.0, c64_3);
			}
			else if	(curtimemodulus < 500)
			{
				newbloom(progress * 4, rarkube, blue, target1, target2, 1.0);
			}
			else if	((curtimemodulus < 650) && (curtimemodulus > 500) )
			{
				overlay(-1.0,1.0, 1.0,-1.0, c64_4);
			}
			else
			{
				newbloom(progress * 4, rarkube, door, target1, target2, 1.0);
			}

			edges = 1;

		}	
		else if (curtime < 46000)
		{
			overlay(-1.0,1.0, 1.0,-1.0, greets);
		}
		else if (curtime < 54000)
		{	
			DWORD progress = (curtime - 46000);
			if ((progress % 1000) > 500)
				starfield(progress, mystarfield, star, 0);
			else
				starfield(progress, endstarfield, star, 0);

		}
		else if (curtime < 60000)
		{
			overlay(-1.0,1.0, 1.0,-1.0, history);
		}
		else if (curtime < 70000)
		{	
			DWORD progress = (curtime - 60000);

			if ((progress % 1000) > 500)
				starfield(progress, mystarfield, star, 0);
		
			else
				starfield(progress, endstarfield, star, 0);

		}

		else if (curtime < 85000)
		{	
				vectorslime(curtime, spikeball1, spikeball1_org, distance_buffer, rotation_buffer, blue, 0.00001);
				edges = 1;
		}
		else if	(curtime < 86000)
		{
			int	curtimemodulus = curtime % 450;
			
			if		(curtimemodulus < 75)
				overlay(-1.0,1.0, 1.0,-1.0, c64_6);
			else if	(curtimemodulus < 150)
				overlay(-1.0,1.0, 1.0,-1.0, c64_5);
			else if	(curtimemodulus < 225)
				overlay(-1.0,1.0, 1.0,-1.0, c64_4);
			else if	(curtimemodulus < 300)
				overlay(-1.0,1.0, 1.0,-1.0, c64_3);
			else if	(curtimemodulus < 375)
				overlay(-1.0,1.0, 1.0,-1.0, c64_2);
			else	
				overlay(-1.0,1.0, 1.0,-1.0, c64_1);
		}
		else if (curtime < 87000)
		{
			int	curtimemodulus = curtime % 1000;

			if		((curtimemodulus) < 150)
				overlay(-1.0,1.0, 1.0,-1.0, c64_2);
			else if	((curtimemodulus < 650) && (curtimemodulus > 500) )
				overlay(-1.0,1.0, 1.0,-1.0, c64_5);
			else
			{
				vectorslime(curtime, spikeball1, spikeball1_org, distance_buffer, rotation_buffer, blue, 0.00001);
				edges = 1;
			}
		}

		else if (curtime < 88000)
		{	
			int	curtimemodulus = curtime % 1000;
			DWORD progress = curtime - starttime - 87000;
			
			if		((curtimemodulus) < 150)
				overlay(-1.0,1.0, 1.0,-1.0, c64_3);
			else if	(curtimemodulus < 500)
			{
				newbloom(progress * 4, rarkube, blue, target1, target2, 1.0);
				edges = 1;

			}
			else if	((curtimemodulus < 650) && (curtimemodulus > 500) )
				overlay(-1.0,1.0, 1.0,-1.0, c64_4);
			else
			{
				newbloom(progress * 4, rarkube, door, target1, target2, 1.0);
				edges = 1;

			}
		}
		else if (curtime < 105000)
		{	
			DWORD progress = (curtime - 90000);
			overlay(-1.0, 1.0, 1.0, -1.0, credits);
		}
		*/
		else {
			exit(0);
		}

// Some very simple debug code, basically a progress ticker
#undef ZOKDEBUG
#ifdef ZOKDEBUG
		glColor4f(1.0, 1.0, 1.0, 1.0);
		char str[256];
		sprintf(str, "%d", curtime - starttime);
		displaytext(-2,-4.5,1.5,str);
#endif		

		// Add some nice edges on top of the image, 
		if (edges) {
			if (edges == 1) {
				glColor3f(1.0, 1.0, 1.0);
			
				float top = sin( (double) curtime / 100000.0) * 0.3;
				float bot = sin( (double) curtime / 100000.0) * 0.3;

				bot = 0;
				top = 0;

				overlay_advanced(-1.0,-0.75 - bot,-0.01,
								1.0,-1.0 - bot ,-0.1,
								bottom,	0.01,0.01,0.99,0.99);

				overlay_advanced(-1.0,1.0 + top,-0.1,1.0,0.75 + top,-0.1,bottom,	0.01,0.99,0.99,0.01);

			} else {

			}
		}
		wnd.update(); // Dislay the currently rendered effect on screen.
		escaped = GetAsyncKeyState(VK_ESCAPE); // Check if the user has pressed ESCAPE key on the keybaord.
	}
	return 0; // In C++ we return 0 if nothing went wrong when ending the main method. Anything else is an error code.s
}