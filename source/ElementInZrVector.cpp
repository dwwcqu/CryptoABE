#include"ElementInZrVector.h"

ElementVector::
        ElementVector(int len,pairing_t pairing,ElementVectorCate evc)
{
    this->length = len;
    this->elementVector = new element_s[len];
    switch(evc)
    {
        case Random :
                for(int i = 0; i < len; ++i)
                {
                    element_init_Zr(&(this->elementVector[i]),pairing);
                    element_random(&(this->elementVector[i]));
                }
                break;
        case Specify:
                for(int i = 0; i < len; ++i)
                {
                    element_init_Zr(&(this->elementVector[i]),pairing);
                    if(i == 0)
                        element_set1(&(this->elementVector[i]));
                    else
                        element_set0(&(this->elementVector[i]));
                }
                break;
        default: 
                std::cout<<"please choose correct vector!\n";
                break;
    }
    
    //std::cout<<"ElementVector Construction\n";
}
ElementVector::ElementVector(MPQVector& mpqv,pairing_t pairing)
{
    this->length = mpqv.size();
    this->elementVector = new element_s[mpqv.size()];
    mpq_t rat;
    mpq_init(rat);
    element_t fenzi;
    element_t fenmu;
    element_init_Zr(fenzi,pairing);
    element_init_Zr(fenmu,pairing);
    for(int i = 0; i < this->length; ++i)
    {
        element_init_Zr(&(this->elementVector[i]),pairing);
        mpq_set(rat,&(mpqv.getAt(i)));
        mpq_abs(rat,rat);
        element_set_mpz(fenzi,mpq_numref(rat));//分子从mpq_t类型转为Zr上的元素
        element_set_mpz(fenmu,mpq_denref(rat));//分母从mpq_t类型转为Zr上的元素
        element_div(fenzi,fenzi,fenmu);
        if(mpq_sgn(&(mpqv.getAt(i))) < 0)
            element_neg(&(this->elementVector[i]),fenzi);
        else
            element_set(&(this->elementVector[i]),fenzi);
    }
    mpq_clear(rat);
    element_clear(fenzi);
    element_clear(fenmu);
}


ElementVector::ElementVector(){}

ElementVector::ElementVector(const ElementVector & ev)
{
    this->length = ev.length;
    this->elementVector = new element_s[ev.length];
    for(int i = 0; i < length; ++i)
    {
        element_init_same_as(&(this->elementVector[i]),&(ev.elementVector[i]));
        element_set(&(this->elementVector[i]),&(ev.elementVector[i]));
    }
}

ElementVector::~ElementVector()
{
    for(int i = 0; i < this->length; ++i)
        element_clear(&(this->elementVector[i]));
    delete[] this->elementVector;
    //std::cout<<"ElementVector Deconstruction\n";
}

int ElementVector::size()
{
    return this->length;
}

void ElementVector::printElementVector()
{
    for(int i = 0; i < this->length; ++i)
        element_printf("Element[%d] = %B\n", i+1,&(this->elementVector[i]));
    std::cout<<"------------------------------------------------------------------------------------------------------------\n";
}

element_s& ElementVector::getElementAt(int index)
{
    if(index >= this->length || index < 0)
    {
        std::cout<<"the index is out range the vector\n";
        exit(1);
    }
    return this->elementVector[index];
}

void
ElementVector::multiElementVector(ElementVector& ev,element_t& result)
{
    if(this->length != ev.length)
    {
        std::cout<<"the vector's length is not equal to this vector\n";
        exit(1);
    }
    if(this->length == 0)
    {
        std::cout<<"this vector doesn't have elements\n";
        exit(1);
    }
    element_t total;
    element_t temp;
    element_init_same_as(temp,&(this->elementVector[0]));
    element_init_same_as(total,&(this->elementVector[0]));
    element_set0(total);
    for(int i = 0; i < this->length; ++i)
    {
        element_mul(temp,&(this->elementVector[i]),&(ev.elementVector[i]));
        element_add(total,total,temp);
    }
    element_set(result,total);
    element_clear(temp);
    element_clear(total);
}