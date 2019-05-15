#pragma once

#include <vector>

class PerlinNoise
{
public:
    PerlinNoise();
    PerlinNoise(unsigned seed);


    double noise(double x, double y, double z);

private:
//Data members
    std::vector<int> permutation;

//Seed choice
    bool seeded;

//Methods
    double fade(double t);
    double lerp(double w, double a, double b);
    double grad(int hs, double x, double y, double z);
};
