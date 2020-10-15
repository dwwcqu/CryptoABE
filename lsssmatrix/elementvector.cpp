/*
 * author: dengweiwei
 * 2020-09-30
 */
#include"elementvector.h"

ElementVector::
        ElementVector(int len,pairing_t pairing,ElementVectorCate evc)
{
    length = len;
    elementVector = new element_s[len];
    switch(evc)
    {
        case Random :
                for(int i = 0; i < len; ++i)
                {
                    element_init_Zr(&(elementVector[i]),pairing);
                    element_random(&(elementVector[i]));
                }
                break;
        case Specify:
                for(int i = 0; i < len; ++i)
                {
                    element_init_Zr(&(elementVector[i]),pairing);
                    if(i == 0)
                        element_set1(&(elementVector[i]));
                    else
                        element_set0(&(elementVector[i]));
                }
                break;
        default: 
                std::cout<<"please choose correct vector!\n";
                break;
    }
    
    //std::cout<<"ElementVector Construction\n";
}

ElementVector::ElementVector(){}

ElementVector::ElementVector(const ElementVector & ev):length{ev.length}
{
    elementVector = new element_s[ev.length];
    for(int i = 0; i < length; ++i)
    {
        element_init_same_as(&(elementVector[i]),&(ev.elementVector[i]));
        element_set(&(elementVector[i]),&(ev.elementVector[i]));
    }
}

ElementVector::~ElementVector()
{
    for(int i = 0; i < length; ++i)
        element_clear(&(elementVector[i]));
    delete[] elementVector;
    //std::cout<<"ElementVector Deconstruction\n";
}

int ElementVector::size() const
{
    return length;
}

void ElementVector::printElementVector()const
{
    for(int i = 0; i < length; ++i)
        element_printf("Element[%d] = %B\n", i+1,&(elementVector[i]));
    std::cout<<"------------------------------------------------------------------------------------------------------------\n";
}

element_s& ElementVector::getElementAt(int index)
{
    assert(index >=0 && index < length && "ElementVector::getElementAt(int index):the index is out range of the vector");
    return elementVector[index];
}

void
ElementVector::multiElementVector(ElementVector& ev,element_t& result)
{
    assert(length == ev.length && "ElementVector::multiElementVector(ElementVector& ev,element_t& result):the length of two vectors is not equal");
    assert(length != 0 && "ElementVector::multiElementVector(ElementVector& ev,element_t& result): length must bigger than 0");
    element_t total;
    element_t temp;
    element_init_same_as(temp,&(elementVector[0]));
    element_init_same_as(total,&(elementVector[0]));
    element_set0(total);
    for(int i = 0; i < length; ++i)
    {
        element_mul(temp,&(elementVector[i]),&(ev.elementVector[i]));
        element_add(total,total,temp);
    }
    element_set(result,total);
    element_clear(temp);
    element_clear(total);
}