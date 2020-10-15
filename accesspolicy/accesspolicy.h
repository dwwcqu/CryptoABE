#ifndef ACCESSPOLICY_H
#define ACCESSPOLICY_H
#include<string>
#include<stack>
#include<cassert>

//保存以字符串形式输入访问策略，并
class accesspolicy
{
    private:
            std::string policy;//使用字符串的形式代表一个访问策略；例如：((A,B,C,2),(D,E,F,2),1)的形式
    public:
            accesspolicy();
            accesspolicy(std::string policy);
            bool isvalidpolicy();//判断当前字符串形式的策略是否准确
            std::string getpolicy() const;
};

int getfirstcommaindex(std::string& policy);
#endif