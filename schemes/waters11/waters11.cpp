#include"waters11.h"


waterskeypair&
waters11::setup(const std::vector<std::string>& atts,pairingparam& pairing)
{
    element_t generator;
    element_init_G1(generator,pairing.p);
    element_random(generator);

    element_t alpha;
    element_init_Zr(alpha,pairing.p);
    element_random(alpha);

    element_t a;
    element_init_Zr(a,pairing.p);
    element_random(a);

    element_t omega;
    element_init_GT(omega,pairing.p);
    pairing_apply(omega,generator,generator,pairing.p);
    element_pow_zn(omega,omega,alpha);

    element_t ga;
    element_init_G1(ga,pairing.p);
    element_pow_zn(ga,generator,a);

    element_t msk;
    element_init_G1(msk,pairing.p);
    element_pow_zn(msk,generator,alpha);

    std::map<std::string,element_t> h;
    for(auto att: atts)
    {
        element_init_G1(h[att],pairing.p);
        element_random(h[att]);
    }
    masterkey mk(msk);
    waterspublickey pk(generator,omega,ga,h);
    static waterskeypair keypair(pk,mk);

    element_clear(generator);
    element_clear(omega);
    element_clear(msk);
    element_clear(ga);
    element_clear(alpha);
    element_clear(a);
    for(auto att: atts)
    {
        element_clear(h[att]);
    }
    return keypair;
}

waterscipher&
waters11::enc(waterspublickey& pk,accesspolicy& policy,message& m,pairingparam& pairing)
{
    ElementMatrix M(policy.getpolicy(),pairing.p);
    ElementVector v(M.getColNum(),pairing.p,Random);
    ElementVector lambda{M.multiVector(v)};

    element_t C;
    element_init_GT(C,pairing.p);
    element_pow_zn(C,pk.omega,&(v.getElementAt(0)));
    element_mul(C,C,m.ms);

    element_t C_;
    element_init_G1(C_,pairing.p);
    element_pow_zn(C_,pk.generator,&(v.getElementAt(0)));

    attstoelement ci;
    attstoelement di;
    attstoelement ri;
    element_t gal;
    element_t hrho;
    element_init_G1(gal,pairing.p);
    element_init_G1(hrho,pairing.p);
    for(int i = 0; i < lambda.size(); ++i)
    {
        std::string att{M.rowMapToAtts(i)};
        element_init_Zr(ri[att],pairing.p);
        element_init_G1(ci[att],pairing.p);
        element_init_G1(di[att],pairing.p);
        element_random(ri[att]);
        element_pow_zn(di[att],pk.generator,ri[att]);
        element_neg(ri[att],ri[att]);

        element_pow_zn(gal,pk.ga,&(lambda.getElementAt(i)));
        element_pow_zn(hrho,pk.hi[att],ri[att]);
        element_mul(ci[att],gal,hrho);
    }
    
    static waterscipher wc(C,C_,ci,di,M);
    element_clear(C);
    element_clear(C_);
    element_clear(gal);
    element_clear(hrho);
    for(attstoelement::iterator ite = ci.begin(); ite != ci.end(); ++ite)
        element_clear(ite->second);
    for(attstoelement::iterator ite = di.begin(); ite != di.end(); ++ite)
        element_clear(ite->second);
    for(attstoelement::iterator ite = ri.begin(); ite != ri.end(); ++ite)
        element_clear(ite->second);
    return wc;
}

watersusersecretkey&
waters11::keygen(const std::vector<std::string>& useratts,waterspublickey& pk,masterkey& msk,pairingparam& pairing)
{
    element_t t;
    element_init_Zr(t,pairing.p);
    element_random(t);

    element_t K;
    element_init_G1(K,pairing.p);
    element_pow_zn(K,pk.ga,t);
    element_mul(K,K,msk.alpha);

    element_t L;
    element_init_G1(L,pairing.p);
    element_pow_zn(L,pk.generator,t);

    attstoelement Kx;
    for(auto att:useratts)
    {
        element_init_G1(Kx[att],pairing.p);
        element_pow_zn(Kx[att],pk.hi[att],t);
    }
    static watersusersecretkey userkey(K,L,Kx);
    element_clear(t);
    element_clear(K);
    element_clear(L);
    for(auto att:useratts)
    {
        element_clear(Kx[att]);
    }
    return userkey;
}

