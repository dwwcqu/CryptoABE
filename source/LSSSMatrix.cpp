#include"LSSSMatrix.h"

LSSSMatrix::LSSSMatrix(int threshold,int childNum)
{
    this->rowNum = childNum;
    this->columnNum = threshold;
    for(int i = 1; i <= childNum; i++)
    {
        std::vector<int> row{};
        for(int j = 0; j < threshold; j++)
            row.push_back(pow(i,j));
        this->lsssmatrix.push_back(row);
    }
}
LSSSMatrix::LSSSMatrix()
{

}

/*
void LSSSMatrix::toDoubleDimenArray(int intArray[][])
{
    for(int i = 0; i < this->rowNum; ++i)
    {

    }
}
*/

LSSSMatrix::LSSSMatrix(const std::string& accesspolicy)
{
    //用于迭代遍历使用的栈
    std::stack<std::string> policyStack;
    policyStack.push(accesspolicy);
    int insertIndex{1};
    while(!policyStack.empty())
    {
        std::string topPolicy = policyStack.top();
        policyStack.pop();

        //如果栈的最上面的元素代表为一个属性，即叶子节点，则加入到属性集合
        if(isAttribute(topPolicy))
        {
            this->attributeSet.push_back(topPolicy);
            ++insertIndex;
        }
        //否则代表为一个内部节点，需要对从根节点便利到叶子节点上
        else
        {
            //获取当前节点的孩子节点数以及阈值数
            int *ptr {parseStringPolicy(topPolicy,policyStack)};
            this->insertNodeMatrixAt(getNodeMatrix(*ptr,*( ptr + 1)),insertIndex);
        }

    }

}
void LSSSMatrix::printLsssMatrix()
{
    for(int i = 0; i < this->rowNum; i++)
    {
        std::cout << this->attributeSet.at(i)<<"  ";
        for(int j = 0; j < this->columnNum; j++)
            std::cout << this->lsssmatrix.at(i).at(j)<< '\t';
        std::cout << '\n';
    }
}

//返回行数
const int LSSSMatrix::getRowNum()
{
    return this->rowNum;
}

//返回列数
const int LSSSMatrix::getColumnNum()
{
    const int colN = this->columnNum;
    return colN;
}

//析构函数
LSSSMatrix::~LSSSMatrix()
{

}

//返回矩阵的第index行的元素
std::vector<int> LSSSMatrix::getRowAt(int index)
{
    if(index >= this->rowNum || index < 1)
    {
        std::cout<< "this index " << index << " is out the matirx row\n";
        exit(1);
    }
    return this->lsssmatrix.at(index-1);
}

//返回矩阵的第index列的所有元素
std::vector<int> LSSSMatrix::getColumnAt(int index)
{
    if(index >= this->columnNum || index < 1)
    {
        std::cout<< "this index " << index << " is out the matirx column\n";
        exit(1);
    }
    std::vector<int> col;
    for(int i = 0; i < getRowNum(); ++i)
         col.push_back(this->lsssmatrix.at(i).at(index-1));
    return col;
}

std::vector<std::string>& LSSSMatrix::getAttributesSet()
{
    return this->attributeSet;
}

int
LSSSMatrix::getAttIndex(std::string& att)
{
    for(int i = 0; i < this->attributeSet.size(); ++i)
    {
        if(this->attributeSet.at(i) == att)
            return i;
    }
    return -1;
}


std::vector<std::vector<int>>
LSSSMatrix::getSubMatrix(const std::vector<std::string>& attrs)
{
    std::vector<std::vector<int>> re;
    for(auto att: attrs)
    {
        int index = this->getAttIndex(att);

        if(index == -1)
        //不存在这个属性，跳过
            continue;
        else
            re.push_back(this->lsssmatrix.at(index));
    }
    return re;
}


/*
 * 类操作需要使用的函数
 */
int pow(int base,int index)
{
    int result{1};
    for(int i = 0; i < index; i++)
         result *= base;
    return result;
}


std::vector<std::vector<int>> getNodeMatrix(int threshold,int childNum)
{
    std::vector<std::vector<int>> nodematrix;
    for(int i = 1; i <= childNum; i++)
    {
        std::vector<int> row;
        for(int j = 0; j < threshold; j++)
            row.push_back(pow(i,j));
        nodematrix.push_back(row);
    }
    return nodematrix;
}

void LSSSMatrix::printAttributes()
{
    for(int i = 0; i < this->attributeSet.size(); ++i)
    {

        std::cout<<this->attributeSet.at(i)<<"   ";
    }
}

