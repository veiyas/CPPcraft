#include "Chunk.h"
#include "Block.h"
#include "Solid.h"
#include "Air.h"
#include <iostream>
#include <vector>

Chunk::Chunk(const int pos_x, const int pos_y, const int pos_z)
    : x_offset{pos_x*LENGTH}, y_offset{pos_y*HEIGHT}, z_offset{pos_z*WIDTH}
{
    //Initialize with nullptrs
    the_chunk = new Block*[LENGTH*WIDTH*HEIGHT]{nullptr};
    the_pool = TexturePool();
}

void Chunk::set_visibility()
{

}

void Chunk::add_object()
{
    //Size check
    if(num_objects == LENGTH*WIDTH*HEIGHT){return;}

    //Create perlin sample with block position as parameters
    double temp_perlin = perlin.noise((length_step + x_offset)*horizontal_scale, (height_step + y_offset) * vertical_scale, (width_step + z_offset) * horizontal_scale);
    temp_perlin -= 0.05;

    //Create new block with perlin sample and texturize
    if(temp_perlin > -0.4) {
        Block *obj = new Solid(length_step + x_offset, height_step + y_offset, width_step + z_offset);

        //Rudimentary texture selector
        if(height_step > 1 && height_step < 28)
            obj->load_texture(the_pool("Grass"));
        else if(height_step >= 28)
            obj->load_texture(the_pool("Stone"));
        else
            obj->load_texture(the_pool("Sand"));

        //Attach block to chunk
        the_chunk[((obj->x-x_offset) * WIDTH * HEIGHT) + ((obj->y-y_offset) * WIDTH) + (obj->z-z_offset)] = obj;

        //CULLING
        Block *eval_block = access_block(obj->x-1, obj->y-1, obj->z);

        if(eval_block && obj->x < LENGTH+x_offset && obj->y < HEIGHT+y_offset && obj->z < WIDTH+z_offset)
        {
           if(obj->y > x_offset && obj->x > y_offset && obj->z > z_offset)
            {
                //Names are in relation to the block being evaluated
                Block *above = access_block(obj->x-1, obj->y, obj->z);
                Block *below = access_block(obj->x-1, obj->y-2, obj->z);
                Block *left = access_block(obj->x-2, obj->y-1, obj->z );
                Block *right = access_block(obj->x, obj->y-1, obj->z);
                Block *behind = access_block(obj->x-1, obj->y-1, obj->z-1);
                Block *front_ = access_block(obj->x-1, obj->y-1, obj->z+1);

                if(above && below && left && right && behind && front_)
                {
                    eval_block->visible = false;
                }
            }
        }
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
    return the_chunk[((x-x_offset) * WIDTH * HEIGHT) + ((y-y_offset) * WIDTH) + (z-z_offset)];
}

void Chunk::render()
{
    for(size_t i = num_objects - 1; i > 0; i--)
    {
        if(the_chunk[i])
            the_chunk[i]->render();

        else
            continue;
    }
}

void Chunk::print_chunk_info()
{
    std::cout << "Number of objects in chunk: " << num_objects << ", size: " << sizeof(**the_chunk) << "\n";

}


