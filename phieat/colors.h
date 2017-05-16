typedef struct _rgba
{
	float r;
	float g;
	float b;
	float a;
}rgba;

typedef struct _Colors
{
	rgba white;
	rgba black;
	rgba gray;

	rgba red;
	rgba green;
	rgba blue;

	rgba darkred;
	rgba darkgreen;
	rgba darkblue;

	rgba cyan;
}Colors;

extern Colors colors;
