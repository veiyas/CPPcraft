#ifndef CHUNK_H
#define CHUNK_H
#include "Block.h"
#include "TexturePool.hpp"
#include "PerlinNoise.hpp"

class Chunk
{
public:
    Chunk(const int pos_x, const int pos_y, const int pos_z);
    ~Chunk() {delete [] the_chunk;}

    //Check if the block is surrounded by other block
    void set_visibility();
    //Attach an object to this chunk
    void add_object();
    //Render objects in chunk
    void render();
    //Print info about chunk
    void print_chunk_info();
    //Access block at a certain position
    Block * access_block(const int &x, const int &y, const int &z);
    //The blocks, uses dynamic binding, declared as a 1-dim array and modified to 3D space with clever indexing
    Block **the_chunk;

private:
    //Constants
    const static int LENGTH = 16;
    const static int WIDTH =  16;
    const static int HEIGHT = 16;

    //Counters used for constructing chunk
    int num_objects = 0;
    int length_step = 0;
    int width_step = 0;
    int height_step = 0;

    //World placement coordinates
    const int x_offset;
    const int y_offset;
    const int z_offset;

    //Texture
    TexturePool the_pool;

    //Perlin noise things
    const double horizontal_scale = 0.10;
    const double vertical_scale = 0.10;
    PerlinNoise perlin = PerlinNoise{};

};

#endif
