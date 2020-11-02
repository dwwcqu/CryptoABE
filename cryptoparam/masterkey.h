#ifndef MASTERKEY_H
#define MASTERKEY_H
#include<iostream>
#include"pbc/pbc.h"
class masterkey
{     
    public:
            element_t alpha;
            masterkey() = default;
            masterkey(element_t& a);
            ~masterkey();
            masterkey(masterkey& mk);
            masterkey& operator= (masterkey& msk);
            void printmasterkey();
};

masterkey::masterkey(element_t& a)
{
    element_init_same_as(alpha,a);
    element_set(alpha,a);
}

masterkey::~masterkey()
{
    element_clear(alpha);
}

masterkey::masterkey(masterkey& mk)
{
    element_init_same_as(alpha,mk.alpha);
    element_set(alpha,mk.alpha);
}
masterkey&
masterkey::operator=(masterkey& msk)
{
    if(this == &msk)
        return *this;
    element_init_same_as(alpha,msk.alpha);
    element_set(alpha,msk.alpha);
    return *this;
}

void
masterkey::printmasterkey()
{
    std::cout<<"\t\tMaster Key\n";
    element_printf("alpha = %B\n",alpha);
}
#endif