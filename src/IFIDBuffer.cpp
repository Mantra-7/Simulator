#include "../include/Structures.hpp"

int16 IFIDBuffer::getInstruction()
{
    return instruction;
}

void IFIDBuffer::set(int16 instruction)
{
    this->instruction = instruction;
    valid = true;
}

void IFIDBuffer::invalidate()
{
    valid = false;
}