/*
 * A C++ framework for OpenGL programming in TNM061 for MT1 2014.
 *
 * This is a small, limited framework, designed to be easy to use
 * for students in an introductory computer graphics course in
 * the second year of a M Sc curriculum. It uses custom code
 * for some things that are better solved by external libraries
 * like GLEW and GLM, but the emphasis is on simplicity and
 * readability, not generality.
 * For the window management, GLFW 3.0 is used for convenience.
 * The framework should work in Windows, MacOS X and Linux.
 * Some Windows-specific stuff for extension loading is still
 * here. GLEW could have been used instead, but for clarity
 * and minimal dependence on other code, we rolled our own extension
 * loading for the things we need. That code is short-circuited on
 * platforms other than Windows. This code is dependent only on
 * the GLFW and OpenGL libraries. OpenGL 3.3 or higher is required.
 *
 * Author: Stefan Gustavson (stegu@itn.liu.se) 2013-2014
 * This code is in the public domain.
 */

// System utilities
#include <cstdio>
#include <cstdlib>
#include <cmath>

#ifndef M_PI
#define M_PI (3.141592653589793)
#endif

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

// GLFW 3.x, to handle the OpenGL window
#include <GLFW/glfw3.h>

// Windows installations usually lack an up-to-date OpenGL extension header,
// so make sure to supply your own, or at least make sure it's of a recent date.
#ifdef __WIN32__
#include <GL/glext.h>
#endif

// Headers for the other source files that make up this program.
#include "tnm061.hpp"
#include "Shader.hpp"
//#include "Texture.hpp"
#include "TriangleSoup.hpp"
#include "Rotator.hpp"
#include "MatrixStack.hpp"
#include "Block.h"
#include "Chunk.h"

/*
 * setupViewport() - set up the OpenGL viewport.
 * This should be done for each frame, to handle window resizing.
 * The "proper" way would be to set a "resize callback function"
 * using glfwSetWindowSizeCallback() and do these few operations
 * only when something changes, but let's keep it simple.
 * Besides, we want to change P when the aspect ratio changes.
 * A callback function would require P to be changed indirectly
 * in some manner, which is somewhat awkward in this case.
 */
//Local function declarations
void setupViewport(GLFWwindow *window, GLfloat *P);
void create_perspective_matrix(float M[], const float &vfov, const float &aspect, const float &znear, const float &zfar);
const float move_speed = 0.05;
void poll_keyboard_input(GLFWwindow *window, float &x, float &y, float &z);

/*
 * main(argc, argv) - the standard C entry point for the program
 */
