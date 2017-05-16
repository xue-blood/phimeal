#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <SDL_thread.h>

#include <FTGL/ftgl.h>

#include <il/ilut.h>


#include "app.h"
#include "colors.h"
#include "shape.h"
#include "particle.h"
#include "sprite.h"


extern int fps;
extern float frame_sec;
extern float delta_sec;


int w, h;
float ratio;

FTGLfont * font;

char buff[128];

TailParticle ball_tail;


#if 1 // phieat data
#define number 6

int current_person;
int pre_person;
int person;
bool change;
float lerp_time;

// for philosopher
float large_radius = .75f;
float phi_raidus = .15f;
float desk_radius = .5f;
float centers_x[number], centers_y[number];
bool is_eating[number];

#define ball_radius .3
float ball_x, ball_y;

// for chopsticks
#define CHOP_LENGTH .3f
float chops_x1[number], chops_y1[number];
float chops_x2[number], chops_y2[number];
bool is_using[number];

// for thread
SDL_Thread* thread;
SDL_mutex*  mutex;
SDL_cond*   cond;
bool aut_go = true;

GLuint img_id;
void * pic;
#endif

int sub_phieat(void*__param)
{
	srand(SDL_GetTicks());

	while (1)
	{
		SDL_Delay(1000);

		SDL_mutexP(mutex);

		
		do
		{	// chose a random person
			int rand_person = rand() % number;
			pre_person = current_person;
			current_person = rand_person;
			lerp_time = 0;

			int left = pre_person - 1;
			if (left < 0) left += number;
			int right = pre_person;
			person = pre_person;
			
			change = false;
			
			// is eating now
			if (is_eating[person])
			{
				// release the chopsticks
				is_using[left] = false;
				is_using[right] = false;

				is_eating[person] = false;
				change = true;
				break;
			}

			// check left and right
			if (is_using[left]) break;
			if (is_using[right]) break;

			// both left and right nobody use them
			is_using[left] = true;
			is_using[right] = true;

			is_eating[person] = true;
			change = true;
		} while (0);

		if (!aut_go)
		{
			SDL_CondWait(cond, mutex);
		}

		SDL_mutexV(mutex);
	}

	return 0;
}

void ui()
{

	// set view for ui
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);


#ifdef APP_SHOW_FPS
	glColor4fv((float*)&colors.cyan);
	sprintf(buff, "%d", fps);

	glPushMatrix();
	glRasterPos3f(-w / 2.0f + 18.0f, h / 2.0f - 27.0f, 0);
	//glTranslatef(-w / 2.0f + 18.0f, h / 2.0f - 27.0f, 0);
	ftglSetFontFaceSize(font, 18, 18);
	ftglRenderFont(font, buff, FTGL_RENDER_ALL);
	glPopMatrix();

#endif

	// tips
	glPushMatrix();
	glLineWidth(1);
	ftglSetFontFaceSize(font, 18, 18);
	
	SDL_mutexP(mutex);
	if (!aut_go)
		glColor4fv((float*)&colors.red);
	else
		glColor4fv((float*)&colors.gray);

	glRasterPos3f(-w / 2.0f + 18.0f, -h / 2.0f + 10, 0);
	ftglRenderFont(font, "space: break run", FTGL_RENDER_ALL);

	if (aut_go)
		glColor4fv((float*)&colors.green);
	else
		glColor4fv((float*)&colors.gray);
	SDL_mutexV(mutex);

	glRasterPos3f(-w / 2.0f + 18.0f, -h / 2.0f + 30, 0);
	ftglRenderFont(font, "return: auto run", FTGL_RENDER_ALL);

	glPopMatrix();
}

float fvLight0[4] = { 0, 0, 1, 1 };

void light()
{

#ifndef APP_USE_LIGHT
	return; // just return

#else
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);



	float vAmbientLightBright[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, vAmbientLightBright);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, fvLight0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, (float*)&colors.black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, (float*)&colors.gray);
	glLightfv(GL_LIGHT0, GL_SPECULAR, (float*)&colors.cyan);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 32);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30);
	

#endif
}

void axis()
{
#ifndef APP_SHOW_AXIS
	return;
#else
	glDisable(GL_LIGHTING);


	glLineWidth(3);
	glBegin(GL_LINES);
	{
		glColor4fv((float*)&colors.red); glVertex3i(0, 0, 0); glVertex3i(1, 0, 0);
		glColor4fv((float*)&colors.green); glVertex3i(0, 0, 0); glVertex3i(0, 1, 0);
		glColor4fv((float*)&colors.blue); glVertex3i(0, 0, 0); glVertex3i(0, 0, 1);
	}
	glEnd();
	glLineWidth(1);

	glEnable(GL_LIGHTING);
#endif
}

