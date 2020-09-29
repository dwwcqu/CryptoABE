#include"ElementInMPQVector.h"


MPQVector::MPQVector(int len)
{
    this->length = len;
    this->mpqVector = new MP_RAT[len];
    for(int i = 0; i < len; ++i)
    {
        mpq_init(&(this->mpqVector[i]));
        mpq_set_si(&(this->mpqVector[i]),0,1);
    }
    //mpq_set_si(&(this->mpqVector[0]),1,1);
    //std::cout<<"MPQVector Construction\n";
}

//复制构造
MPQVector::MPQVector(const MPQVector & mpqv)
{
    this->length = mpqv.length;
    this->mpqVector = new MP_RAT[mpqv.length];
    for(int i = 0; i < mpqv.length; ++i)
    {
        mpq_init(&(this->mpqVector[i]));
        mpq_set(&(this->mpqVector[i]),&(mpqv.mpqVector[i]));
    }
}

MPQVector::~MPQVector()
{
    for(int i = 0; i < this->length; ++i)
        mpq_clear(&(this->mpqVector[i]));
    delete[] this->mpqVector;
    //std::cout<<"MPQVector Deconstruction\n";
}

void
MPQVector::setIntAt(int index,long lv)
{
    if(index >= this->length || index < 0)
    {
        std::cout<<"this index is out range of the vector\n";
        exit(1);
    }
    mpq_set_si(&(this->mpqVector[index]),lv,1);
}

void 
MPQVector::setMPQAt(int index,MP_RAT& mp)
{
    if(index >= this->length || index < 0)
    {
        std::cout<<"this index is out range of the vector\n";
        exit(1);
    }
    mpq_set(&(this->mpqVector[index]),&mp);
}


MP_RAT&
MPQVector::getAt(int index)
{
    if(index >= this->length || index < 0)
    {
        std::cout<<"this index is out range of the vector\n";
        exit(1);
    }
    return this->mpqVector[index];
}

int
MPQVector::size()
{
    return this->length;
}

void
MPQVector::printVector()
{
    for(int i = 0; i < this->length; ++i)
        gmp_printf("%Zd/%Zd\n",mpq_numref(&(this->mpqVector[i])),mpq_denref(&(this->mpqVector[i])));
}
