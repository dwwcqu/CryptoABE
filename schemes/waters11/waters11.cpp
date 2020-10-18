#include"waters11.h"


waterskeypair
waters11::setup(const std::vector<std::string>& atts,pairingparam& pairing)
{
    element_t generator;
    element_init_G1(generator,pairing.p);
    element_random(generator);

    element_t alpha;
    element_init_Zr(alpha,pairing.p);
    element_random(alpha);

    element_t a;
    element_init_Zr(a,pairing.p);
    element_random(a);

    element_t omega;
    element_init_GT(omega,pairing.p);
    pairing_apply(omega,generator,generator,pairing.p);
    element_pow_zn(omega,omega,alpha);

    element_t ga;
    element_init_G1(ga,pairing.p);
    element_pow_zn(ga,generator,a);

    element_t msk;
    element_init_G1(msk,pairing.p);
    element_pow_zn(msk,generator,alpha);

    std::map<std::string,element_t> h;
    for(auto att: atts)
    {
        element_init_G1(h[att],pairing.p);
        element_random(h[att]);
    }
    masterkey mk(msk);
    waterspublickey pk(generator,omega,ga,h);
    waterskeypair re(pk,mk);

    element_clear(generator);
    element_clear(omega);
    element_clear(msk);
    element_clear(ga);
    element_clear(alpha);
    element_clear(a);
    for(auto att: atts)
    {
        element_clear(h[att]);
    }
    return re;
}



int main(int argc,char* argv[])
{
    pairingparam pairing(curve_param::a_param.c_str());
    pairing_init_set_str(pairing.p,curve_param::a_param.c_str());
    
    return 0;
}