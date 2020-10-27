#include"ribf.h"

ribf::ribf(int lf,int lh):labf(lf),lhash(lh)
{
    atte = new uint8_t[lf];
    for(int i = 0; i < lf; ++i)
        atte[i] = 0x00;
    H = new hasher[lh];
    for(int i = 0; i < lhash; ++i)
        H[i].setSpace(labf);
}

ribf::ribf(const ribf& a)
{
    labf = a.labf;
    lhash = a.lhash;
    atte = new uint8_t[labf];
    for(int i = 0; i < labf; ++i)
        atte[i] = a.atte[i];
    H = new hasher[lhash];
    for(int i = 0; i < lhash; ++i)
    {
        H[i].SEED = a.H[i].SEED;
        H[i].setSpace(labf);
    }
}
ribf::~ribf()
{
    delete[] atte;
    delete[] H;
}

void
ribf::ribfmap(const std::string& att,const int rowid)
{
    uint8_t finalshare = static_cast<uint8_t>(rowid);
    int emptypos{-1};//找第一个下标映射为空的下标
    for(int i = 0; i < lhash; ++i)
    {
        int location = H[i].hash(att.c_str());
        std::cout<< "H["<<i<<"] = "<<location<<'\n';
        if(atte[location] == 0x00)//此处代表还未插入任何二进制串
        {
            if(emptypos == -1)
                emptypos = location;
            else
            {
                //srandom((int)time(0));
                uint8_t randid =  rand()%(MAX_SEED - MIN_SEED) + MIN_SEED;
                std::cout<<"rand["<<i<<"] = "<<static_cast<int>(randid)<<"\n";
                atte[location] = randid;
                finalshare = (finalshare xor randid);
            }
        
        }
        else//如果以及存在非空二进制串,直接使用这个位置处的二进制串
            finalshare = (finalshare xor atte[location]);  
         std::cout<<"round "<< i <<" emptypos = "<< emptypos <<'\n';
    }
    atte[emptypos] = finalshare;
}

void
ribf::ribfbuild(rho& attset)
{
    for(rho::iterator ite = attset.begin(); ite != attset.end(); ++ite)
        ribfmap(ite->first,ite->second);
    for(int i = 0; i < labf; ++i)
        if(atte[i] == 0x00)
            atte[i] = rand() % (MAX_SEED - MIN_SEED) + MIN_SEED;
}

int
ribf::ribfquery(const std::string& att)
{
    uint8_t index = 0x00;
    for(int i = 0; i < lhash; ++i)
    {   
        int h = H[i].hash(att.c_str());
        index = (index xor atte[h]);
    }
    return static_cast<int>(index);
}

void
ribf::printRIBF()
{
    for(int i = 0; i < lhash; ++i)
        std::cout<<"SEED " << i + 1 <<" = "<<H[i].SEED<<'\n';
    for(int i = 0; i < labf; ++i)
        std::cout<< "position "<< i+1 << "\t=\t"<< static_cast<int>(atte[i])<<'\n';
    
}
int main(int argc,char* argv[])
{
    ribf a(60,3);
    rho m{{"Aa",1},{"Bb",2},{"Cc",3}};
    a.ribfbuild(m);
    a.printRIBF();
    std::string aa = "Cc";
    std::cout<<"Bb's index is "<<a.ribfquery(aa)<<'\n';
    return 0;
}
