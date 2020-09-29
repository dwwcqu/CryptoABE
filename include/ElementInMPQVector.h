#ifndef ELEMENTINMPQVECTOR_H
#define ELEMENTINMPQVECTOR_H


#include<vector>
#include<iostream>
#include"gmp.h"

//mpq元素向量
class MPQVector
{
private:
        MP_RAT* mpqVector;
        int length{0};
public:
        MPQVector(int len);
        MPQVector(const MPQVector & mpqv);
        ~MPQVector();
        void setIntAt(int index,long lv);
        MP_RAT& getAt(int index);
        int size();
        void printVector();
        void setMPQAt(int index,MP_RAT& mp);
};

#endif