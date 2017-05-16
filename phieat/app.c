#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

#include <FTGL/ftgl.h>


#include "app.h"
#include "colors.h"



int fps;
int start_msec;
int delta_msec;
float delta_sec;
float frame_sec;


SDL_Window *mainwindow; /* Our window handle */
SDL_GLContext maincontext; /* Our opengl context handle */



static int old_tick;
static int current_tick;
void mainloop()
{
	// Main loop
	
	SDL_Event event;
	old_tick = SDL_GetTicks();
	start_msec = old_tick;

	while (1)
	{
#ifdef APP_LOCK_FPS
		SDL_Delay(33);
#endif
		// Check for messages
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
			{
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
				{
					int w, h;
					SDL_GL_GetDrawableSize(mainwindow, &w, &h);
					resize(w, h);
				}
					break;
				default: break;
				}
			}
			break;

			case SDL_KEYDOWN:
				keydown(event.key.keysym);
				break;

			case SDL_QUIT:	return;
			default:		break;
			}
		}

		current_tick = SDL_GetTicks();
		delta_msec = (current_tick - old_tick);
		old_tick = current_tick;
		fps = delta_msec ? 1000 / delta_msec : 1000;
		delta_sec = (float)delta_msec / 1000;
		frame_sec = (float)(current_tick - start_msec) / 1000;
		// Game loop will go here
		
		// draw scene
		update(); 

		// draw ui layel
		ui(); 

		SDL_GL_SwapWindow(mainwindow);
	}
}



/* Our program's entry point */
int main(int argc, char *argv[])
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
		appdie("Unable to initialize SDL"); /* Or die on error */

	init();

	/* Request opengl 3.2 context.
	* SDL doesn't have the ability to choose which profile at this time of writing,
	* but it should default to the core profile */
// 	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
// 	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	/* Turn on double buffering with a 24bit Z buffer.
	* You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// antialias
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	

	/* Create our window centered at 512x512 resolution */
	mainwindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!mainwindow) /* Die if creation failed */
		appdie("Unable to create window");

	/* Create our opengl context and attach it to our window */
	maincontext = SDL_GL_CreateContext(mainwindow);


#ifndef APP_UNLIMIT_FPS & !APP_LOCK_FPS
	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);
#endif // APP_UNLIMIT_FPS & !APP_LOCK_FPS

	int w, h;
	SDL_GL_GetDrawableSize(mainwindow, &w, &h);
	resize(w, h);
	
	mainloop();

	clear();

	/* Delete our opengl context, destroy our window, and shutdown SDL */
	SDL_GL_DeleteContext(maincontext);
	SDL_DestroyWindow(mainwindow);
	SDL_Quit();

	return 0;
}

void appdie(const char *msg)
{
	printf("%s: %s\n", msg, SDL_GetError());
	SDL_Quit();
	exit(1);
}