//在矩阵的第index插入节点矩阵
void LSSSMatrix::insertNodeMatrixAt(std::vector<std::vector<int>> nodematrix,int index)
{
    if(index < 1 || index > this->rowNum)
    {
        std::cout<< "this index " << index << " is out the matirx row\n";
        exit(1);
    }
    //获取节点矩阵的行数
    int nodeRowNum = nodematrix.size();

    //获取节点矩阵的列数
    int nodeColNum = nodematrix.at(0).size();


    //处理在index之前的行
    for(int i = 0; i < index - 1; ++i)
    {

        for(int j = 1; j < nodeColNum; ++j)
        {
            this->lsssmatrix.at(i).push_back(0);
        }
    }

    //处理从index开始的nodeRowNum行
    for(int i = 0; i < nodeRowNum; ++i)
    {
        //此时处在第index这一行
        if(i == 0)
        {
            for(int j = 1; j < nodeColNum; ++j)
                this->lsssmatrix.at(index - 1).push_back(nodematrix.at(i).at(j));
        }
        //后续在增加 nodeRowNum - 1 行
        else
        {
            std::vector<int> newRow{};
            for(int k = 0; k < columnNum; ++k)
                newRow.push_back(this->lsssmatrix.at(index - 1).at(k));
            for(int j = 1; j < nodeColNum; ++j)
                newRow.push_back(nodematrix.at(i).at(j));
            this->lsssmatrix.insert(this->lsssmatrix.begin() + index - 1 + i,newRow);
        }
    }

    //处理index后面的行
    for(int i = index + nodeRowNum - 1; i < this->rowNum + nodeRowNum - 1; ++i)
    {
        for(int j = 1; j < nodeColNum; j++)
            this->lsssmatrix.at(i).push_back(0);
    }

    this->rowNum += nodeRowNum - 1;
    this->columnNum += nodeColNum - 1;
}

int* parseStringPolicy(std::string accesspolicy,std::stack<std::string> &policyStack)
{
    //去掉两端的方括号
    std::string parseStr = accesspolicy.substr(1,accesspolicy.length() - 2);

    int nodeNum{0};//代表这个节点的孩子节点的个数
    //代表这个节点的阈值
    int threshold{std::stoi(parseStr.substr(findLastComma(parseStr) + 1))};

    //去掉最后的数字和逗号
    parseStr = parseStr.substr(0,parseStr.length() - 2);

    while(parseStr.length() != 0)
    {
        int leftParentheNum{0};//左括号数量
        int rightParentheNum{0};//右括号数量
        int leftParentheIndex{0};//左括号的下标
        int rightParentheIndex{0};//右括号的下标
        for(int i = parseStr.length() - 1; i >= 0 ; --i)
        {
            if(parseStr[i] == '(')
            {
                ++leftParentheNum;
                leftParentheIndex = i;
            }
            else if(parseStr[i] == ')')
            {
                ++rightParentheNum;
                rightParentheIndex = i;
            }
            if(leftParentheNum == rightParentheNum && leftParentheNum != 0)
                break;
        }
        //左括号数量与右括号数量一样,且数量不为0时，代表找到一对括号
        if(leftParentheNum == rightParentheNum && leftParentheNum != 0)
        {
            ++nodeNum;//找到一个子结点，数量加1
            //子结点入栈
            policyStack.push(parseStr.substr(leftParentheIndex));
            //去掉子结点
            parseStr = parseStr.substr(0,leftParentheIndex);
            //如果有逗号，去掉最后的逗号
            if(parseStr[parseStr.length() - 1] == ',')
                parseStr = parseStr.substr(0,parseStr.length() - 1);
        }
        //否则代表这个节点里面全为叶子节点，即为属性
        else
        {
            int lastCommaIndex = findLastComma(parseStr);
            //代表有逗号存在
            if(lastCommaIndex != -1)
            {
                ++nodeNum;
                policyStack.push(parseStr.substr(lastCommaIndex + 1));
                parseStr = parseStr.substr(0,lastCommaIndex);
            }
            //只剩下最后一个属性
            else
            {
                ++nodeNum;
                policyStack.push(parseStr);
                parseStr = parseStr.substr(0,0);
            }

        }

    }
    static int result[2]{0};
    result[0] = threshold;
    result[1] = nodeNum;
    return result;
}

int findLastComma(std::string& str)
{
    for(int i = str.length() - 1; i >= 0; --i)
        if(str[i] == ',')
            return i;
    return -1;//代表不存在逗号
}


//判断当前字符串是否为属性
bool isAttribute(std::string& str)
{
    if(str[0] != '(' && str[str.length() - 1] != ')')
        return true;
    return false;
}


std::vector<std::vector<int>>&
LSSSMatrix::getLsssMatrix()
{
    return this->lsssmatrix;
}