#ifndef MASTERKEY_H
#define MASTERKEY_H
#include"pbc/pbc.h"
class masterkey
{     
    public:
            element_t alpha;
            masterkey(element_t a)
            {
                element_init_same_as(alpha,a);
                element_set(alpha,a);
            }
            ~masterkey()
            {
                element_clear(alpha);
            }
            masterkey(masterkey& mk)
            {
                element_init_same_as(alpha,mk.alpha);
                element_set(alpha,mk.alpha);
            }
};

#endif