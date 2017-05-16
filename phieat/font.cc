#include "font.h"
#include "colors.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_thread.h>
#include <gl/GLU.h>
#include <stdio.h>

#include "app.h"

FTTextureFont* myFont = (FTTextureFont*)FTGLFontManager::Instance().GetFont("arial.ttf", 72);



	extern "C" extern int w, h;
	extern "C" extern int fps;
	extern "C" extern bool aut_go;
	char buff[128];

	extern "C" 	extern SDL_mutex* mutex;


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
	glTranslatef(-w / 2.0f + 18.0f, h / 2.0f - 27.0f, 0);
	myFont->Render(buff);
	glPopMatrix();

#endif

	// tips
	glPushMatrix();
	glLineWidth(1);

	SDL_mutexP(mutex);
	if (!aut_go)
		glColor4fv((float*)&colors.red);
	else
		glColor4fv((float*)&colors.gray);

	glTranslatef(-w / 2.0f + 18.0f, -h / 2.0f + 10, 0);
	myFont->Render("space: break run");
	if (aut_go)
		glColor4fv((float*)&colors.green);
	else
		glColor4fv((float*)&colors.gray);
	SDL_mutexV(mutex);

	glTranslatef(0, 20, 0);
	myFont->Render("return: auto run");
	//ftglRenderFont(font, "return: auto run", FTGL_RENDER_ALL);

	glPopMatrix();
}
