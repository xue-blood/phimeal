#include <SDL.h>
#include <SDL_opengl.h>

#if 1 // custom you program by this define

// lock fps to 30
// comment this to get 60 fps
//#define APP_LOCK_FPS

// uncomment this to get unlimited fps
// but the monitor cant get higher than 60
//#define APP_UNLIMIT_FPS 

// comment this to hidden fps
#define APP_SHOW_FPS

// comment this to disable light
#define APP_USE_LIGHT

// show aixs in world
//#define APP_SHOW_AXIS

#define PROGRAM_NAME "Philosopher Meal"


#endif


#if 1 // app
/* A simple function that prints a message, the error code returned by SDL,
* and quits the application */
void appdie(const char *msg);
#endif



#if 1 // app callback
void update();
void ui();
void resize(int w,int h);
void init();
void clear();

void keydown(SDL_Keysym);
#endif

#if 1 // math function for angle
#define Deg2Rad (3.14 / 180)
#define sinfa(angle) sinf((angle) * (float)Deg2Rad)
#define cosfa(angle) cosf((angle) * (float)Deg2Rad)

#define lerp(a,b,t)  ((a) + ((b)-(a))*(t))
#define lerp2(a,b,t) ((t)*(t)*((a)+(b)) - 2*(a)*(t) +(a))
#define lerpfv(a,b,r,t) do{\
						((float*)r)[0] = lerp(((float*)a)[0],((float*)b)[0],t);\
					    ((float*)r)[1] = lerp(((float*)a)[1],((float*)b)[1],t);\
					    ((float*)r)[2] = lerp(((float*)a)[2],((float*)b)[2],t);}while(0)
#define lerp2fv(a,b,r,t) do{\
						((float*)r)[0] = lerp2(((float*)a)[0],((float*)b)[0],t);\
					    ((float*)r)[1] = lerp2(((float*)a)[1],((float*)b)[1],t);\
					    ((float*)r)[2] = lerp2(((float*)a)[2],((float*)b)[2],t);}while(0)
#endif