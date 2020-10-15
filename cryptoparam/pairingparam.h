#ifndef PAIRINGPARAM_H
#define PAIRINGPARAM_H

#include"pbc/pbc.h"

class pairingparam
{
    protected:
            pairing_s pairing;
    public:
            pairingparam(const char *str)
            {
                pairing_init_set_str(&pairing,str);
            }
            pairingparam(const pairingparam& pg)
            {

            }
            ~pairingparam()
            {
                pairing_clear(&pairing);
            }
};
#endif