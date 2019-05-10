#ifndef CHUNK_H
#define CHUNK_H
#include "Block.h"
#include "Mesh.h"
#include "TexturePool.hpp"
#include "PerlinNoise.hpp"

class Chunk
{
public:
    Chunk(const int & x, const int & y, const int & z);
    ~Chunk() = default;

    //Attach an object to this chunk
    void add_object();
    //Render objects in chunk
    void render();
    //Print info about chunk
    void print_chunk_info();

private:
    //The blocks, uses dynamic binding
    std::vector<Block*> chunk;
    Mesh mesh;

    //Check if the block is surrounded by other block
    void set_visibility(Block *obj);

    //Access block at a certain position
    Block * access_block(const int &x, const int &y, const int &z);

    //Constants
    const int MAX_SIZE;

    //Counters used for constructing chunk
    int num_objects = 0;

    //Texture
    TexturePool the_pool;
};

//class Chunk
//{
//public:
//    Chunk(const int pos_x, const int pos_y, const int pos_z);
//    ~Chunk() {delete [] the_chunk;}
//
//    //Attach an object to this chunk
//    void add_object();
//    //Render objects in chunk
//    void render();
//    //Print info about chunk
//    void print_chunk_info();
//
//private:
//    //The blocks, uses dynamic binding, declared as a 1-dim array and modified to 3D space with clever indexing
//    Block **the_chunk;
//
//    //Check if the block is surrounded by other block
//    void set_visibility(Block *obj);
//
//    //Access block at a certain position
//    Block * access_block(const int &x, const int &y, const int &z);
//
//    //Constants
//    const float PERLIN_LIMIT = 0.4;
//    const float horizontal_scale = 0.075;
//    const float vertical_scale = 0.1;
//    const static int LENGTH = 16;
//    const static int WIDTH =  16;
//    const static int HEIGHT = 16;
//
//    //Counters used for constructing chunk
//    int num_objects = 0;
//    int length_step = 0;
//    int width_step = 0;
//    int height_step = 0;
//
//    //World placement coordinates
//    const int x_offset;
//    const int y_offset;
//    const int z_offset;
//
//    //Texture
//    TexturePool the_pool;
//
//    //Perlin noise things
//
//    PerlinNoise perlin = PerlinNoise{};
//
//};

#endif
