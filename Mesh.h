#include "Block.h"
#include "Chunk.h"
#include "Solid.h"
#include "Air.h"
#include "PerlinNoise.hpp"

#pragma once

class Mesh
{
    //Block placeholder construct, defaults to private if not specified otherwise
    struct Shell
    {
        Shell(int x_, int y_, int z_)
            : x{x_}, y{y_}, z{z_} {}

        const int x;
        const int y;
        const int z;
    };

public:
    Mesh(const int pos_x, const int pos_y, const int pos_z)
        :X_OFFSET{pos_x*LENGTH}, Y_OFFSET{pos_y*HEIGHT}, Z_OFFSET{pos_z*WIDTH}
    {
        //Initialize local perlin sample var
        double perlin_sample;

        for(size_t i = 0; i < LENGTH; i++)
        {
            for(size_t j = 0; j < HEIGHT; j++)
            {
               for(size_t k = 0; k < WIDTH; k++)
                {
                    //Generate noise and see if a block should be placed
                    perlin_sample = perlin.noise(i*H_SCALE,j*V_SCALE,k*H_SCALE);

                    if(perlin_sample > PERLIN_LIMIT)
                        mesh.push_back(Shell(i+X_OFFSET, j+Y_OFFSET, k+Z_OFFSET));
                }
            }
        }
        //Save memory
        mesh.shrink_to_fit();
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

private:
    //Noise generator
    PerlinNoise perlin{};

    //Members
    std::vector<Shell> mesh;

    //Constants
    const float PERLIN_LIMIT = 0.4;
    const float H_SCALE = 0.075;
    const float V_SCALE = 0.1;
    const static int LENGTH = 16;
    const static int WIDTH =  16;
    const static int HEIGHT = 16;

    //World placement coordinates
    const int X_OFFSET;
    const int Y_OFFSET;
    const int Z_OFFSET;

};
