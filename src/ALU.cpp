#include "../include/Structures.hpp"

int8 ALU::adder(int8 X , int8 Y, flag as)
{
    if(!as)
    {
        return X + Y;
    }
    else
    {
        return X - Y;
    }
}

int8 ALU::MUL(int8 X, int8 Y)
{
    return X * Y;
}

int8 ALU::OR(int8 X, int8 Y)
{
    return X | Y;
}

int8 ALU::AND(int8 X, int8 Y)
{
    return X & Y;
}

int8 ALU::NOT(int8 X)
{
    return ~X;
}

int8 ALU::XOR(int8 X, int8 Y)
{
    return X ^ Y;
}

flag ALU::BEQZ(int X)
{
    return X == 0;
}