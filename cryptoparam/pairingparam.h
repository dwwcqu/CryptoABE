#ifndef PAIRINGPARAM_H
#define PAIRINGPARAM_H

#include"pbc/pbc.h"

class pairingparam
{
    public:
            pairing_t p;
            pairingparam(const char *str);
            ~pairingparam();
};

pairingparam::pairingparam(const char *str)
{
    pairing_init_set_str(p,str);
}

pairingparam::~pairingparam()
{
    pairing_clear(p);
}
#endif