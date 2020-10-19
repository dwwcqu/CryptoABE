#ifndef WATERS11_H
#define WATERS11_H

#include"elementmatrix.h"
#include"../basescheme.h"
#include"waterskeypair.h"
#include"waterscipher.h"
#include"watersusersecretkey.h"
#include"abeparam.h"
#include"curve_param.h"
#include"attsmaptoelement.h"
#include"message.h"
class waters11:basescheme
{
    public:
        static waterskeypair& setup(const std::vector<std::string>& atts,pairingparam& pairing);
        static waterscipher& enc(waterspublickey& pk,accesspolicy& policy,message& m,pairingparam& pairing);
        static watersusersecretkey& keygen(const std::vector<std::string>& useratts,waterspublickey& pk,masterkey& msk,pairingparam& pairing);
        static message& dec(waterscipher& c,watersusersecretkey& userkey,pairingparam& pairing);
};

#endif