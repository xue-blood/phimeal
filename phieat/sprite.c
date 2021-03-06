#include <SDL_opengl.h>
#include <il/ilu.h>
#include <FreeImage.h>

void SpriteRenderTexture(unsigned int id)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, id);

	float tex_u_max = 1.0f;
	float tex_v_max = 1.0f;
	
	glNormal3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, tex_v_max);			glVertex3f(0.0f, 0.0f, 0.1f);
		glTexCoord2f(tex_u_max, tex_v_max);		glVertex3f(1.0f, 0.0f, 0.1f);
		glTexCoord2f(tex_u_max, 0.0f);			glVertex3f(1.0f, 1.0f, 0.1f);
		glTexCoord2f(0.0f, 0.0f);				glVertex3f(0.0f, 1.0f, 0.1f);
	glEnd();;
}

void* SpriteLoadImage(const wchar_t* filename)
{
	ILuint image_id;
	
	ilGenImages(1, &image_id);
	ilBindImage(image_id);
	if (!ilLoadImage(filename))
		return NULL;

	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}

	return ilGetData();
}

// refer:https://r3dux.org/tag/ilutglloadimage/
// Function load a image, turn it into a texture, and return the texture ID as a GLuint for use
GLuint SpriteLoadTexture(const wchar_t* theFileName)
{
	ILuint imageID;				// Create an image ID as a ULuint

	GLuint textureID;			// Create a texture ID as a GLuint

	ILboolean success;			// Create a flag to keep track of success/failure

	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID

	ilBindImage(imageID); 			// Bind the image

	success = ilLoadImage(theFileName); 	// Load the image file

	// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			printf("Image conversion failed\n");
			return 0;
			//std::cout << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
			//exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureID);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
			0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself
	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		printf("Image load failed\n");
		return 0;
		//std::cout << "Image load failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
		//exit(-1);
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	//std::cout << "Texture creation successful." << std::endl;

	return textureID; // Return the GLuint to the texture so you can use it!
}
