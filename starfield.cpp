#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <stdlib.h>

#include <windows.h>

#include "includes/starfield.h"
#include "includes/main.h"

DWORD lastpro = 0;

void initstarfield(float field[STAR_NUM][6])
{
	int i = 0;

	while (i != STAR_NUM - 1)
	{
		field[i][0] = ((float) ((float) (rand() % 2000)) / 1000.0) - 1.0  ;
		field[i][1] = ((float) ((float) (rand() % 2000)) / 1000.0) - 1.0  ;
		field[i][2] = ((float) ((float) (rand() % 4000)) / 1000.0) - 4.0  ;

		field[i][3] = ((float) ((float) (rand() % 100)) / 1000.0) + 0.7;
		field[i][4] = ((float) ((float) (rand() % 100)) / 1000.0) + 0.7;
		field[i][5] = ((float) ((float) (rand() % 100)) / 1000.0) + 0.7	;

//		Sleep(5);
//		printDebug("X: %1.3f, Y: %1.3f, Z: %1.3f\n", field[i][0], field[i][1], field[i][2]);
			
		i++;
	}
}

void starfield(DWORD progress, float myfield[STAR_NUM][6],  Texture &star, DWORD rotation)
{
	int i = 0;
	float addfactor = 0.0;
	
	glPushMatrix();
	//gluPerspective(90, (float)(4.0f / 3.0f), 0.01, 3.0);
	gluPerspective(90, 960.0/540.0, 0.01, 3.0);

	gluLookAt(0,0,-1, 0,0,-2,      
		0 + sin( (( float) rotation / 1000.0)) ,	
		0 + cos( (( float) rotation / 1000.0)),  
		0.0 + (float) ( (float) rotation / 1000.0));

	

	while ((progress - lastpro) > 10)
	{
		addfactor = addfactor + 0.0065;
		lastpro = lastpro + 10;
	}
	
	star.enable();
	glBegin(GL_QUADS);

	while (i != STAR_NUM)
	{
		float yfac = 1.0 + (myfield[i][2] / 3.0);

		glColor3f(myfield[i][3] * yfac, myfield[i][4] * yfac , myfield[i][5] * yfac);

		glTexCoord2f(0,0);
		glVertex3f(myfield[i][0],		myfield[i][1],		myfield[i][2]);
		glTexCoord2f(0,1);
		glVertex3f(myfield[i][0],		myfield[i][1] - 0.01,myfield[i][2]);
		glTexCoord2f(1,1);
		glVertex3f(myfield[i][0] + 0.01,	myfield[i][1] - 0.01,myfield[i][2]);
		glTexCoord2f(1,0);
		glVertex3f(myfield[i][0] + 0.01,	myfield[i][1],		myfield[i][2]);
	

//		printDebug("x: %1.2f, y: %1.2f, z: %1.2f\n", myfield[i][0], myfield[i][1], myfield[i][2]);

		myfield[i][2] = myfield[i][2] + addfactor ;
			
		if (myfield[i][2] > 0.5)
		{
			myfield[i][0] = ((float) ((float) (rand() % 2400)) / 1000.0) - 1.0  ;
			myfield[i][1] = ((float) ((float) (rand() % 2000)) / 1000.0) - 1.0  ;
			myfield[i][2] = -4.0; 
		}
		i++;
	}
	glEnd();
	star.disable();

	glPopMatrix();

}

