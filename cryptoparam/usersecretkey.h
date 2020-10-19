#ifndef USERSECRETKEY_H
#define USERSECRETKEY_H
#include<map>

#include"pbc/pbc.h"
using atttokey = std::map<std::string,element_t>;

class usersecretkey
{
    public:
            atttokey Kx;
            usersecretkey(atttokey& kx);
            ~usersecretkey();
            usersecretkey(usersecretkey& usk);
};
usersecretkey::usersecretkey(atttokey& kx)
{
    for(atttokey::iterator ite = kx.begin(); ite != kx.end(); ++ite)
    {
        element_init_same_as(Kx[ite->first],ite->second);
        element_set(Kx[ite->first],ite->second);
    }
}
usersecretkey::~usersecretkey()
{
    for(atttokey::iterator ite = Kx.begin(); ite != Kx.end(); ++ite)
    {
        element_clear(ite->second);
    }
}
usersecretkey::usersecretkey(usersecretkey& usk)
{
    for(atttokey::iterator ite = usk.Kx.begin(); ite != usk.Kx.end(); ++ite)
    {
        element_init_same_as(Kx[ite->first],ite->second);
        element_set(Kx[ite->first],ite->second);
    }
}
#endif