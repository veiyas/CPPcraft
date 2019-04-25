#ifndef SOLID_H
#define SOLID_H

#include <Block.h>

class Solid : public Block
{
public:
    Solid(const int &_x, const int &_y, const int &_z);

    ~Solid();

    //Render the block, code from TNM046, calls sub class method through dynamic binding
    void render();
    //Print info about block
    void print_info() const;
    //Translate the cubes vertex coordinates, used to move blocks to correct position in 3D-array
    void translate_block(const float &x, const float &y, const float &z);
    //Finalize for rendering
    void prep_block();
    //Load texture from texture pool
    void load_texture(Texture &the_tex) override;

private:
    //Data
    Texture tex;
    const GLfloat block_size = 1.0f;

    //Methods
    //Set texture used in ctor
    void set_texture(const char *tex_name);
    //Used for dynamic allocation
    void create_block();

    //OpenGL magic
    GLuint vao = 1; // Vertex array object, the main handle for geometry
    int nverts; // Number of vertices in the vertex array
    int ntris;  // Number of triangles in the index array (may be zero)
    GLuint vertexbuffer; // Buffer ID to bind to GL_ARRAY_BUFFER
    GLuint indexbuffer;  // Buffer ID to bind to GL_ELEMENT_ARRAY_BUFFER
    GLfloat *vertexarray; // Vertex array on interleaved format: x y z nx ny nz s t
    GLuint *indexarray;   // Element index array
};

#endif // SOLID_H
