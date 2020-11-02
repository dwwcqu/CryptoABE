#ifndef _BYTE_H
#define _BYTE_H
#include<iostream>
#include<math.h>
#include<stdint.h>
class byte
{
    public:
        uint8_t bits;
        byte(char c);
        byte(int b);
        friend std::ostream& operator<<(std::ostream& out,const byte& b);

};

byte::byte(char c):bits(c)
{
}
byte::byte(int b):bits(static_cast<uint8_t>(b))
{
}
std::ostream& operator<<(std::ostream& out,const byte& b)
{
    uint8_t hex1 = 0x80;
    for(int i = 7; i >= 0; --i)
    {
        uint8_t AND = hex1 & b.bits;
        if(pow(2,i) == static_cast<int>(AND))
        {
            out<<"1";
            hex1 = hex1 >> 1;
        }
        else
        {
            out<<"0";
            hex1 = hex1 >> 1;;
        }
    }
    return out;
}
#endif