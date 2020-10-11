#ifndef ACCESSPOLICY_H
#define ACCESSPOLICY_H
#include<string>
#include<stack>
#include<cassert>
class accesspolicy
{
    private:
            std::string policy;
            bool isvalidpolicy();
    public:
            accesspolicy();
            accesspolicy(std::string policy);
            std::string getpolicy() const;
};

int getfirstcommaindex(std::string& policy);
#endif