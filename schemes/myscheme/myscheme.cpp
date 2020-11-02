#include"myscheme.h"


int main(int argc, char* argv[])
{
    pairingparam pairing(curve_param::a1_param.c_str());
    element_t rand;
    element_init_G1(rand,pairing.p);
    element_random(rand);
    myschememasterkey msm(rand);
    msm.printmasterkey();
    return 0;
}