message&
waters11::dec(waterscipher& c,watersusersecretkey& userkey,pairingparam& pairing)
{
    std::vector<std::string> useratts;
    for(attstoelement::iterator ite = userkey.Kx.begin(); ite != userkey.Kx.end(); ++ite) 
        useratts.push_back(ite->first);
    
    ElementMatrix submatrix(c.M.getSubMatrix(useratts));

    ElementMatrix A(submatrix.trans());
    ElementVector b(A.getRowNum(),pairing.p,Specify);
    ElementVector solution = A.solve(b);
    AttsMapToElement omega(useratts,solution);

    element_t numerator;
    element_init_GT(numerator,pairing.p);
    pairing_apply(numerator,c.C_,userkey.K,pairing.p);

    element_t CL;
    element_init_GT(CL,pairing.p);
    element_t DK;
    element_init_GT(DK,pairing.p);
    element_t CLDK;
    element_init_GT(CLDK,pairing.p);
    
    element_t denominator;
    element_init_GT(denominator,pairing.p);
    element_set1(denominator);

    for(auto att: useratts)
    {
        if(!c.attIsInCipher(att))
            continue;
        pairing_apply(DK,c.Di[att],userkey.Kx[att],pairing.p);
        pairing_apply(CL,c.Ci[att],userkey.L,pairing.p);
        element_mul(CLDK,CL,DK);
        element_pow_zn(CLDK,CLDK,&(omega.getElement(att)));
        element_mul(denominator,denominator,CLDK);
    }

    element_t m;
    element_init_GT(m,pairing.p);
    element_div(m,numerator,denominator);
    element_div(m,c.C,m);
    static message re(m); 
    element_clear(numerator);
    element_clear(CL);
    element_clear(DK);
    element_clear(CLDK);
    element_clear(denominator);
    element_clear(m);
    return re;
}



int main(int argc,char* argv[])
{   
    clock_t time;
    double total_time{0};
    static pairingparam pairing(curve_param::a_param.c_str());
    pairing_init_set_str(pairing.p,curve_param::a_param.c_str());


    std::cout<<"########################################### Setup Begin ###########################################\n";
    time = clock();
    waterskeypair &key(waters11::setup(abeparam::universalAttributes_example_01,pairing));
    time = clock() - time;
    std::cout<<"########################################### Setup End ###########################################\n";
    std::cout<<"the time of Setup phase:\t" << static_cast<float>(time)/CLOCKS_PER_SEC << "s\n";
    total_time += static_cast<float>(time)/CLOCKS_PER_SEC;
    
    accesspolicy policy(abeparam::accessPolicy_example_02);
    element_t m;
    element_init_GT(m,pairing.p);
    element_random(m);
    message mes(m);//被加密的消息
    std::cout<<"########################################### Enc Begin ###########################################\n";
    time = clock();
    waterscipher& c(waters11::enc(key.pk,policy,mes,pairing));
    time = clock() - time;
    std::cout<<"########################################### Enc End ###########################################\n";
    std::cout<<"the time of Enc phase:\t" << static_cast<float>(time)/CLOCKS_PER_SEC << "s\n";
    total_time += static_cast<float>(time)/CLOCKS_PER_SEC;


    std::cout<<"########################################### KeyGen Begin ###########################################\n";
    time = clock();
    watersusersecretkey& userkey(waters11::keygen(abeparam::userAttributes_satisfied_example_02,key.pk,key.msk,pairing));
    time = clock() - time;
    std::cout<<"########################################### KeyGen End ###########################################\n";
    std::cout<<"the time of KeyGen phase:\t" << static_cast<float>(time)/CLOCKS_PER_SEC << "s\n";
    total_time += static_cast<float>(time)/CLOCKS_PER_SEC;

    std::cout<<"########################################### Dec Begin ###########################################\n";
    time = clock();
    message& mes_(waters11::dec(c,userkey,pairing));
    time = clock() - time;
    std::cout<<"########################################### Dec End ###########################################\n";
    std::cout<<"the time of Dec phase:\t" << static_cast<float>(time)/CLOCKS_PER_SEC << "s\n";
    total_time += static_cast<float>(time)/CLOCKS_PER_SEC;

    if(mes == mes_)
    {
        std::cout<<"decrypt is correct!\n";
        std::cout<<"Water11Scheme total time:\t" << total_time << "s\n";
    }
    else
        std::cout<<"decrypt is not correct!\n";

    element_clear(m);
    return 0;
}