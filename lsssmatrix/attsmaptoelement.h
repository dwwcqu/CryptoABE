#ifndef ATTSMAPTOELEMENT_H
#define ATTSMAPTOELEMENT_H
#include<vector>
#include<iostream>
#include<string>
#include"pbc/pbc.h"
#include"elementvector.h"
//#include"pbc/pbc_test.h"
enum AlgebraicS{
    Zr,G1,G2,GT
};


class AttsMapToElement
{
private:
        std::string* attrs;
        element_s* eles;
        int length{0};
        int findAttIndex(std::string& att);
public:
        AttsMapToElement(const std::vector<std::string>& attrs,pairing_t pairing, AlgebraicS c = Zr);
        AttsMapToElement(const std::vector<std::string>& attrs,ElementVector& eles);
        ~AttsMapToElement();
        element_s& getElement(std::string& att);
        void printAttsAndElement();
        int size();
        bool isInAttributeSet(std::string& att);
};


#endif