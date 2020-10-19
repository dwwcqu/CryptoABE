#ifndef MESSAGE_H
#define MESSAGE_H
#include"pbc/pbc.h"
class message
{
    public:
            element_t ms;
            message(element_t& mes);
            message(message& mes);
            friend bool operator==(message& m1,message& m2);
            ~message();
            void printmessage();
};
message::message(element_t& mes)
{
    element_init_same_as(ms,mes);
    element_set(ms,mes);
}

message::~message()
{
    element_clear(ms);
}

bool operator==(message& m1,message& m2)
{
    if(!element_cmp(m1.ms,m2.ms))
        return true;
    return false;
}

message::message(message& mes)
{
    element_init_same_as(ms,mes.ms);
    element_set(ms,mes.ms);
}
void
message::printmessage()
{
    std::cout<<"\t\tMessage\n";
    element_printf("message = %B\n",ms);
}
#endif 