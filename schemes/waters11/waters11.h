#ifndef WATERS11_H
#define WATERS11_H

#include"elementmatrix.h"
#include"../basescheme.h"
#include"waterskeypair.h"
#include"waterscipher.h"
#include"watersusersecretkey.h"
#include"abeparam.h"
#include"curve_param.h"


class waters11:basescheme
{
    public:
        static waterskeypair setup(const std::vector<std::string>& atts,pairingparam& pairing);
        static waterscipher enc(waterspublickey& pk,ElementMatrix& M,element_t m);
};

#endif