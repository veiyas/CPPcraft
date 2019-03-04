#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <string>
#include "Texture.hpp"
#include <GLFW/glfw3.h>

#ifdef __WIN32__
#include <GL/glext.h>
#endif

#include "tnm061.hpp"  // To be able to use OpenGL extensions

class Block
{
public:
    Block();
    Block(const char *tex_name);
    ~Block();
    Block(const Block&) = delete;
    operator=(const Block&) = delete;

    //Render the block, code from TNM046
    void render();
    //Print info about block
    void print_info() const;
    //Translate the cubes vertex coordinates, used to move blocks to correct position in 3D-array
    void translate_vertices(const float &x, const float &y, const float &z);
    //Finalize for rendering
    void prep_block();

protected:

private:
    //Data
    Texture tex;
    const GLfloat block_size = 1.0f;

    //Methods
    void set_texture(const char *tex_name);

    //OpenGL magic
    GLuint vao = 1; // Vertex array object, the main handle for geometry
    int nverts; // Number of vertices in the vertex array
    int ntris;  // Number of triangles in the index array (may be zero)
    GLuint vertexbuffer; // Buffer ID to bind to GL_ARRAY_BUFFER
    GLuint indexbuffer;  // Buffer ID to bind to GL_ELEMENT_ARRAY_BUFFER
    GLfloat *vertexarray; // Vertex array on interleaved format: x y z nx ny nz s t
    GLuint *indexarray;   // Element index array

};

#endif // BLOCK_H