void update()
{
	glClearColor(.2f, .2f, .2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(90, 640.0f / 480.0f, 1, 1000);
	//gluOrtho2D(-ratio, ratio, -1, 1);
	glOrtho(-ratio, ratio, -1, 1, .01, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 2.0f,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);


	light();

	axis();

	//glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);


	SDL_mutexP(mutex);

	lerp_time = (lerp_time >= 1) ? 1 : lerp_time + delta_sec;
	float line_time = frame_sec - (int)frame_sec;
	float f = 2 * ((float)fabs(line_time - .5f));

	rgba c = { 0, 0, 0, 1 };

	// desktop
	glLineWidth(4);
	glPushMatrix();
	glScalef(desk_radius, desk_radius, 1);

	glColor4fv((float*)&colors.white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (float*)&colors.gray);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (float*)&colors.cyan);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (float*)&colors.cyan);
	drawCircle();
	glPopMatrix();

	// the philosopher
	for (int i = 0; i < number; ++i)
	{
		if (is_eating[i])
		{
			if (person == i && change)
				lerpfv(&colors.darkgreen, &colors.darkred, &c, lerp_time);
			else
				c = colors.darkred;
		}
		else
		{
			if (person == i && change)
				lerpfv(&colors.darkred, &colors.darkgreen, &c, lerp_time);
			else
				c = colors.darkgreen;
		}

		glColor4fv((float*)&c);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (float*)&c);

		glPushMatrix();
		glTranslatef(centers_x[i], centers_y[i], 0);
		glScalef(phi_raidus, phi_raidus, 1);
		drawPie();
		glPopMatrix();
	}

	// the chopstick
	glLineWidth(12);
	glBegin(GL_LINES);
	glNormal3f(0, 0, 1.0f);
	for (int i = 0; i < number; i++)
	{
		if (is_using[i])
		{
			if (person == i && change)
				lerpfv(&colors.darkgreen, &colors.darkred, &c, lerp_time);
			else
				c = colors.darkred;
		}
		else
		{
			if (person == i && change)
				lerpfv(&colors.darkred, &colors.darkgreen, &c, lerp_time);
			else
				c = colors.darkgreen;
		}

		glColor4fv((float*)&c);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (float*)&c);


		glVertex2f(chops_x1[i], chops_y1[i]);
		glVertex2f(chops_x2[i], chops_y2[i]);
	}
	glEnd();
	glLineWidth(2);

	// jump ball
	float px, py, cx, cy;

	px = centers_x[pre_person] * .8f;
	py = centers_y[pre_person] * .8f + f * .2f;

	cx = centers_x[current_person] * .8f;
	cy = centers_y[current_person] * .8f +f * .2f;

	vec3 pos;
	
	pos.x = lerp(px, cx, lerp_time);
	pos.y = lerp2(py, cy, lerp_time);
	pos.z = 0;
	
	fvLight0[0] = pos.x; fvLight0[1] = pos.y; // move the light

	glDisable(GL_LIGHTING);
	
	// ball tail particle
	//glColor4f(colors.cyan.r, colors.cyan.g, colors.cyan.b, .0f);
	//RenderTailParticle(&ball_tail, &pos);


	glColor4fv((float*)&colors.cyan);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glScalef( .05f, .05f, 1);
	drawPie();
	glPopMatrix();

	//glDrawPixels(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
	

	SDL_mutexV(mutex);
}




void init()
{
	font = ftglCreatePixmapFont("C:\\Windows\\Fonts\\Arial.ttf");
	
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);

	if (!font)
		appdie("Unable create font.");

	initShapes();
	
	// calc position
	int ang = 360 / number;

	// the philosopher
	for (int i = 0; i < number; ++i)
	{
		centers_x[i] = large_radius * cosfa(i *  ang);
		centers_y[i] = large_radius * sinfa(i *  ang);
	}

	// the chopsticks
	for (int i = 0; i < number; ++i)
	{
		chops_x1[i] = desk_radius *  cosfa((i *  ang + ang / 2));
		chops_y1[i] = desk_radius *  sinfa((i *  ang + ang / 2));

		chops_x2[i] = CHOP_LENGTH * chops_x1[i];
		chops_y2[i] = CHOP_LENGTH * chops_y1[i];
	}


	// create a new thread
	thread = SDL_CreateThread(sub_phieat, "Phieat_Thread", NULL);
	if (!thread)
		appdie("Unable create thread.");
	mutex = SDL_CreateMutex();
	cond = SDL_CreateCond();

	// init ball tail particle
	ball_tail.child = PieDraw;
	ball_tail.scale = (vec3){ .01f, .01f, 1 };
	vec3 pos;
	pos.x = centers_x[pre_person];
	pos.y = centers_y[pre_person];
	pos.z = 0;
	InitTailParticle(&ball_tail, &pos);

	// load texture
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	ilutInit();
	
	//SpriteLoadImage(L"img_test.png");
}

void clear()
{
	ilShutDown();

	clearShapes();
	ftglDestroyFont(font);

	SDL_DestroyMutex(mutex);
	SDL_DestroyCond(cond);
}


void resize(int _w,int _h)
{
	w = _w;
	h = _h;
	ratio = (float)w / (float)h;

	glViewport(0, 0, w, h);


	

}


void keydown(SDL_Keysym key)
{
	switch (key.sym)
	{
	case SDLK_SPACE:
		SDL_LockMutex(mutex);
			
		aut_go = false;
		SDL_CondSignal(cond);
		SDL_UnlockMutex(mutex);
		break;
	case SDLK_RETURN:
		SDL_LockMutex(mutex);

		aut_go = true;
		SDL_CondSignal(cond);
		SDL_UnlockMutex(mutex);
		break;

	default:
		break;
	}
}