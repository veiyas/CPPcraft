#include "Chunk.h"
#include "Block.h"
#include "Solid.h"
#include "Air.h"
#include <iostream>

Chunk::Chunk()
{
    //Initialize with nullptrs
    the_chunk = new Block*[LENGTH*WIDTH*HEIGHT]{nullptr};
    tex = Texture("textures/sand.tga");
}

void Chunk::set_visibility()
{

}

void Chunk::add_object()
{
    //Size check
    if(num_objects == LENGTH*WIDTH*HEIGHT){
        return;
    }

    Block *obj = new Solid(length_step, height_step, width_step);

    //Visibility checks for cube only
    if(obj->x == 0 || obj->y == 0 || obj->z == 0 || obj->x == LENGTH-1 || obj->y == WIDTH-1 || obj->z == HEIGHT-1)
    {
        the_chunk[(obj->x * WIDTH * HEIGHT) + (obj->y * WIDTH) + obj->z] = obj;
        obj->load_texture(tex);
    }
    else
    {
        delete obj;
    }

    ++length_step;

    if(length_step == LENGTH)
    {
        ++width_step;
        length_step = 0;
    }

    if(width_step == WIDTH)
    {
        ++height_step;
        width_step = 0;
    }

    ++num_objects;
}

Block * Chunk::access_block(const int &x, const int &y, const int &z)
{
    return the_chunk[(x * WIDTH * HEIGHT) + (y * WIDTH) + z];
}

void Chunk::render()
{
    for(size_t i = 0; i < num_objects; i++)
    {
        if(the_chunk[i] != nullptr)
            the_chunk[i]->render();
        else
            continue;
    }
}

void Chunk::print_chunk_info()
{
    std::cout << "Number of objects in chunk: " << num_objects << ", size: " << sizeof(**the_chunk) << "\n";

}


