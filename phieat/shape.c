#include "shape.h"
#include <math.h>
#include "colors.h"
#include <SDL.h>
#include <SDL_opengl.h>

extern Colors colors;

int circle_precise = 100; // vertex count for simulate a circle

static double *circle_data;



void initCircle()
{
	circle_data = SDL_malloc(2 * ( circle_precise +1)* sizeof(double));
	if(!circle_data)
		appdie("memory allocate failed.");


	// use a quick methon
	double ang_step = (double)360 / (double)circle_precise;
	double T = tan(ang_step / 2 / 180 * 3.14);	// tan of half angle
	double S = 2 * T / (1 + T*T);	// sin of half angle
	
	// default radius is 1
	double x = sin(0);
	double y = cos(0);
	for (int i = 0; i < circle_precise; i++)
	{
		circle_data[2*i] = x;
		circle_data[2 * i + 1] = y;

		//
		// use shear to represent cos and sin function
		//
		x += T*y;
		y -= S*x;
		x += T*y;
	}
	// the end
	circle_data[2 * circle_precise] = 0;
	circle_data[2 * circle_precise + 1] = 1;
}

void drawArc(int _angle_start, int _angle_end)
{
	int s = 0;
	int e = circle_precise;

	glBegin(GL_LINE_STRIP);
	for (int i = s; i <= e;i++)
	{
		glVertex2d(circle_data[2 * i], circle_data[2 * i + 1]);
	}
	glEnd();
}

void drawPieArc(int _angle_start, int _angle_end)
{
	int s = 0;
	int e = circle_precise;

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, 0, 1.0f);
	glVertex2d(0, 0); // center is origin
	for (int i = e; i >=0; i--) // use count clock wise
	{
		glVertex2d(circle_data[2 * i], circle_data[2 * i + 1]);
	}
	glEnd();
}



void initShapes()
{
	initCircle();
}

void clearShapes()
{
	SDL_free(circle_data);
}


void ArcDraw()
{
	drawArc(0, 360);
}

void PieDraw()
{
	drawPieArc(0, 360);
}

void QuadDraw()
{
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1.0f);
	
	glVertex3f(1.0f, 1.0f, 0); // top right
	glVertex3f(-1.0f, 1.0f, 0); // top left
	glVertex3f(-1.0f, -1.0f, 0); // bottom left
	glVertex3f(1.0f, -1.0f, 0); // bottom right
	
	glEnd();
}