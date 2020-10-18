#ifndef WATERSCIPHER_H
#define WATERSCIPHER_H
#include<map>
#include"cipher.h"
using attstocipher = std::map<std::string,element_t>;
class waterscipher:cipher
{
    public:
            attstocipher Ci;
            attstocipher Di;
            waterscipher(element_t& c,element_t& c_,attstocipher& ci,attstocipher& di);
            ~waterscipher();
            waterscipher(waterscipher& wc);
};

waterscipher::waterscipher(element_t& c,element_t& c_,attstocipher& ci,attstocipher& di):cipher(c,c_)
{
    for(attstocipher::iterator ite = ci.begin(); ite != ci.end(); ++ite)
    {
        element_init_same_as(Ci[ite->first],ite->second);
        element_set(Ci[ite->first],ite->second);
    }
    for(attstocipher::iterator ite = di.begin(); ite != di.end(); ++ite)
    {
        element_init_same_as(Di[ite->first],ite->second);
        element_set(Di[ite->first],ite->second);
    }
}
waterscipher::~waterscipher()
{
    for(attstocipher::iterator ite = Ci.begin(); ite != Ci.end(); ++ite)
    {
        element_clear(ite->second);
    }
    for(attstocipher::iterator ite = Di.begin(); ite != Di.end(); ++ite)
    {
        element_clear(ite->second);
    }
}

waterscipher::waterscipher(waterscipher& wc):cipher(wc.C,wc.C_)
{
    for(attstocipher::iterator ite = wc.Ci.begin(); ite != wc.Ci.end(); ++ite)
    {
        element_init_same_as(Ci[ite->first],ite->second);
        element_set(Ci[ite->first],ite->second);
    }
    for(attstocipher::iterator ite = wc.Di.begin(); ite != wc.Di.end(); ++ite)
    {
        element_init_same_as(Di[ite->first],ite->second);
        element_set(Di[ite->first],ite->second);
    }
}
#endif