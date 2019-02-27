/* texture.hpp */
/* Class to manage an OpenGL texture, and load texture data from a TGA file. */
/* Modified, stripped-down and cleaned-up version of TGA loader from NeHe tutorial 33 */
/* Stefan Gustavson (stefan.gustavson@liu.se 2014-02-28 */

// In MacOS X, tell GLFW to include the modern OpenGL headers.
// Windows does not want this, so we make this Mac-only.
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#endif

// In Linux, tell GLFW to include the modern OpenGL functions.
// Windows does not want this, so we make this Linux-only.
#ifdef __linux__
#define GL_GLEXT_PROTOTYPES
#endif

#include <GLFW/glfw3.h> // For OpenGL typedefs

#include <cstdio>  // For file I/O
#include <cstring> // For memcmp()

#ifdef __WIN32__
#include <GL/glext.h>
#endif

#include "tnm061.hpp" // To have access to glGenerateMipmap() extension in createTexture()


class Texture {

public:

GLuint	width;		// Image width
GLuint	height;		// Image height
GLuint	texID;		// Texture ID for OpenGL
GLuint	type;		// Image type (3 bytes per pixel: GL_RGB, 4 bytes: GL_RGBA)

private:

GLubyte	*imageData;	// Image data (3 or 4 bytes per pixel)
GLuint	bpp;		// Image color depth in bits per pixel

public:

/* Constructor */
Texture();

/* Constructor to load and intialize the texture all at once */
Texture(const char *filename);

/* Destructor */
~Texture();

// The external entry point for loading a texture from a TGA file
void createTexture(const char *filename); // Load GL texture from file

private:

// Internal "private" funtions, called internally by createTexture()
int loadUncompressedTGA(FILE *tgafile); // Load data from an uncompressed TGA file
int loadTGA(const char *filename);		    // Open, check and load a TGA file

};

// Two convenient structs to read and interpret TGA file information
typedef struct
{
	GLubyte Header[12];	// TGA File Header
} TGAHeader;

typedef struct
{
	GLubyte		header[6];								// First 6 useful bytes from the header
	GLuint		bytesPerPixel;							// Holds number of bytes per pixel used in the TGA file
	GLuint		imageSize;								// Data size in bytes
	GLuint		type;
	GLuint		height;									//Height of Image
	GLuint		width;									//Width ofImage
	GLuint		bpp;									// Bits Per Pixel
} TGA;
