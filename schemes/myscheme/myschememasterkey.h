#ifndef MYSCHEMEMASTER_H_INCLUDE
#define MYSCHEMEMASTER_H_INCLUDE
#include"masterkey.h"
class myschememasterkey:public masterkey
{
    public:
            myschememasterkey(element_t& a);
            myschememasterkey(myschememasterkey& msm);
            myschememasterkey& operator=(myschememasterkey& msm);
};

myschememasterkey::myschememasterkey(element_t& a):masterkey(a)
{

}
myschememasterkey::myschememasterkey(myschememasterkey& msm):masterkey(msm.alpha)
{

}
myschememasterkey&
myschememasterkey::operator=(myschememasterkey& msm)
{
    if(this == &msm)
        return *this;
    element_init_same_as(alpha,msm.alpha);
    element_set(alpha,msm.alpha);
    return *this;
}
#endif