#ifndef ABF_H
#define ABF_H
#define MAX_RAND 0xff
#define MIN_RAND 0xf0
#include<string>
#include<map>
#include<iostream>
#include"byte.h"
#include"hasher.h"
using rho = std::map<std::string,int>;
//属性布隆过滤器
class abf
{
    private:
            uint8_t* atte;//用以存放映射属性布隆过滤其中的行号下标的加密串
            hasher* H;//使用的lhash个哈希函数
            int labf;//表示可以映射比特串的长度
            int lhash;//表示使用的哈希函数的个数
            void abfmap(const std::string& att,const int rowid);
    public:
            abf(int lf = 0, int lh = 3);
            ~abf();
            abf(const abf& a);
            void abfbuild(rho& attset);
            void printABF();
};

#endif
