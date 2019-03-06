#include "Chunk.h"
#include "Block.h"

Chunk::Chunk()
{
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < width; j++)
        {
            for(int k = 0; k < width; k++)
            {
                the_chunk[i][j][k].translate_block(i,j,k);
            }
        }
    }
}

bool Chunk::is_visible()
{

}

void Chunk::render()
{
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < width; j++)
        {
            for(int k = 0; k < height; k++)
            {
                the_chunk[i][j][k].render();
            }
        }
    }
}
