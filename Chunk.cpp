#include "Chunk.h"
#include "Block.h"
#include "Solid.h"
#include "Air.h"
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

void render_block(Block * b)
{
    b->render();
}

Chunk::Chunk(const int & x, const int & y, const int & z)
    :mesh{Mesh(x,y,z)}, MAX_SIZE{mesh.get_size()}
{
    the_pool = TexturePool();
}

void Chunk::set_visibility(Block *obj)
{

}

void Chunk::add_object()
{
    //Check if chunk is full
    //if(num_objects == LENGTH*WIDTH*HEIGHT) beautiful bug
    if(num_objects == MAX_SIZE) return;

    //Create temp vars to put into vector
    Shell temp_shell = mesh(num_objects);
    Block * temp_block = new Solid(temp_shell.x, temp_shell.y, temp_shell.z);

    //Load texture
    temp_block->load_texture(the_pool("Grass"));

    chunk.push_back(temp_block);

    ++num_objects;
}

Block * Chunk::access_block(const int &x, const int &y, const int &z)
{
//    return the_chunk[(x * WIDTH * HEIGHT) + (z * HEIGHT) + y];
}

void Chunk::render()
{
    std::for_each(std::begin(chunk), std::end(chunk), render_block);
}

void Chunk::print_chunk_info()
{
    std::cout << "Number of objects in chunk: " << num_objects << "\n";

}


