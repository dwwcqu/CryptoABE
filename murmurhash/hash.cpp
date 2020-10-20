#include"hash.h"

std::vector<std::string> SEED::seeds = {
                                "A","B","C","D","F","G","H","I","J","K","L","I","M","N","O","P","U","R","S","X","Y","Z"
                                };

int hasher::id = 0;

hasher::hasher(int s):space(s)
{
    assert(id < SEED::seeds.size() && "hasher::hasher():no hash to construct");
    MurmurHash3_x86_32(&SEED::seeds[id],SEED::seeds[id].length(),OSEED,&SEED);
    ++id;
}

hasher::~hasher()
{
    --id;
}

int
hasher::hash(const std::string& message)
{
    uint32_t h;
    MurmurHash3_x86_32(&message,message.length(),SEED,&h);
    return h % space;
}


int main()
{
    std::string mess{"dengweiwei"};
    return 0;
}