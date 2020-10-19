#ifndef WATERSUSERSECRETKEY_H
#define WATERSUSERSECRETKEY_H
#include"usersecretkey.h"
class watersusersecretkey:public usersecretkey
{
    public:
            element_t K;
            element_t L;
            watersusersecretkey(element_t& k,element_t& l,atttokey& kx);
            ~watersusersecretkey();
            watersusersecretkey(watersusersecretkey& usk);
            void printSecretKey();
};

watersusersecretkey::watersusersecretkey(element_t& k,element_t& l,atttokey& kx):usersecretkey(kx)
{
    element_init_same_as(K,k);
    element_init_same_as(L,l);
    element_set(K,k);
    element_set(L,l);
}
watersusersecretkey::~watersusersecretkey()
{
    element_clear(K);
    element_clear(L);
}
watersusersecretkey::watersusersecretkey(watersusersecretkey& usk):usersecretkey(usk.Kx)
{
    element_init_same_as(K,usk.K);
    element_init_same_as(L,usk.L);
    element_set(K,usk.K);
    element_set(L,usk.L);
}
void
watersusersecretkey::printSecretKey()
{
    std::cout<<"\t\tUser Secret Key\n";
    element_printf("K = %B\n",K);
    element_printf("L = %B\n",L);
    for(attstoelement::iterator ite = Kx.begin(); ite != Kx.end(); ++ite)
    {
        std::cout<<"K["<<ite->first<<"] = ";
        element_printf("%B\n",ite->second);
    }
    std::cout<<"\n" ;
}
#endif