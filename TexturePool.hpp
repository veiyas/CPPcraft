#include <map>
#include <string>
#include <functional>

class TexturePool
{
    using Tex_pool = std::map<std::string, Texture>;

public:

    TexturePool()
    {
    pool.insert(std::make_pair("Grass", Texture("textures/grass.tga")));
    pool.insert(std::make_pair("Stone", Texture("textures/stone.tga")));
    pool.insert(std::make_pair("Dirt", Texture("textures/dirt.tga")));
    pool.insert(std::make_pair("Sand", Texture("textures/sand.tga")));
    }

    Texture & operator()(std::string tex_name)
    {
        return pool[tex_name];
    }

    Tex_pool pool;
};
