#include"Waters11Scheme.h"


void waters11Scheme()
{
    clock_t time;
    double totaltime{0};
    std::cout << "#################################### Setup Begin ####################################\n";
    time = clock();
    pairing_t pairing;
    pairing_init_set_str(pairing,dww_param_a);

    //群G1上的生成元
    element_t generator;
    element_init_G1(generator,pairing);
    element_random(generator);

    //系统的主密钥
    element_t alpha;
    element_init_Zr(alpha,pairing);
    element_random(alpha);

    element_t a;
    element_init_Zr(a,pairing);
    element_random(a);

    //系统的公钥
    element_t omega;
    element_init_GT(omega,pairing);
    pairing_apply(omega,generator,generator,pairing);
    element_pow_zn(omega,omega,alpha);

    element_t ga;
    element_init_G1(ga,pairing);
    element_pow_zn(ga,generator,a);

    element_t msk;
    element_init_G1(msk,pairing);
    element_pow_zn(msk,generator,alpha);

    //系统全局属性以及对应的代数结构上的元素
    AttsMapToElement hn(universalAttributes_example_01,pairing,G1);
    time = clock() - time;
    std::cout<<"the time of Setup phase:\t" << static_cast<float>(time)/CLOCKS_PER_SEC << "s\n";
    totaltime += static_cast<float>(time)/CLOCKS_PER_SEC;
    std::cout << "#################################### Setup End ####################################\n";
    std::cout << "#################################### Encrypt Begin ####################################\n";
    time = clock();
    element_t M;//代表在GT上的元素为一个消息
    element_init_GT(M,pairing);
    element_random(M);

    LSSSMatrix lsssmatrix(accessPolicy_example_02);

    ElementMatrix elementM(lsssmatrix.getLsssMatrix(),pairing);

    std::vector<std::string> accessPolicyAttSet {lsssmatrix.getAttributesSet()};

    ElementVector sharedValueAndRandom(elementM.getColNum(),pairing,Random);

    ElementVector sharedValueOfAttrs = elementM.multiVector(sharedValueAndRandom);

    AttsMapToElement lambda(accessPolicyAttSet,sharedValueOfAttrs);
    
    element_t C;
    element_init_GT(C,pairing);
    element_pow_zn(C,omega,&(sharedValueAndRandom.getElementAt(0)));//C = M * e(g,g) ^ alpha * s
    element_mul(C,C,M);

    element_t C_;
    element_init_G1(C_,pairing);
    element_pow_zn(C_,generator,&(sharedValueAndRandom.getElementAt(0)));//C' = g ^ s


    AttsMapToElement rl(accessPolicyAttSet,pairing,Zr);

    AttsMapToElement Cl(accessPolicyAttSet,pairing,G1);
    AttsMapToElement Dl(accessPolicyAttSet,pairing,G1);

    element_t galamda;
    element_t hri;
    element_init_G1(galamda,pairing);
    element_init_G1(hri,pairing);
    for(auto att: accessPolicyAttSet)
    {
        element_pow_zn(&(Dl.getElement(att)),generator,&(rl.getElement(att)));
        element_pow_zn(galamda,ga,&(lambda.getElement(att)));
        element_neg(&(rl.getElement(att)),&(rl.getElement(att)));
        element_pow_zn(hri,&(hn.getElement(att)),&(rl.getElement(att)));
        element_mul(&(Cl.getElement(att)),galamda,hri);
    }
    element_clear(galamda);
    element_clear(hri);
    time = clock() - time;
    std::cout<<"the time of Encrypt phase:\t" << static_cast<float>(time)/CLOCKS_PER_SEC << "s\n";
    totaltime += static_cast<float>(time)/CLOCKS_PER_SEC;
    std::cout << "#################################### Encrypt End ####################################\n";
    std::cout << "#################################### KeyGen Begin ####################################\n";
    time = clock();
    element_t t;
    element_init_Zr(t,pairing);
    element_random(t);

    element_t L;
    element_init_G1(L,pairing);
    element_pow_zn(L,generator,t);//L = g ^ t

    element_t K;
    element_init_G1(K,pairing);
    element_t temp;
    element_init_G1(temp,pairing);
    element_pow_zn(temp,ga,t);
    element_set(K,msk);
    element_mul(K,K,temp);//K = msk * (g ^ a*t)
    element_clear(temp);

    AttsMapToElement Kx(userAttributes_satisfied_example_02,pairing,G1);
    for(auto att: userAttributes_satisfied_example_02)
        element_pow_zn(&(Kx.getElement(att)),&(hn.getElement(att)),t);//Kx = hx ^ t
    time = clock() - time;
    std::cout<<"the time of KeyGen phase:\t" << static_cast<float>(time)/CLOCKS_PER_SEC << "s\n";
    totaltime += static_cast<float>(time)/CLOCKS_PER_SEC;
    std::cout << "#################################### KeyGen End ####################################\n";
    std::cout << "#################################### Decrypt Begin ####################################\n";
    time = clock();
    //根据用户的属性集合获得秘密分享矩阵的一个子矩阵，即对应属性的行构成的矩阵
    std::vector<std::vector<int>> sublsssmatirx = lsssmatrix.getSubMatrix(userAttributes_satisfied_example_02);
    ElementMatrix subElementM(sublsssmatirx,pairing);
    
    ElementMatrix trans = subElementM.trans();

    ElementVector b(trans.getRowNum(),pairing,Specify);
    ElementVector solution = trans.solve(b);

    AttsMapToElement lomega(userAttributes_satisfied_example_02,solution);

    element_t C_K;
    element_init_GT(C_K,pairing);
    pairing_apply(C_K,C_,K,pairing);

    element_t multi;
    element_init_GT(multi,pairing);
    element_set1(multi);

    element_t CL;
    element_init_GT(CL,pairing);//CL = e(Ci,L)
    element_t DK;
    element_init_GT(DK,pairing);
    element_t CLDK;
    element_init_GT(CLDK,pairing);
    for(auto att: userAttributes_satisfied_example_02)
    {
        if(!Cl.isInAttributeSet(att))
            continue;
        else
        {
            pairing_apply(CL,&(Cl.getElement(att)),L,pairing);//CL = e(Ci,L)
            pairing_apply(DK,&(Dl.getElement(att)),&(Kx.getElement(att)),pairing);//DK = e(Di,Ki)
            element_mul(CLDK,CL,DK);// CLDK = CL * DK = e(Ci,L) * e(Di,Ki)
            element_pow_zn(CLDK,CLDK,&(lomega.getElement(att)));//CLDK ^ lomegai = (CL * DK) ^ lomegai
            element_mul(multi,multi,CLDK);//累乘
        }
    }
    element_t omegas;
    element_init_GT(omegas,pairing);
    element_div(omegas,C_K,multi);

    element_t M_;
    element_init_GT(M_,pairing);
    element_div(M_,C,omegas);
    if(!element_cmp(M,M_))
        std::cout<<"Decryption is successful!\n";
    else
        std::cout<<"Decryption is not correct!\n";
    time = clock() - time;
    std::cout<<"the time of Decrypt phase:\t" << static_cast<float>(time)/CLOCKS_PER_SEC << "s\n";
    totaltime += static_cast<float>(time)/CLOCKS_PER_SEC;
    std::cout << "#################################### Decrypt End ####################################\n";
    std::cout<<"Water11Scheme total time:\t" << totaltime << "\n";
    element_clear(generator);
    element_clear(alpha);
    element_clear(a);
    element_clear(omega);
    element_clear(ga);
    element_clear(msk);
    element_clear(M);
    element_clear(C);
    element_clear(t);
    element_clear(L);
    element_clear(C_K);
    element_clear(CL);
    element_clear(DK);
    element_clear(multi);
    element_clear(CLDK);
    element_clear(omegas);
    pairing_clear(pairing);
}


