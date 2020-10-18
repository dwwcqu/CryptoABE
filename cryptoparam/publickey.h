#ifndef PUBLICKEY_H
#define PUBLICKEY_H

#include"pbc/pbc.h"

class publickey
{
    public:
            element_t generator;
            element_t omega;
            publickey(element_t& g,element_t& o);
            publickey(publickey& pk);
            ~publickey();
            void printpublickey() const;
};

publickey::publickey(element_t& g,element_t& o)
{
    element_init_same_as(generator,g);
    element_init_same_as(omega,o);
    element_set(generator,g);
    element_set(omega,o);      
}

publickey::publickey(publickey& pk)
{
    element_init_same_as(generator,pk.generator);
    element_set(generator,pk.generator);
    element_init_same_as(omega,pk.omega);
    element_set(omega,pk.omega);
}
publickey::~publickey()
{
    element_clear(generator);
    element_clear(omega);
    //std::cout<<"publickey deconstruction\n";
}
void publickey::printpublickey() const
{
    element_printf("generator = %B\n",generator);
    element_printf("omega = %B\n",omega);
}
#endif