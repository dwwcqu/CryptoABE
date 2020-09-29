#include"AttsMapToElement.h"

AttsMapToElement::AttsMapToElement(const std::vector<std::string>& attrs,pairing_t pairing,AlgebraicS c)
{
    this->length = attrs.size();
    this->attrs = new std::string[attrs.size()];
    this->eles = new element_s[attrs.size()];
    for(int i = 0; i < attrs.size(); ++i)
    {
        this->attrs[i] = attrs[i];
        switch(c)
        {
            case Zr: 
                    element_init_Zr(&(this->eles[i]),pairing);
                    element_random(&(this->eles[i]));
                    break;
            case G1: 
                    element_init_G1(&(this->eles[i]),pairing);
                    element_random(&(this->eles[i]));
                    break;
            case G2: 
                    element_init_G2(&(this->eles[i]),pairing);
                    element_random(&(this->eles[i]));
                    break;
            case GT: 
                    element_init_GT(&(this->eles[i]),pairing);
                    element_random(&(this->eles[i]));
                    break;
            default:
                    std::cout<<"this algebraics is not exit!\n";
                    exit(1);
        }
    }
}


AttsMapToElement::AttsMapToElement(const std::vector<std::string>& attrs,ElementVector& eles)
{

    if(attrs.size() != eles.size())
    {
        std::cout<<"can't to initializing, becasue the length of the two parameters is not same\n";
        exit(1);
    }
    this->length = attrs.size();
    this->attrs = new std::string[attrs.size()];
    this->eles = new element_s[attrs.size()];
    for(int i = 0; i < attrs.size(); ++i)
    {
        this->attrs[i] = attrs[i];
        element_init_same_as(&(this->eles[i]),&(eles.getElementAt(i)));
        element_set(&(this->eles[i]),&(eles.getElementAt(i)));
    }
}


AttsMapToElement::~AttsMapToElement()
{
    //for(int i = 0; i < this->length; ++i)
        //element_clear(&(this->eles[i]));
    delete[] this->attrs;
    delete[] this->eles;
}

int
AttsMapToElement::findAttIndex(std::string& att)
{
    for(int i = 0; i < this->length; ++i)
        if(this->attrs[i] == att)
            return i;
    return -1;
}


element_s&
AttsMapToElement::getElement(std::string& att)
{
    int index = this->findAttIndex(att);
    if(index == -1 )
    {
        std::cout<<"this attribute is not exist!\n";
        exit(1);
    }
    else
        return this->eles[index];
}

int
AttsMapToElement::size()
{
    return this->length;
}


bool
AttsMapToElement::isInAttributeSet(std::string& att)
{
    if(this->findAttIndex(att) == -1)
        return false;
    return true;
}

void
AttsMapToElement::printAttsAndElement()
{
    for(int i = 0; i < this->length; ++i)
    {
        std::cout<<this->attrs[i]<<" = ";
        element_printf("%B",&(this->eles[i]));
        std::cout<<'\n';
    }
}