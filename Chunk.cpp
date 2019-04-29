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

void Chunk::set_visibility(Block *obj)
{
    //CULLING
    Block *eval_block = access_block((obj->x-1) - x_offset, (obj->y-1) - y_offset, obj->z - z_offset);

    if(eval_block && obj->x - x_offset < LENGTH && obj->y - y_offset < HEIGHT && obj->z - z_offset < WIDTH)
    {
       if(obj->x > x_offset && obj->y > y_offset && obj->z > z_offset)
        {
            //Names are in relation to the block being evaluated
            Block *above = access_block( (obj->x-1) - x_offset, (obj->y)   - y_offset, (obj->z)   - z_offset);
            Block *below = access_block( (obj->x-1) - x_offset, (obj->y-2) - y_offset, (obj->z)   - z_offset);
            Block *left = access_block(  (obj->x-2) - x_offset, (obj->y-1) - y_offset, (obj->z)   - z_offset);
            Block *right = access_block( (obj->x)   - x_offset, (obj->y-1) - y_offset, (obj->z)   - z_offset);
            Block *behind = access_block((obj->x-1) - x_offset, (obj->y-1) - y_offset, (obj->z-1) - z_offset);
            Block *front_ = access_block((obj->x-1) - x_offset, (obj->y-1) - y_offset, (obj->z+1) - z_offset);

            if(above && below && left && right && behind && front_)
            {
                eval_block->visible = false;
            }
        }
    }
}

void Chunk::add_object()
{
    //Size check
    if(num_objects == LENGTH*WIDTH*HEIGHT){return;}

    //Create perlin sample with block position as parameters
    double temp_perlin = perlin.noise((length_step + x_offset)*horizontal_scale, (height_step + y_offset) * vertical_scale, (width_step + z_offset) * horizontal_scale);
    temp_perlin -= 0.05;

    //Create new block with perlin sample and texturize
    if(temp_perlin > PERLIN_LIMIT) {
        Block *obj = new Solid(length_step + x_offset, height_step + y_offset, width_step + z_offset);

        //Rudimentary texture selector
        if(height_step > 1 && height_step < 28)
            obj->load_texture(the_pool("Grass"));
        else if(height_step >= 28)
            obj->load_texture(the_pool("Grass"));
        else
            obj->load_texture(the_pool("Grass"));

        //Attach block to chunk
        the_chunk[((obj->x-x_offset) * WIDTH * HEIGHT) + ((obj->z-z_offset) * HEIGHT) + (obj->y-y_offset)] = obj;

        //Cull/prune unseen blocks
        set_visibility(obj);

        Block *dirt_check = access_block(obj->x - x_offset, (obj->y-1) - y_offset, obj->z - z_offset);

        if(dirt_check)
        {
//            dirt_check->load_texture(the_pool("Grass"));
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
    return the_chunk[(x * WIDTH * HEIGHT) + (z * HEIGHT) + y];
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


