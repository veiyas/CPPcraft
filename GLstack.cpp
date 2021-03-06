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
#include "Solid.h"
#include "Air.h"
#include "Mesh.h"
#include "PerlinNoise.hpp"

#include <functional>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <algorithm>


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

typedef std::chrono::high_resolution_clock Clock;


//Local function declarations
void setupViewport(GLFWwindow *window, GLfloat *P);
void create_perspective_matrix(float M[], const float &vfov, const float &aspect, const float &znear, const float &zfar);
void poll_keyboard_input(GLFWwindow *window, float &x, float &y, float &z, float &phi, float &theta);
void render_chunk_object(Chunk & c);
std::map<std::string, Texture> create_texture_pool();

const float MOVE_SPEED = 1.0f;

int main(int argc, char *argv[])
{
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
    window = glfwCreateWindow(vidmode->width, vidmode->height, "C++Craft", nullptr, nullptr);
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

/********************************************************
                        TEST AREA
********************************************************/
    auto t1 = Clock::now();

    std::vector<Chunk> world;

    for(size_t i=0; i < 4; i++)
    {
        for(size_t j=0; j < 4; j++)
        {
            world.push_back(Chunk(i,0,j,true));
        }
    }

    auto t2 = Clock::now();
    std::cout << "\nWorld constructed in " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " milliseconds\n";
/********************************************************
********************************************************/

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
            // Camera movement, rotate local camera position then translate (move) the camera
            MVstack.rotX(rotator.theta);
            MVstack.rotY(rotator.phi);
            MVstack.translate(x_move - 8, y_move - 8, z_move - 50);
            glUniformMatrix4fv( location_MV, 1, GL_FALSE, MVstack.getCurrentMatrix());

        MVstack.pop(); // Restore the initial, untouched matrix

/********************************************************
                RENDERING CODE GOES HERE
********************************************************/

//        for(size_t i=0; i < world.size(); i++)
//        {
//            world[i].add_object();
//            world[i].render();
//        }

        std::for_each(world.begin(), world.end(), render_chunk_object);

/********************************************************

********************************************************/

		// Play nice and deactivate the shader program
		glUseProgram(0);

		// Swap buffers, i.e. display the image and prepare for next frame.
        glfwSwapBuffers(window);

		// Poll events (mouse input)
		glfwPollEvents();

        // Keyboard poll
        poll_keyboard_input(window, x_move, y_move, z_move, rotator.phi, rotator.theta);

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

void poll_keyboard_input(GLFWwindow *window, float &x, float &y, float &z, float &phi, float &theta)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
          glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else
    {
        float z_move = MOVE_SPEED*cos(phi);
        float x_move = MOVE_SPEED*sin(phi);

        if(glfwGetKey(window, GLFW_KEY_W)) {
          z += z_move;
          x -= x_move;
        }
        if(glfwGetKey(window, GLFW_KEY_S)) {
          z -= z_move;
        }
        if(glfwGetKey(window, GLFW_KEY_D)) {
          x -= MOVE_SPEED;
        }
        if(glfwGetKey(window, GLFW_KEY_A)) {
          x += MOVE_SPEED;
        }
        if(glfwGetKey(window, GLFW_KEY_SPACE)) {
          y -= MOVE_SPEED;
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
          y += MOVE_SPEED;
        }
    }
}

void render_chunk_object(Chunk & c)
{
    c.add_object();
    c.render();
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
