#ifndef MESH_H
#define MESH_H
#include "Chunk.h"
#include "PerlinNoise.hpp"
#pragma once

//Block placeholder construct
struct Shell
{
    Shell()
        : x{0}, y{0}, z{0} {}
    Shell(int x_, int y_, int z_)
        : x{x_}, y{y_}, z{z_} {}

    int x;
    int y;
    int z;
};

class Mesh
{
    public:
    Mesh(const int pos_x, const int pos_y, const int pos_z)
        :X_OFFSET{pos_x*LENGTH}, Y_OFFSET{pos_y*HEIGHT}, Z_OFFSET{pos_z*WIDTH}
    {
        //Initialize local perlin sample var
        double perlin_sample;

        //Create mesh of shells
        for(size_t i = 0; i < LENGTH; i++)
        {
            for(size_t j = 0; j < HEIGHT; j++)
            {
               for(size_t k = 0; k < WIDTH; k++)
                {
                    //Generate noise and see if a block should be placed
                    perlin_sample = perlin.noise((i+X_OFFSET)*H_SCALE, (j+Y_OFFSET)*V_SCALE, (k+Z_OFFSET)*H_SCALE);

                    if(perlin_sample > PERLIN_LIMIT)
                    {
                        temp_array[i][j][k].x = i + X_OFFSET;
                        temp_array[i][j][k].y = j + Y_OFFSET;
                        temp_array[i][j][k].z = k + Z_OFFSET;
                    }
                }
            }
        }
        double xtemp, ytemp, ztemp;

        //Culling TODO


        //Save memory
        mesh.shrink_to_fit();

    }
    ~Mesh() = default;

    void sanity_check_vector() const;
    void sanity_check_array() const;

    Shell & operator()(const unsigned int k){return mesh[k];}

    int get_size(){return mesh.size();}

    //Members
    const static int LENGTH = 16;
    const static int WIDTH =  16;
    const static int HEIGHT = 16;

private:
    //Noise generator
    PerlinNoise perlin{};

    //Members
    std::vector<Shell> mesh;
    Shell temp_array[LENGTH][WIDTH][HEIGHT];

    //Constants
    const float PERLIN_LIMIT = 0.4;
    const float H_SCALE = 0.075;
    const float V_SCALE = 0.1;

    //World placement coordinates
    const int X_OFFSET;
    const int Y_OFFSET;
    const int Z_OFFSET;

};

//Mesh::Mesh(const int pos_x, const int pos_y, const int pos_z)


//void Mesh::sanity_check_array() const
//{
//    for(size_t i = 0; i < LENGTH; i++)
//    {
//        for(size_t j = 0; j < HEIGHT; j++)
//        {
//           for(size_t k = 0; k < WIDTH; k++)
//            {
//                std::cout << "(" << temp_array[i][j][k].x << ", " << temp_array[i][j][k].y << ", " << temp_array[i][j][k].z << ")\n";
//            }
//        }
//    }
//}

//void Mesh::sanity_check_vector() const
//{
//    for(auto ptr = std::begin(mesh); ptr != std::end(mesh); ++ptr)
//        std::cout << "(" << ptr->x << ", " << ptr->y << ", " << ptr->z << ")\n";
//}

#endif // MESH_H
