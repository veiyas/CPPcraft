#pragma once

#include <vector>

class PerlinNoise
{
public:
    PerlinNoise();

    double noise(double x, double y, double z);

private:
//Data members
    std::vector<int> permutation;

//Methods
    double fade(double t);
    double lerp(double w, double a, double b);
    double grad(int hs, double x, double y, double z);
};
