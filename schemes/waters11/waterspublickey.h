#ifndef WATERSPUBLICKEY_H
#define WATERSPUBLICKEY_H
#include<string>
#include<map>
#include<iostream>
#include<vector>
#include"publickey.h"
#include"pairingparam.h"
#include"curve_param.h"

using attstoelement = std::map<std::string,element_t>;

class waterspublickey: publickey
{
    public:
            element_t ga;
            std::map<std::string,element_t> hi;
            waterspublickey(element_t& g,element_t& omega,element_t& ag,std::map<std::string,element_t>& h);
            ~waterspublickey();
            waterspublickey(waterspublickey& pk);
            void printpublickey();
};

waterspublickey::waterspublickey(element_t& g,element_t& omega,element_t& ag,std::map<std::string,element_t>& h)
                        :publickey(g,omega)
{
    element_init_same_as(ga,ag);
    element_set(ga,ag);
    for(std::map<std::string,element_t>::iterator ite = h.begin();ite != h.end();ite++)
    {
        element_init_same_as(hi[ite->first],ite->second);
        element_set(hi[ite->first],ite->second);
    }
}

waterspublickey::~waterspublickey()
{
    element_clear(ga);
    for(std::map<std::string,element_t>::iterator ite = hi.begin();ite != hi.end();ite++)
    {
        element_clear(ite->second);
    }
    //std::cout<<"waterspublickey deconstruction\n";
}

void waterspublickey::printpublickey()
{
    element_printf("generator = %B\n",generator);
    element_printf("omega = %B\n",omega);
    element_printf("g^a = %B\n",ga);
    for(std::map<std::string,element_t>::iterator ite = hi.begin();ite != hi.end();ite++)
    {
        std::cout<< ite->first<<": ";
        element_printf("%B\n",ite->second);
    }
}

waterspublickey::waterspublickey(waterspublickey& pk):publickey(pk.generator,pk.omega)
{  
    element_init_same_as(ga,pk.ga);
    element_set(ga,pk.ga);
    for(std::map<std::string,element_t>::iterator ite = pk.hi.begin();ite != pk.hi.end();ite++)
    {
        element_init_same_as(hi[ite->first],ite->second);
        element_set(hi[ite->first],ite->second);
    }
}
#endif