#include "particle.h"
#include <SDL_opengl.h>

void RenderTailParticle(TailParticle* tail, vec3* pos)
{
	if (!tail) return;
	if (!tail->child) return;

	// set position
	tail->pos[tail->index] = *pos;

	tail->index++; if (tail->index >= TailParticle_Level) tail->index = 0;

	// render each child
	glPushMatrix();
	
	for (int i = 0; i < TailParticle_Level; i++)
	{
		glPushMatrix();
		glTranslatef(tail->pos[i].x, tail->pos[i].y, tail->pos[i].z);
		glScalef(tail->scale.x, tail->scale.y, tail->scale.z); 
		
		tail->child();
		glPopMatrix();
	}
	glPopMatrix();
}

void InitTailParticle(TailParticle* tail, vec3* pos)
{
	tail->index = 0;
	
	for (int i = 0; i < TailParticle_Level;i++)
	{
		tail->pos[i] = *pos;
	}
}