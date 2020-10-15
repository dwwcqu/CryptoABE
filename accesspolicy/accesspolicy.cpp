#include"accesspolicy.h"

accesspolicy::accesspolicy()
{
}
accesspolicy::accesspolicy(std::string policy):policy{policy}
{
}
std::string accesspolicy::getpolicy() const
{
    return policy;
}

bool accesspolicy::isvalidpolicy()
{
    static std::stack<std::string> policystack;
    policystack.push(policy);
    while(!policystack.empty())
    {
        std::string spolicy {policystack.top()};
        policystack.pop();
        int len = spolicy.length();
        if(spolicy[0] == '(' && spolicy[len - 1] == ')')
        {
            spolicy = spolicy.substr(1,len - 2);
            int rightcommaindex = getfirstcommaindex(spolicy);
            if(rightcommaindex == -1)
                return false;
            
            std::string threshold = spolicy.substr(rightcommaindex + 1);
            int tranInt = std::stoi(threshold);
            if(tranInt <= 0 )
                return false;

            spolicy = spolicy.substr(0,rightcommaindex);
            while(spolicy != "")
            {
                int firstcomma = getfirstcommaindex(spolicy);
                if(firstcomma == -1)
                {
                    policystack.push(spolicy);
                    spolicy = "";

                }
                else
                {
                    policystack.push(spolicy.substr(firstcomma + 1));
                    spolicy = spolicy.substr(0,firstcomma);
                }  
            }
        }
        else
        {
            for(int i = 0; i < len; ++i)
            {
                if((spolicy[i] >= '0' && spolicy[i] <= '9') || (spolicy[i] >= 'a' && spolicy[i] <= 'z') || (spolicy[i] >= 'A' && spolicy[i] <= 'Z'))
                    ;
                else
                    return false;
            }
        }
         
    }
    return true;
}

int getfirstcommaindex(std::string& policy)
{
    int len = policy.length();
    bool isinparenthesis {false};
    for(int i = len - 1; i >=0; --i)
    {
        if(policy[i] == ')')
        {
            isinparenthesis = true;
            continue;
        }
        if(policy[i] == '(')
        {
            isinparenthesis = false;
            continue;
        }
        if(policy[i] == ',' && !isinparenthesis)
            return i;
    }
    return -1;//代表不存在逗号
}