#ifndef CHUNK_H
#define CHUNK_H
#include "Block.h"

class Chunk
{
public:
    Chunk() {the_chunk = new Block*[LENGTH*WIDTH*HEIGHT];}
    ~Chunk() = default;

    //Check if the block is surrounded by other block
    bool is_visible();
    //Attach an object to this chunk
    void add_object(Block *obj);
    //Render objects in chunk
    void render();
    //Create test chunk
    void create_dummy_chunk();
    //Print info about chunk
    void print_chunk_info();

    //The blocks, uses dynamic binding, declared as a 1-dim array and modified to 3D space with basic math
    Block **the_chunk;

private:
    //Constants
    const static int LENGTH = 4;
    const static int WIDTH = 8;
    const static int HEIGHT = 4;

    //Counters used for constructing 3D representation of chunk
    int num_objects = 0;
    int length_step = 0;
    int width_step = 0;
    int height_step = 0;
};

#endif
