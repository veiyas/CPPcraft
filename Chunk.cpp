#include "Chunk.h"
#include "Block.h"
#include "Solid.h"
#include "Air.h"
#include <iostream>

bool Chunk::is_visible()
{

}

void Chunk::add_object(Block* obj)
{
    the_chunk[num_objects] = obj;

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

    //If you try to add more elements than the max size it crashes
}
void Chunk::print_chunk_info()
{
    std::cout << "Number of objects in chunk: " << num_objects << std::endl;
}

void Chunk::create_dummy_chunk()
{
    for(size_t i = 0; i < (LENGTH*WIDTH*HEIGHT); i++)
    {
        add_object(new Solid(length_step, width_step, height_step));
    }
}

void Chunk::render()
{
    for(size_t i = 0; i < num_objects; i++)
    {
        the_chunk[i]->render();
    }


}
