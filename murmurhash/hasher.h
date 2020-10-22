#ifndef HASH_H
#define HASH_H

#define MIN_SEED 0x10000000//用于产生随机种子
#define MAX_SEED 0xffffffff

#include <stdio.h>
#include<cstdlib>
#include<ctime>
#include<iostream>
#include<string>
#include<string.h>
#include"MurmurHash3.h"


class hasher
{
    private:
        uint32_t SEED;//哈希种子
        int space;//映射空间长度
    public:
        hasher();
        hasher(int s);
        hasher(const hasher& h);
        int hash(const std::string& message);
        void setSpace(int s);//设置映射空间大小
        void printSEED();
};
#endif