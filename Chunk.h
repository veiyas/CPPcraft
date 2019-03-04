#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"


class Chunk
{
public:
    Chunk();
    ~Chunk() = default;

    //Check if the block is surrounded by other block
    bool is_visible();

    void render();

protected:

private:
    const static int length = 4;
    const static int width = 4;
    const static int height = 4;
    Block the_chunk[length][width][height];
};

#endif // CHUNK_H
