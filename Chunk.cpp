#include "Chunk.h"
#include "Block.h"
#include "Solid.h"
#include "Air.h"
#include <iostream>

Chunk::Chunk()
{
    //Initialize with nullptrs
    the_chunk = new Block*[LENGTH*WIDTH*HEIGHT]{nullptr};
    tex = Texture("textures/grass.tga");
    the_pool = TexturePool();
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

    double temp_perlin = perlin.noise(length_step*horizontal_scale, width_step*horizontal_scale, height_step*vertical_scale);
    temp_perlin -= 0.05;

    if(temp_perlin > 0) {
        Block *obj = new Solid(length_step, height_step, width_step);
        obj->load_texture(the_pool("Grass"));
        the_chunk[(obj->x * WIDTH * HEIGHT) + (obj->y * WIDTH) + obj->z] = obj;

        if(obj->x > 2 && obj->y > 2 && obj->z > 2 && obj->x < 11 && obj->y < 11 && obj->z < 11)
        {
            Block *block_eval = access_block(obj->x, obj->y-1, obj->z-1);
            Block *block_below = access_block(obj->x, obj->y-1, obj->z);
            Block *block_back = access_block(obj->x-1, obj->y , obj->z);
            Block *block_back_left = access_block(obj->x-1, obj->y-1 , obj->z-1);

            if(block_below && block_back && block_back_left && block_eval)
                block_eval->visible = false;

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
    return the_chunk[(x * WIDTH * HEIGHT) + (y * WIDTH) + z];
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

    //        if(the_chunk[i] != nullptr)
//            {
//                if(the_chunk[i]->x > 0 && the_chunk[i]->y > 0 && the_chunk[i]->z > 0 && the_chunk[i]->x < length_step && the_chunk[i]->y < height_step && the_chunk[i]->z < width_step)
//                {
//                    if(access_block(the_chunk[i]->x+1, the_chunk[i]->y, the_chunk[i]->z) &&
//                       access_block(the_chunk[i]->x, the_chunk[i]->y+1, the_chunk[i]->z) &&
//                       access_block(the_chunk[i]->x, the_chunk[i]->y, the_chunk[i]->z+1))
//                    {
//                        the_chunk[i]->visible = false;
//                        continue;
//                    }
//                }
//                else
//                {
//                    the_chunk[i]->render();
//                }
//
//            }
}

void Chunk::print_chunk_info()
{
    std::cout << "Number of objects in chunk: " << num_objects << ", size: " << sizeof(**the_chunk) << "\n";

}


