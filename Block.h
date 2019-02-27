#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <string>
#include "../Texture.hpp"
#include <GLFW/glfw3.h>

#ifdef __WIN32__
#include <GL/glext.h>
#endif

#include "tnm061.hpp"  // To be able to use OpenGL extensions

class Block
{
public:
    Block(const char *tex_name);
    ~Block();
    Block(const Block& other) = delete;

    void render();
    void print_info();

    bool is_visible();

protected:

private:
    Texture tex;
    void set_texture(const char *tex_name);
    const int block_size = 1;

    GLuint vao; // Vertex array object, the main handle for geometry
    int nverts; // Number of vertices in the vertex array
    int ntris;  // Number of triangles in the index array (may be zero)
    GLuint vertexbuffer; // Buffer ID to bind to GL_ARRAY_BUFFER
    GLuint indexbuffer;  // Buffer ID to bind to GL_ELEMENT_ARRAY_BUFFER
    GLfloat *vertexarray; // Vertex array on interleaved format: x y z nx ny nz s t
    GLuint *indexarray;   // Element index array

};

#endif // BLOCK_H
