#ifndef WATERSCIPHER_H
#define WATERSCIPHER_H
#include<map>
#include"cipher.h"
using attstocipher = std::map<std::string,element_t>;
class waterscipher:public cipher
{
    public:
            attstocipher Ci;
            attstocipher Di;
            ElementMatrix M;
            waterscipher(element_t& c,element_t& c_,attstocipher& ci,attstocipher& di,ElementMatrix& m);
            ~waterscipher();
            waterscipher(waterscipher& wc);
            bool attIsInCipher(std::string& att);
            void printcipher();
};

waterscipher::waterscipher(element_t& c,element_t& c_,attstocipher& ci,attstocipher& di,ElementMatrix& m):cipher(c,c_),M(m)
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
    //M.~ElementMatrix();
}

waterscipher::waterscipher(waterscipher& wc):cipher(wc.C,wc.C_),M(wc.M)
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
void 
waterscipher::printcipher()
{
    std::cout<<"\t\tCiphertext\n";
    element_printf("C = %B\n",C);
    element_printf("C' = %B\n",C_);
    for(attstoelement::iterator ite = Ci.begin(); ite != Ci.end(); ++ite)
    {
        std::cout<<"C["<<ite->first<<"] = ";
        element_printf("%B\n",ite->second);
    }
    for(attstoelement::iterator ite = Di.begin(); ite != Di.end(); ++ite)
    {
        std::cout<<"D["<<ite->first<<"] = ";
        element_printf("%B\n",ite->second);
    }
    M.printElementMatrix();
    std::cout<<"\n";
}

bool
waterscipher::attIsInCipher(std::string& att)
{
    for(attstoelement::iterator ite = Ci.begin(); ite != Ci.end(); ++ite)
        if(ite->first == att)
            return true;
    return false;
}
#endif