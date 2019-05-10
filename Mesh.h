#include "Block.h"
#include "Chunk.h"
#include "Solid.h"
#include "Air.h"
#include "PerlinNoise.hpp"

#pragma once

//Block placeholder construct
struct Shell
{
    Shell(int x_, int y_, int z_)
        : x{x_}, y{y_}, z{z_} {}

    const int x;
    const int y;
    const int z;
};

class Mesh
{
    public:
    Mesh(const int pos_x, const int pos_y, const int pos_z)
        :X_OFFSET{pos_x*LENGTH}, Y_OFFSET{pos_y*HEIGHT}, Z_OFFSET{pos_z*WIDTH}
    {
        //Initialize local perlin sample var
        double perlin_sample;

        for(size_t i = LENGTH; i > 0; i--)
        {
            for(size_t j = HEIGHT; j > 0; j--)
            {
               for(size_t k = WIDTH; k > 0; k--)
                {
                    //Generate noise and see if a block should be placed
                    perlin_sample = perlin.noise((i+X_OFFSET)*H_SCALE, (j+Y_OFFSET)*V_SCALE, (k+Z_OFFSET)*H_SCALE);

                    if(perlin_sample > PERLIN_LIMIT)
                        mesh.push_back(Shell(i+X_OFFSET, j+Y_OFFSET, k+Z_OFFSET));
                }
            }
        }
        //Save memory
        mesh.shrink_to_fit();

        //Culling
    }

    ~Mesh() = default;

    void sanity_check() const
    {
        for(auto ptr = std::begin(mesh); ptr != std::end(mesh); ++ptr)
            std::cout << "(" << ptr->x << ", " << ptr->y << ", " << ptr->z << ")\n";
    }

    Shell & operator()(const unsigned int k)
    {
        return mesh[k];
    }

    int get_size()
    {
        return mesh.size();
    }

    //Members
    const int LENGTH = 16;
    const int WIDTH =  16;
    const int HEIGHT = 16;

private:
    //Noise generator
    PerlinNoise perlin{};

    //Members
    std::vector<Shell> mesh;

    //Constants
    const float PERLIN_LIMIT = 0.4;
    const float H_SCALE = 0.075;
    const float V_SCALE = 0.1;

    //World placement coordinates
    const int X_OFFSET;
    const int Y_OFFSET;
    const int Z_OFFSET;

};
