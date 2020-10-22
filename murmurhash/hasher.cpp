#include"hasher.h"


hasher::hasher()
{
    SEED = rand()%(MAX_SEED - MIN_SEED) + MIN_SEED;
}

hasher::hasher(int s):space(s)
{
    srandom((int)time(0));
    SEED = rand()%(MAX_SEED - MIN_SEED) + MIN_SEED;
}


hasher::hasher(const hasher& h)
{
    space = h.space;
    SEED = h.SEED;
}

int
hasher::hash(const std::string& message)
{
    uint32_t h;
    MurmurHash3_x86_32(&message,message.length(),SEED,&h);
    return h % space;
}

void
hasher::setSpace(int s)
{
    space = s;
}

void 
hasher::printSEED()
{
    std::cout<<std::hex<<SEED<<'\n';
}