int main(int argc,char** argv)
{

    waters11Scheme();

    /*
    pairing_t pairing;
    pairing_init_set_str(pairing,dww_param_a);

    LSSSMatrix lsssmatrix (accessPolicy_example_04);


    //std::vector<std::vector<int>> submatrix = lsssmatrix.getSubMatrix(userAttributes_satisfied_example_01);

    ElementMatrix em(lsssmatrix.getLsssMatrix(),pairing);
    std::cout<<"\t\tlsss matrix \n";
    em.printElementMatrix();
    ElementVector shareEV(em.getColNum(),pairing,Random);
    std::cout<<"\t\tshared value\n";
    shareEV.printElementVector();
    std::cout<<"\t\tthe shared value from LSSS Matrix\n";
    ElementVector lamda = em.multiVector(shareEV);
    lamda.printElementVector();

    ElementMatrix trans = em.trans();
    std::cout<<"\t\tthe transposition of lsssmatrix\n";
    trans.printElementMatrix();
    ElementVector b(trans.getRowNum(),pairing,Specify);
    std::cout<<"\t\tspecial vector \n";
    b.printElementVector();
    ElementMatrix A_b = trans.addColumuAtLast(b);
    A_b.rowSetpMatrixTrans();
    std::cout<<"\t\tA_b's row setp matrix\n";
    A_b.printElementMatrix();


    ElementVector solution = trans.solve(b);
    std::cout<<"\t\tsolution vector \n";
    solution.printElementVector();
    ElementVector mul = trans.multiVector(solution);
    std::cout<<"\t\ttrans * solution\n";
    mul.printElementVector();
    element_t re;
    element_init_Zr(re,pairing);
    solution.multiElementVector(lamda,re);
    std::cout<<"\t\trecoved value\n";
    element_printf("reconverd s = %B\n",re);
    element_clear(re);

    pairing_clear(pairing);
    */
    return 0;
}
