#ifndef WATERSUSERSECRETKEY_H
#define WATERSUSERSECRETKEY_H
#include"usersecretkey.h"
class watersusersecretkey:usersecretkey
{
    public:
            element_t K;
            element_t L;
            watersusersecretkey(element_t& k,element_t& l,atttokey& kx);
            ~watersusersecretkey();
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
#endif