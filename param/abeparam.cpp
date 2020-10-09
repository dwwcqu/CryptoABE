#include"abeparam.h"

const std::vector<std::string> abeparam::universalAttributes_example_01 
                                            {"A","B","C","D","E","F","G","H","M","N","P","Q"};


const std::string abeparam::accessPolicy_example_01 {"((A,B,D,2),(F,H,1),2)"};
const std::string abeparam::accessPolicy_example_02 {"((A,B,D,2),(E,F,H,2),2)"};
const std::string abeparam::accessPolicy_example_03 {"((A,B,D,2),(M,N,P,Q,2),(E,F,H,2),2)"};
const std::string abeparam::accessPolicy_example_04 {"((A,B,D,1),(M,N,P,Q,1),(E,F,H,1),1)"};

const std::vector<std::string> abeparam::userAttributes_satisfied_example_01 {"A","B","F"};
const std::vector<std::string> abeparam::userAttributes_satisfied_example_02 {"A","B","E","F"};
const std::vector<std::string> abeparam::userAttributes_satisfied_example_03 {"A","B","M","Q","E","F"};
const std::vector<std::string> abeparam::userAttributes_satisfied_example_04 {"A"};

const std::vector<std::string> abeparam::userAttributes_unsatisfied_example_01 {"A","F","H"};
const std::vector<std::string> abeparam::userAttributes_unsatisfied_example_02 {"A","E","F","H"};
const std::vector<std::string> abeparam::userAttributes_unsatisfied_example_03 {"A","M","F","H"};
const std::vector<std::string> abeparam::userAttributes_unsatisfied_example_04 {"C"};