#ifndef CHUNK_H
#define CHUNK_H
#include "Block.h"

class Chunk
{
public:
    Chunk() {the_chunk = new Block*[LENGTH*WIDTH*HEIGHT];
    for(size_t i = 0; i < LENGTH*WIDTH*HEIGHT; i++)
        the_chunk[i] = nullptr;
    tex = Texture("textures/grass.tga");
    }
    ~Chunk() {delete [] the_chunk;}

    //Check if the block is surrounded by other block
    void set_visibility();
    //Attach an object to this chunk
    void add_object();
    //Render objects in chunk
    void render();
    //Create test chunk
    void create_dummy_chunk(Texture &tex);
    //Print info about chunk
    void print_chunk_info();
    //Access block at a certain position
    Block * access_block(const int &x, const int &y, const int &z);
    //The blocks, uses dynamic binding, declared as a 1-dim array and modified to 3D space with basic math
    Block **the_chunk;

private:
    //Constants
    const static int LENGTH = 16;
    const static int WIDTH =  16;
    const static int HEIGHT = 16;

    //Counters used for constructing 3D representation of chunk
    int num_objects = 0;
    int length_step = 0;
    int width_step = 0;
    int height_step = 0;

    //Temp texture
    Texture tex;
};

#endif
