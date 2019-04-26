#include "Chunk.h"
#include "Block.h"
#include "Solid.h"
#include "Air.h"
#include <iostream>

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
    obj->load_texture(tex);

    //Visibility checks
    if(obj->x == 0 || obj->y == 0 || obj->z == 0 || obj->x == LENGTH-1 || obj->y == WIDTH-1 || obj->z == HEIGHT-1)
        obj->visible = true;
    else
    {
        if(obj->y - 1 == 0 && obj->x > 0 && obj->x < LENGTH && obj->z > 0 && obj->z < WIDTH)
            obj->visible = false;
    }

    the_chunk[(obj->x * WIDTH * HEIGHT) + (obj->y * WIDTH) + obj->z] = obj;

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
    std::cout << num_objects << "\n";
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
    std::cout << "Number of objects in chunk: " << num_objects << std::endl;
}

void Chunk::create_dummy_chunk(Texture &tex)
{
    for(size_t i = 0; i < (LENGTH*WIDTH*HEIGHT); i++)
    {
        Block *temp = new Solid(length_step, height_step, width_step);
        temp->load_texture(tex);
//        add_object(temp);
    }
}


