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

    if(num_objects == LENGTH*WIDTH*HEIGHT){
        delete obj;
        return;
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

    //If you try to add more elements than the max size it crashes
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
        add_object(temp);
    }
}

void Chunk::render()
{
    for(size_t i = 0; i < num_objects; i++)
    {
        the_chunk[i]->render();
    }


}
