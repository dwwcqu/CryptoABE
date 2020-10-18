#ifndef CIPHER_H
#define CIPHER_H
#include"pbc/pbc.h"
class cipher
{
    public:
            element_t C;
            element_t C_;
            cipher(element_t& c,element_t c_);
            cipher(cipher& cp);
            ~cipher();
};

cipher::cipher(element_t& c,element_t c_)
{
    element_init_same_as(C,c);
    element_init_same_as(C_,c_);
    element_set(C,c);
    element_set(C_,c_);
}

cipher::~cipher()
{
    element_clear(C);
    element_clear(C_);
}

cipher::cipher(cipher& cp)
{
    element_init_same_as(C,cp.C);
    element_set(C,cp.C);
    element_init_same_as(C_,cp.C_);
    element_set(C_,cp.C_);
}
#endif