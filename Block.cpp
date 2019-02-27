#include "Block.h"
#include <iostream>
#include <string>

void Block::set_texture(const char *tex_name)
{
    const std::string temp1 = tex_name;
    const std::string temp_path = "textures/" + temp1 + ".tga";
    const char *path = temp_path.c_str();

    tex = Texture(path);
}

Block::~Block()
{
    delete [] vertexarray;
    delete [] indexarray;
}

bool Block::is_visible()
{
    //
}

void Block::render()
{
    glBindTexture(GL_TEXTURE_2D, tex.texID);
    glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3 * ntris, GL_UNSIGNED_INT, (void*)0);
	// (mode, vertex count, type, element array buffer offset)
	glBindVertexArray(0);
}

void Block::print_info()
{
    printf("TriangleSoup information:\n");
    printf("vertices : %d\n", nverts);
    printf("triangles: %d\n", ntris);
}

Block::Block(const char *tex_name)
{
    set_texture(tex_name);

    // The data array contains 8 floats per vertex:
    // coordinate xyz, normal xyz, texcoords st
    const GLfloat vertex_array_data[] = {
//        -block_size, -block_size, -block_size,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Vertex 0
//         block_size, -block_size, -block_size,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // Vertex 1
//        -block_size,  block_size, -block_size,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  // Vertex 2
//         block_size,  block_size, -block_size,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  // Vertex 3
//        -block_size, -block_size,  block_size,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // Vertex 4
//         block_size, -block_size,  block_size,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Vertex 5
//        -block_size,  block_size,  block_size,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  // Vertex 6
//         block_size,  block_size,  block_size,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f  // Vertex 7

        //topp botten
        -block_size, -block_size, -block_size,   0.0f, -1.0f, 0.0f,   0.75f, 0.33f, // Vertex 0
         block_size, -block_size, -block_size,   0.0f, -1.0f, 0.0f,   1.0f, 0.33f, // Vertex 1
        -block_size,  block_size, -block_size,   0.0f, 1.0f, 0.0f,   0.25f, 0.67f,  // Vertex 2
         block_size,  block_size, -block_size,   0.0f, 1.0f, 0.0f,   0.5f, 0.67f,  // Vertex 3
        -block_size, -block_size,  block_size,   0.0f, -1.0f, 0.0f,   0.75f, 0.67f, // Vertex 4
         block_size, -block_size,  block_size,   0.0f, -1.0f, 0.0f,   1.0f, 0.67f, // Vertex 5
        -block_size,  block_size,  block_size,   0.0f, 1.0f, 0.0f,   0.25f, 0.33f,  // Vertex 6
         block_size,  block_size,  block_size,   0.0f, 1.0f, 0.0f,   0.50f, 0.33f,  // Vertex 7

        //h�ger v�nster
        -block_size, -block_size, -block_size,   -1.0f, 0.0f, 0.0f,   0.0f, 0.67f, // Vertex 0 - 8
         block_size, -block_size, -block_size,   1.0f, 0.0f, 0.0f,   0.75f, 0.67f, // Vertex 1 - 9
        -block_size,  block_size, -block_size,   -1.0f, 0.0f, 0.0f,   0.25f, 0.67f,  // Vertex 2 - 10
         block_size,  block_size, -block_size,   1.0f, 0.0f, 0.0f,   0.5f, 0.67f,  // Vertex 3 - 11
        -block_size, -block_size,  block_size,   -1.0f, 0.0f, 0.0f,   0.0f, 0.33f, // Vertex 4 - 12
         block_size, -block_size,  block_size,   1.0f, 0.0f, 0.0f,   0.75f, 0.33f, // Vertex 5 - 13
        -block_size,  block_size,  block_size,   -1.0f, 0.0f, 0.0f,   0.25f, 0.33f,  // Vertex 6 - 14
         block_size,  block_size,  block_size,   1.0f, 0.0f, 0.0f,   0.50f, 0.33f,  // Vertex 7 - 15
        //fram bak
        -block_size, -block_size, -block_size,   0.0f, 0.0f, -1.0f,   0.25f, 1.0f, // Vertex 0 - 16
         block_size, -block_size, -block_size,   0.0f, 0.0f, -1.0f,   0.5f, 1.0f, // Vertex 1 - 17
        -block_size,  block_size, -block_size,   0.0f, 0.0f, -1.0f,   0.25f, 0.67f,  // Vertex 2 - 18
         block_size,  block_size, -block_size,   0.0f, 0.0f, -1.0f,   0.5f, 0.67f,  // Vertex 3 - 19
        -block_size, -block_size,  block_size,   0.0f, 0.0f, 1.0f,   0.25f, 0.0f, // Vertex 4 - 20
         block_size, -block_size,  block_size,   0.0f, 0.0f, 1.0f,   0.5f, 0.0f, // Vertex 5 - 21
        -block_size,  block_size,  block_size,   0.0f, 0.0f, 1.0f,   0.25f, 0.33f,  // Vertex 6 - 22
         block_size,  block_size,  block_size,   0.0f, 0.0f, 1.0f,   0.50f, 0.33f,  // Vertex 7 - 23
    };
    const GLuint index_array_data[] = {
        6,7,2,
        7,3,2,
        0,1,5,
        0,5,4,

        8,12,14,
        8,14,10,
        13,9,15,
        9,11,15,

        20,21,23,
        20,23,22,
        17,16,18,
        17,18,19

//        0,3,1,
//        0,2,3,
//        1,4,0,
//        1,5,4,
//        4,2,0,
//        4,6,2,
//        1,3,7,
//        1,7,5,
//        7,2,6,
//        7,3,2,
//        4,5,7,
//        4,7,6
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

	// Generate one vertex array object (VAO) and bind it
	glGenVertexArrays(1, &(vao));
	glBindVertexArray(vao);

	// Generate two buffer IDs
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &indexbuffer);

 	// Activate the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
 	// Present our vertex coordinates to OpenGL
	glBufferData(GL_ARRAY_BUFFER,
		8*nverts * sizeof(GLfloat), vertexarray, GL_STATIC_DRAW);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		8*sizeof(GLfloat), (void*)0); // xyz coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		8*sizeof(GLfloat), (void*)(3*sizeof(GLfloat))); // normals
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat))); // texcoords

 	// Activate the index buffer
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
 	// Present our vertex indices to OpenGL
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	 	3*ntris*sizeof(GLuint), indexarray, GL_STATIC_DRAW);

	// Deactivate (unbind) the VAO and the buffers again.
	// Do NOT unbind the index buffer while the VAO is still bound.
	// The index buffer is an essential part of the VAO state.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
