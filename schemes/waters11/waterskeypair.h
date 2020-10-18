#ifndef WATERSKEYPAIR_H
#define WATERSKEYPAIR_H
#include"waterspublickey.h"
#include"masterkey.h"
class waterskeypair
{
    public:
            waterspublickey pk;
            masterkey msk;
            waterskeypair(waterspublickey& p,masterkey& m);
            waterskeypair(waterskeypair& kp);
            void printkeypair();
};

waterskeypair::waterskeypair(waterspublickey& p,masterkey& m):pk(p),msk(m)
{
}

waterskeypair::waterskeypair(waterskeypair& kp):pk(kp.pk),msk(kp.msk)
{

}

void
waterskeypair::printkeypair()
{
    pk.printpublickey();
    msk.printmasterkey();
}
#endif