#ifndef BASESCHEME_H
#define BASESCHEME_H

class basescheme
{
    public:
        virtual void setup();
        virtual void keygen();
        virtual void enc();
        virtual void dec();
};
#endif