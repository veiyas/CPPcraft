#include "Solid.h"

Solid::Solid(const int &_x, const int &_y, const int &_z)
{
    create_block();

    x = _x;
    y = _y;
    z = _z;

    translate_block(_x,_y,_z);
}

Solid::~Solid()
{
    delete [] vertexarray;
    delete [] indexarray;
}

void Solid::translate_block(const float &x, const float &y, const float &z)
{
    for(size_t i = 0; i < 8*nverts; i += 8)
    {
        vertexarray[i]   += (2*x);
        vertexarray[i+1] += (2*y);
        vertexarray[i+2] += (2*z);
        vertexarray[i+3] += (2*x);
        vertexarray[i+4] += (2*y);
        vertexarray[i+5] += (2*z);
    }

    prep_block();
}

void Solid::print_info() const
{
    printf("TriangleSoup information:\n");
    printf("Vertices : %d\n", nverts);
    printf("Triangles: %d\n", ntris);
}

void Solid::load_texture(Texture &the_tex)
{
    tex_id = the_tex.texID;
}

void Solid::prep_block()
{
    // Generate one vertex array object (VAO) and bind it
	glGenVertexArrays(1, &(vao));
	glBindVertexArray(vao);

	// Generate two buffer IDs
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &indexbuffer);

 	// Activate the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
 	// Present our vertex coordinates to OpenGL
	glBufferData(GL_ARRAY_BUFFER, 8*nverts * sizeof(GLfloat), vertexarray, GL_STATIC_DRAW);
	// Specify how many attribute arrays we have in our VAO
	glEnableVertexAttribArray(0); // Vertex coordinates
	glEnableVertexAttribArray(1); // Normals
	glEnableVertexAttribArray(2); // Texture coordinates
	// Specify how OpenGL should interpret the vertex buffer data:
	// Attributes 0, 1, 2 (must match the lines above and the layout in the shader)
	// Number of dimensions (3 means vec3 in the shader, 2 means vec2)
	// Type GL_FLOAT
	// Not normalized (GL_FALSE)
	// Stride 8 floats (interleaved array with 8 floats per vertex)
	// Array buffer offset 0, 3 or 6 floats (offset into first vertex)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), nullptr); // xyz coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,	8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat))); // normals
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,	8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat))); // texcoords

 	// Activate the index buffer
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
 	// Present our vertex indices to OpenGL
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*ntris*sizeof(GLuint), indexarray, GL_STATIC_DRAW);

	// Deactivate (unbind) the VAO and the buffers again.
	// Do NOT unbind the index buffer while the VAO is still bound.
	// The index buffer is an essential part of the VAO state.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Solid::render()
{
    if(visible)
    {
        //Bind texture
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glBindVertexArray(vao);

        //Render
        glDrawElements(GL_TRIANGLES, 3 * ntris, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glBindBuffer(0, 0);
        glBindTexture(0, 0);
    }
    else
        return;
}

void Solid::create_block()
{
    // The data array contains 8 floats per vertex:
    // coordinate xyz, normal xyz, texcoords st
    const GLfloat vertex_array_data[] = {
         //Back face
        -block_size, -block_size, -block_size,   0.0f, 0.0f,-1.0f,   0.0f, 1.0f/3.0f,  // Vertex 0
         block_size, -block_size, -block_size,   0.0f, 0.0f,-1.0f,   1.0f, 1.0f/3.0f,  // Vertex 1
        -block_size,  block_size, -block_size,   0.0f, 0.0f,-1.0f,   0.0f, 2.0f/3.0f,  // Vertex 2
         block_size,  block_size, -block_size,   0.0f, 0.0f,-1.0f,   1.0f, 2.0f/3.0f,  // Vertex 3
         //Front face
        -block_size, -block_size,  block_size,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f/3.0f,  // Vertex 4
         block_size, -block_size,  block_size,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f/3.0f,  // Vertex 5
        -block_size,  block_size,  block_size,   0.0f, 0.0f, 1.0f,   0.0f, 2.0f/3.0f,  // Vertex 6
         block_size,  block_size,  block_size,   0.0f, 0.0f, 1.0f,   1.0f, 2.0f/3.0f,  // Vertex 7
         //Right face
         block_size, -block_size,  block_size,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f/3.0f,  // Vertex 5 - 8
         block_size, -block_size, -block_size,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f/3.0f,  // Vertex 1 - 9
         block_size,  block_size,  block_size,   1.0f, 0.0f, 0.0f,   0.0f, 2.0f/3.0f,  // Vertex 7 - 10
         block_size,  block_size, -block_size,   1.0f, 0.0f, 0.0f,   1.0f, 2.0f/3.0f,  // Vertex 3 - 11
         //Left face
        -block_size, -block_size, -block_size,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f/3.0f,  // Vertex 0 - 12
        -block_size, -block_size,  block_size,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f/3.0f,  // Vertex 4 - 13
        -block_size,  block_size, -block_size,  -1.0f, 0.0f, 0.0f,   0.0f, 2.0f/3.0f,  // Vertex 2 - 14
        -block_size,  block_size,  block_size,  -1.0f, 0.0f, 0.0f,   1.0f, 2.0f/3.0f,  // Vertex 6 - 15
         //Top face
        -block_size,  block_size,  block_size,   0.0f, 1.0f, 0.0f,   0.0f,  2.0f/3.0f, // Vertex 6 - 16
         block_size,  block_size,  block_size,   0.0f, 1.0f, 0.0f,   1.0f,  2.0f/3.0f, // Vertex 7 - 17
        -block_size,  block_size, -block_size,   0.0f, 1.0f, 0.0f,   0.0f,  1.0f,      // Vertex 2 - 18
         block_size,  block_size, -block_size,   0.0f, 1.0f, 0.0f,   1.0f,  1.0f,      // Vertex 3 - 19
         //Bottom face
        -block_size, -block_size, -block_size,   0.0f,-1.0f, 0.0f,   0.0f,  0.0f,      // Vertex 0 - 20
         block_size, -block_size, -block_size,   0.0f,-1.0f, 0.0f,   1.0f,  0.0f,      // Vertex 1 - 21
        -block_size, -block_size,  block_size,   0.0f,-1.0f, 0.0f,   0.0f,  1.0f/3.0f, // Vertex 4 - 22
         block_size, -block_size,  block_size,   0.0f,-1.0f, 0.0f,   1.0f,  1.0f/3.0f, // Vertex 5 - 23
    };
    const GLuint index_array_data[] = {
        //Back face
        0,2,1,
        1,2,3,
        //Front face
        4,5,6,
        5,7,6,
        //Right face
        8,9,10,
        9,11,10,
        //Left face
        12,15,14,
        13,15,12,
        //Top face
        16,19,18,
        17,19,16,
        //Bottom face
        20,21,23,
        20,23,22
    };

    nverts = 24;
    ntris = 12;

    vertexarray = new GLfloat[8*nverts];
    indexarray = new GLuint[3*ntris];

    for(int i=0; i<8*nverts; i++) {
        vertexarray[i]=vertex_array_data[i];
    }
    for(int i=0; i<3*ntris; i++) {
        indexarray[i]=index_array_data[i];
    }
}

