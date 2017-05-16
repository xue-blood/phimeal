#ifndef __vector_h__
#define __vector_h__

#ifdef  USE_DOUBLE
typedef double real;
#else
typedef float real;
#endif

typedef struct _vec3
{
	float x, y, z;
}vec3;
#endif // !__vector_h__
