#ifndef WATERSPUBLICKEY_H
#define WATERSPUBLICKEY_H
#include<string>
#include"publickey.h"

class waterspublic: publickey
{
    public:
            element_t ga;
            std::string *attrs;
            element_s *hi;
            waterspublic(element_t g,element_t omega,element_t ag,std::string *attr,element_s *h)
                        :publickey(g,omega)
            {
                element_init_same_as(ga,ag);
                element_set(ga,ag);
                attrs = attr;
                hi = h;
            }
            ~waterspublic()
            {
                element_clear(ga);
                delete[] attrs;
                delete[] hi;
            }
};

#endif