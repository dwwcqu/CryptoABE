#ifndef HASH_H
#define HASH_H

#define OSEED 0x97c29b3a
#include<initializer_list>
#include <stdio.h>
#include<iostream>
#include<string>
#include<string.h>
#include<cassert>
#include<vector>
#include"MurmurHash3.h"


class SEED
{
    public:
            static std::vector<std::string> seeds;
};

std::vector<std::string> seed;
class hasher
{
    private:
        uint32_t SEED;
        static int id;
        int space;
    public:
        hasher(int s = 10);
        hasher(std::initializer_list<int>& list);
        int hash(const std::string& message);
        ~hasher();
};
#endif