int main(int argc, char *argv[]) {

	TriangleSoup earthSphere;
    Texture earthTexture;
    Shader the_shader;

 	GLint location_time, location_MV, location_P, location_tex; // Shader uniforms
    float time;
	double fps = 0.0;

    MatrixStack MVstack; // The matrix stack we are going to use to set MV

    const GLFWvidmode *vidmode;  // GLFW struct to hold information about the display
	GLFWwindow *window;    // GLFW struct to hold information about the window

	MouseRotator rotator;

    // Initialise GLFW
    glfwInit();

    // Determine the desktop size
    vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	// Make sure we are getting a GL context of at least version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Exclude old legacy cruft from the context. We don't need it, and we don't want it.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Open a square window (aspect 1:1) to fill half the screen height
    window = glfwCreateWindow(vidmode->height/2, vidmode->height/2, "GLprimer", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate(); // No window was opened, so we can't continue in any useful way
        return -1;
    }

    // Make the newly created window the "current context" for OpenGL
    // (This step is strictly required, or things will simply not work)
    glfwMakeContextCurrent(window);

	rotator.init(window);

    // Load the extensions for GLSL - note that this has to be done
    // *after* the window has been opened, or we won't have a GL context
    // to query for those extensions and connect to instances of them.
    tnm061::loadExtensions();

    printf("GL vendor:       %s\n", glGetString(GL_VENDOR));
    printf("GL renderer:     %s\n", glGetString(GL_RENDERER));
    printf("GL version:      %s\n", glGetString(GL_VERSION));
    printf("Desktop size:    %d x %d pixels\n", vidmode->width, vidmode->height);

    glfwSwapInterval(0); // Do not wait for screen refresh between frames

	// Perspective projection matrix
    GLfloat P[16];
    create_perspective_matrix(P, 1.0f, 1.0f, 0.0f, 100.0f);

    // Intialize the matrix to an identity transformation
    MVstack.init();

	// Create geometry for rendering
//	Block test{"textures/earth.tga"};
//    test.translate_vertices(1.0f, 1.0f, 0);
//    test.print_info();
    Block test2{"textures/earth.tga"};

	// Create a shader program object from GLSL code in two files
	the_shader.createShader("vertexshader.glsl", "fragmentshader.glsl");

	glEnable(GL_TEXTURE_2D);

	location_MV = glGetUniformLocation( the_shader.programID, "MV" );
	location_P = glGetUniformLocation( the_shader.programID, "P" );
	location_time = glGetUniformLocation( the_shader.programID, "time" );
	location_tex = glGetUniformLocation( the_shader.programID, "tex" );

	float x_move = 0;
	float y_move = 0;
	float z_move = 0;

    Chunk testchunk{};

    Block testblock("textures/test_tex.tga");
    testblock.prep_block();

    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        // Calculate and update the frames per second (FPS) display
        fps = tnm061::displayFPS(window);

		// Set the clear color and depth, and clear the buffers for drawing
        glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST); // Use the Z buffer
		glEnable(GL_CULL_FACE);  // Use back face culling
		glCullFace(GL_BACK);

        // Set up the viewport
        setupViewport(window, P);

		// Handle mouse input
		rotator.poll(window);

		// Activate our shader program.
		glUseProgram( the_shader.programID );

        // Copy the projection matrix P into the shader.
		glUniformMatrix4fv( location_P, 1, GL_FALSE, P );

		// Update the uniform time variable.
		time = (float)glfwGetTime(); // Needed later as well
        glUniform1f( location_time, time );

        // We use MatrixStack code to produce prototype faster
        MVstack.push();
            // Camera movement
            MVstack.translate(0.0f + x_move, 0.0f + y_move, -6.0f + z_move);
            MVstack.rotX(rotator.theta);
            MVstack.rotY(rotator.phi);
            glUniformMatrix4fv( location_MV, 1, GL_FALSE, MVstack.getCurrentMatrix() );

        MVstack.pop(); // Restore the initial, untouched matrix

//        testchunk.render();
        testblock.render();

		// Play nice and deactivate the shader program
		glUseProgram(0);

		// Swap buffers, i.e. display the image and prepare for next frame.
        glfwSwapBuffers(window);

		// Poll events (read keyboard and mouse input)
		glfwPollEvents();

        // Keyboard poll
        poll_keyboard_input(window, x_move, y_move, z_move);

    }

    // Close the OpenGL window and terminate GLFW.
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void create_perspective_matrix(float M[], const float &vfov, const float &aspect, const float &znear, const float &zfar)
{
    float f = (cos(vfov/2)) / (sin(vfov/2));
    float z1 = -1 * ((zfar + znear)/(zfar-znear));
    float z2 = -1 * ((2*znear*zfar)/(zfar-znear));

    float MTemp[16] = {
        (f/aspect) , 0.0f     , 0.0f    , 0.0f ,
        0.0f       , f        , 0.0f    , 0.0f ,
        0.0f       , 0.0f     , z1      ,-1.0f ,
        0.0f       , 0.0f     , z2      , 1.0f
    };

    for(int i=0; i < 16; i++){
        M[i] = MTemp[i];
    }
}

void poll_keyboard_input(GLFWwindow *window, float &x, float &y, float &z)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
          glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if(glfwGetKey(window, GLFW_KEY_W)) {
      z += move_speed;
    }
    if(glfwGetKey(window, GLFW_KEY_S)) {
      z -= move_speed;
    }
    if(glfwGetKey(window, GLFW_KEY_D)) {
      x -= move_speed;
    }
    if(glfwGetKey(window, GLFW_KEY_A)) {
      x += move_speed;
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE)) {
      y -= move_speed;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
      y += move_speed;
    }
}

void setupViewport(GLFWwindow *window, GLfloat *P) {

    int width, height;

    // Get window size. It may start out different from the requested
    // size, and will change if the user resizes the window.
    glfwGetWindowSize( window, &width, &height );

    // Ugly hack: adjust perspective matrix for non-square aspect ratios
    P[0] = P[5]*height/width;

    // Set viewport. This is the pixel rectangle we want to draw into.
    glViewport( 0, 0, width, height ); // The entire window
}
