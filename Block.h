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
    Block() = default;
    virtual ~Block() = default;
    Block(const Block&) = delete;
    operator=(const Block&) = delete;

    //Pure virtual functions
    //Render the block, code from TNM046, calls sub class method through dynamic binding
    virtual void render() = 0;
    //Print info about block
    virtual void print_info() const = 0;
    //Translate the cubes vertex coordinates, used to move blocks to correct position in 3D-array
    virtual void translate_block(const float &x, const float &y, const float &z) = 0;
    //Finalize for rendering
    virtual void prep_block() = 0;
};

#endif // BLOCK_H
