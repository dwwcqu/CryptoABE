#ifndef PARAMETERSUTILS_H
#define PARAMETERSUTILS_H

#include<vector>
#include<string>

const std::vector<std::vector<int>> lsssmatrix_example_01{
                                        {1,1,1,0,0,0,0},{1,1,2,0,0,0,0},{1,1,3,0,0,0,0},
                                        {1,2,0,1,0,0,0},{1,2,0,2,0,0,0},{1,2,0,3,0,0,0},
                                        {1,3,0,0,1,0,0},{1,3,0,0,2,0,0},{1,3,0,0,3,1,1},
                                        {1,3,0,0,3,2,4},{1,3,0,0,3,3,9},{1,3,0,0,3,4,16}
                                        };
const std::vector<std::vector<int>> lsssmatrix_example_02 {
                                                {1,2,3},{3,5,7},
                                                {4,6,8},{9,10,11},
                                                {2,5,6}
                                                };
const std::vector<std::vector<int>> lsssmatrix_example_03 {
                                                {1,1,1},
                                                {1,2,4},
                                                {1,3,9},
                                                {1,4,16},
                                                {1,5,25}
                                                };


const std::vector<std::string> universalAttributes_example_01 {"A","B","C","D","E","F","G","H","M","N","P","Q"};

//const std::vector<std::string> accessPolicy_attributeSet_example_01 {"A","B","D","F","H"};

const std::string accessPolicy_example_01 {"((A,B,D,2),(F,H,1),2)"};
const std::string accessPolicy_example_02 {"((A,B,D,2),(E,F,H,2),2)"};
const std::string accessPolicy_example_03 {"((A,B,D,2),(M,N,P,Q,2),(E,F,H,2),2)"};
const std::string accessPolicy_example_04 {"((A,B,D,1),(M,N,P,Q,1),(E,F,H,1),1)"};

const std::vector<std::string> userAttributes_satisfied_example_01 {"A","B","F"};
const std::vector<std::string> userAttributes_satisfied_example_02 {"A","B","E","F"};
const std::vector<std::string> userAttributes_satisfied_example_03 {"A","B","M","Q","E","F"};
const std::vector<std::string> userAttributes_satisfied_example_04 {"A"};

const std::vector<std::string> userAttributes_unsatisfied_example_01 {"A","F","H"};
const std::vector<std::string> userAttributes_unsatisfied_example_02 {"A","E","F","H"};
const std::vector<std::string> userAttributes_unsatisfied_example_03 {"A","M","F","H"};
const std::vector<std::string> userAttributes_unsatisfied_example_04 {"C"};


const char* dww_param_a {"type a\n\
q 8780710799663312522437781984754049815806883199414208211028653399266475630880222957078625179422662221423155858769582317459277713367317481324925129998224791\n\
h 12016012264891146079388821366740534204802954401251311822919615131047207289359704531102844802183906537786776\n\
r 730750818665451621361119245571504901405976559617\n\
exp2 159\n\
exp1 107\n\
sign1 1\n\
sign0 1\n"};


#endif
