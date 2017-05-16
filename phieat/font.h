
#include <map>
#include <string>
#include <FTGL/ftgl.h>
#include <SDL.h>

using namespace std;

typedef map<string, FTFont*> FontList;
typedef FontList::const_iterator FontIter;

class FTGLFontManager
{
public:
	// NOTE
	// This is shown here for brevity. The implementation should be in the source
	// file otherwise your compiler may inline the function resulting in
	// multiple instances of FTGLFontManager
	static FTGLFontManager& Instance()
	{
		static FTGLFontManager tm;
		return tm;
	}

	~FTGLFontManager()
	{
		FontIter font;
		for (font = fonts.begin(); font != fonts.end(); font++)
		{
			delete (*font).second;
		}

		fonts.clear();
	}


	FTFont* GetFont(const char *filename, int size)
	{
		char buf[256];
		sprintf(buf, "%s%i", filename, size);
		string fontKey = string(buf);

		FontIter result = fonts.find(fontKey);
		if (result != fonts.end())
		{
			//LOGMSG("Found font %s in list", filename);
			return result->second;
		}
		char *path = SDL_GetBasePath();
		string fullname = path + string(filename);

		FTFont* font = new FTTextureFont(fullname.c_str());


		if (!font)
		{
			//LOGERROR("Font %s failed to open", fullname.c_str());
			delete font;
			return NULL;
		}

		if (!font->FaceSize(size))
		{
			//LOGERROR("Font %s failed to set size %i", filename, size);
			delete font;
			return NULL;
		}

		fonts[fontKey] = font;

		return font;
	}


private:
	// Hide these 'cause this is a singleton.
	FTGLFontManager(){}
	FTGLFontManager(const FTGLFontManager&){};
	FTGLFontManager& operator = (const FTGLFontManager&){ return *this; };

	// container for fonts
	FontList fonts;
};