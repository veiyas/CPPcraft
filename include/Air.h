#ifndef AIR_H
#define AIR_H

#include <Block.h>

//A block that obviously does not get rendered
//Current idea is that while we iterate over an array of blocks
//The air blocks methods get called but simply don't do anything
//Inefficient but good enough for now
class Air : public Block
{
public:
    Air() = default;
     ~Air() = default;

    void render() override {return;};
    void print_info() const override {return;};
    void translate_block(const float &x, const float &y, const float &z) {return;};
    void prep_block() {return;};
};

#endif // AIR_H
