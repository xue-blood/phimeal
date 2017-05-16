#include "vector.h"

#define TailParticle_Level 30
#define TailParticle_FrameInterval 3

typedef void(*render_function)(void);

typedef struct _TailSprite
{
	vec3 pos[TailParticle_Level]; // store sprite position
	vec3 scale;						// use uniform scale
	int  index;					// store current index
	render_function child;		// render callback for child shape
}TailParticle;

void InitTailParticle(TailParticle* tail, vec3* pos);
void RenderTailParticle(TailParticle* tail,vec3* pos);