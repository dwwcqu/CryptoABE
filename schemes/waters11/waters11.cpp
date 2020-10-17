#include"waterspublickey.h"



int main(int argc,char* argv[])
{
    pairing_t pairing;
    pairing_init_set_str(pairing,curve_param::a_param.c_str());

    element_t generator;
    element_init_G1(generator,pairing);
    element_random(generator);
    element_t omega;
    element_init_GT(omega,pairing);
    element_random(omega);
    element_t ga;
    element_init_G1(ga,pairing);
    element_random(ga);
    std::vector<std::string> atts{"A","B","C","D"};
    std::map<std::string,element_t> h;
    for(auto att: atts)
    {
        element_init_G1(h[att],pairing);
        element_random(h[att]);
    }
    element_printf("first generator = %B\n\n",generator);
    element_printf("first omega = %B\n\n",omega);
    for(std::map<std::string,element_t>::iterator ite = h.begin(); ite != h.end(); ++ite)
    {
        std::cout<<ite->first<<": ";
        element_printf("%B\n\n",ite->second);
    }

    publickey wpk(generator,omega);
    wpk.printpublickey();

    element_set1(generator);
    element_printf("second generator = %B\n\n",generator);

    wpk.printpublickey();

    for(auto att: atts)
    {
        std::cout<<"clear "<<att<<"\n";
        element_clear(h[att]);
    }
    element_clear(generator);
    element_clear(omega);
    element_clear(ga);
    pairing_clear(pairing);
    return 0;
}