#ifndef ELEMENTINZRVECTOR_H
#define ELEMENTINZRVECTOR_H


#include<iostream>
#include"pbc/pbc.h"
#include"ElementInMPQVector.h"

//#include"ElementInZrMatrix.h"



enum ElementVectorCate 
{
        Random,Specify
};


//列向量
class ElementVector
{
private:
        int length{0};
        element_s* elementVector;
        friend class ElementMatrix;
public:
        ElementVector(int len,pairing_t pairing,ElementVectorCate evc = Specify);
        ElementVector(MPQVector& mpqv,pairing_t pairing);
        ElementVector();
        ElementVector(const ElementVector & ev);
        ~ElementVector();
        int size();
        void printElementVector();
        element_s& getElementAt(int index);
        void multiElementVector(ElementVector& ev,element_t& result);
};



#endif