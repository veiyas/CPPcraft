#ifndef CHUNK_H
#define CHUNK_H
#include "Block.h"
#include "Mesh.h"
#include "TexturePool.hpp"
#include "PerlinNoise.hpp"

class Chunk
{
public:
    Chunk(const int & x, const int & y, const int & z, bool seeded);
    ~Chunk() = default;

    //Attach an object to this chunk
    void add_object();
    //Render objects in chunk
    void render();
    //Print info about chunk
    void print_chunk_info();

    Mesh mesh;
private:
    //The blocks, uses dynamic binding
    std::vector<Block*> chunk;

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
#endif
