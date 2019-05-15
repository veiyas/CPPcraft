#ifndef MESH_H
#define MESH_H

#include "Chunk.h"
#include "PerlinNoise.hpp"

#include <chrono>
#include <random>

#pragma once

const int air = -1000;
const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

//Block placeholder construct
struct Shell
{
    Shell()
        : x{0}, y{air}, z{0} {}
    Shell(int x_, int y_, int z_)
        : x{x_}, y{y_}, z{z_} {}

    std::string tex_name = "Dirt";

    int x;
    int y;
    int z;
};

class Mesh
{
    public:
    Mesh(const int pos_x, const int pos_y, const int pos_z, bool seeded)
        :X_OFFSET{pos_x*LENGTH}, Y_OFFSET{pos_y*HEIGHT}, Z_OFFSET{pos_z*WIDTH}
    {
        if(seeded)
            perlin = PerlinNoise(seed);
        else
            perlin = PerlinNoise();

        //Local perlin sample var
        double perlin_sample;

        //Create mesh of shells
        for(int j = 0; j < HEIGHT; j++)
        {
            for(size_t i = 0; i < LENGTH; i++)
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

        //Culling
        for(size_t j = 0; j < HEIGHT; j++)
        {
            for(size_t i = 0; i < LENGTH; i++)
            {
               for(size_t k = 0; k < WIDTH; k++)
                {
                    ytemp = temp_array[i][j][k].y;

                    if(ytemp == air) continue;

                    if(j == HEIGHT-1)
                    {
                        xtemp = temp_array[i][j][k].x;
                        ztemp = temp_array[i][j][k].z;
                        Shell temp_shell = Shell(xtemp,ytemp,ztemp);
                        temp_shell.tex_name = "Grass";
                        mesh.push_back(temp_shell);
                    }
                    else if (j == 0 || i == 0 || i == LENGTH-1 || k == 0 || k == WIDTH-1)
                    {
                        const int above = temp_array[i][j+1][k].y;
                        const int below = temp_array[i][j-1][k].y;

                        if(above == air)
                        {
                            xtemp = temp_array[i][j][k].x;
                            ztemp = temp_array[i][j][k].z;
                            Shell temp_shell = Shell(xtemp,ytemp,ztemp);
                            temp_shell.tex_name = "Grass";
                            mesh.push_back(temp_shell);
                        }
                        else
                        {
                            xtemp = temp_array[i][j][k].x;
                            ztemp = temp_array[i][j][k].z;
                            mesh.push_back(Shell(xtemp,ytemp,ztemp));
                        }
                    }
                    else
                    {
                        const int above = temp_array[i][j+1][k].y;

                        if(above == air)
                        {
                            xtemp = temp_array[i][j][k].x;
                            ztemp = temp_array[i][j][k].z;
                            Shell temp_shell = Shell(xtemp,ytemp,ztemp);
                            temp_shell.tex_name = "Grass";
                            mesh.push_back(temp_shell);
                        }

                        const int below = temp_array[i][j-1][k].y;
                        const int front_ = temp_array[i][j][k+1].y;
                        const int back_ = temp_array[i][j][k-1].y;
                        const int left = temp_array[i-1][j][k].y;
                        const int right = temp_array[i+1][j][k].y;

                        if(below == air || front_ == air || back_ == air || left == air || right == air)
                        {
                            xtemp = temp_array[i][j][k].x;
                            ztemp = temp_array[i][j][k].z;
                            mesh.push_back(Shell(xtemp,ytemp,ztemp));
                        }
                    }
                }
            }
        }
        //Save memory
        mesh.shrink_to_fit();
    }
    ~Mesh() = default;

    void sanity_check_vector() const;
    void sanity_check_array() const;

    Shell & operator()(const unsigned int k){return mesh[k];}

    int get_size(){return mesh.size();}

    //Members
    const static int LENGTH = 10;
    const static int WIDTH =  10;
    const static int HEIGHT = 20;

private:
    //Noise generator
    PerlinNoise perlin;

    //Members
    std::vector<Shell> mesh;
    Shell temp_array[LENGTH][HEIGHT][WIDTH];

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
