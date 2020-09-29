#include"ElementInMPQMatrix.h"

//使用LSSSMatrix进行构造
MPQMatrix::MPQMatrix(std::vector<std::vector<int>>& lsssmatrix)
{
    this->rowNum = lsssmatrix.size();
    this->colNum = lsssmatrix.at(0).size();
    this->mpqMatrix = new MP_RAT* [this->rowNum];
    for(int i = 0; i < this->rowNum; ++i)
    {
        mpqMatrix[i] = new MP_RAT[this->colNum];
        for(int j = 0; j < this->colNum; ++j)
        {
            mpq_init(&(this->mpqMatrix[i][j]));
            mpq_set_si(&(this->mpqMatrix[i][j]),lsssmatrix[i][j],1);
            mpq_canonicalize(&(this->mpqMatrix[i][j]));
        }
    }
    //std::cout<<"MPQMatrix Construction\n";
}

//简单构造
MPQMatrix::MPQMatrix(){}


//析构函数
MPQMatrix::~MPQMatrix()
{
    for(int i = 0; i < this->rowNum; ++i)
    {
        for(int j = 0; j < this->colNum; ++j)
        {
            mpq_clear(&(this->mpqMatrix[i][j]));
        }
        delete[] this->mpqMatrix[i];
    }
    delete[] mpqMatrix;
    //std::cout<<"MPQMatrix Deconstruction\n";
}

//复制构造函数
MPQMatrix::MPQMatrix(const MPQMatrix & mpqm)
{
    this->rowNum = mpqm.rowNum;
    this->colNum = mpqm.colNum;
    this->mpqMatrix = new MP_RAT* [this->rowNum];
    for(int i = 0; i < this->rowNum; ++i)
    {
        this->mpqMatrix[i] = new MP_RAT [this->colNum];
        for(int j = 0; j < this->colNum; ++j)
        {
            mpq_init(&(this->mpqMatrix[i][j]));
            mpq_set(&(this->mpqMatrix[i][j]),&(mpqm.mpqMatrix[i][j]));
        }
    }
    std::cout<<"MPQMatrix Copy Construction\n";
}

MPQMatrix
MPQMatrix::trans()
{
    MPQMatrix re;
    re.rowNum = this->colNum;
    re.colNum = this->rowNum;
    re.mpqMatrix = new MP_RAT* [this->colNum];
    for(int i = 0; i < this->colNum; ++i)
    {
        re.mpqMatrix[i] = new MP_RAT[this->rowNum];
        for(int j = 0; j < this->rowNum; ++j)
        {
            mpq_init(&(re.mpqMatrix[i][j]));
            mpq_set(&(re.mpqMatrix[i][j]),&(this->mpqMatrix[j][i]));
        }
    }
    return re;
}

MPQMatrix
MPQMatrix::addOneColumnAtLast(MPQVector& mpqv)
{
    if(this->rowNum != mpqv.size())
    {
        std::cout<<"the row number is not equal to vector's length\n";
        exit(1);
    }
    MPQMatrix re;
    re.rowNum = this->rowNum;
    re.colNum = this->colNum + 1;
    re.mpqMatrix = new MP_RAT* [this->rowNum];
    for(int i = 0; i < this->rowNum; ++i)
    {
        re.mpqMatrix[i] = new MP_RAT[this->colNum + 1];
        for(int j = 0; j < this->colNum; ++j)
        {
            mpq_init(&(re.mpqMatrix[i][j]));
            mpq_set(&(re.mpqMatrix[i][j]),&(this->mpqMatrix[i][j]));
        }
        mpq_init(&(re.mpqMatrix[i][this->colNum]));
        mpq_set(&(re.mpqMatrix[i][this->colNum]),&(mpqv.getAt(i)));
    }
    return re;
}

MPQVector
MPQMatrix::fsolve(MPQVector& b)
{
    if(this->rowNum != b.size())
    {
        std::cout<<"the row number of matrix is not equal to vector's length\n";
        exit(1);
    }
    MPQVector re(this->colNum);
    //连接矩阵和向量，然后进行手动变形
    MPQMatrix A_b = this->addOneColumnAtLast(b);
    //进行连接矩阵的行阶梯变换
    A_b.rowSetpMatrixTrans();
    const int minOfRowAndCol = (A_b.rowNum <= A_b.colNum ? A_b.rowNum : A_b.colNum);
    for(int i = minOfRowAndCol; i >= 0; --i)
    {
        int nonZeroIndex = A_b.getFirstNonZeroIndexAtRow(i);
        if(nonZeroIndex == -1)
            continue;
        else if(nonZeroIndex == A_b.colNum - 1)
        {
            std::cout<<"this equation doesn't have solutions\n";
            exit(0);
        }
        else
            mpq_set(&(re.getAt(nonZeroIndex)),&(A_b.mpqMatrix[i][A_b.colNum - 1]));
    }
    return re;
}

MP_RAT&
MPQMatrix::getMPQAt(int r, int c)
{
    if(r >= this->rowNum || r < 0 || c >= this->colNum || c < 0)
    {
        std::cout<<"the row index or column index is out range of the Matrix's\n";
        exit(0);
    }
    return this->mpqMatrix[r][c];
}

void
MPQMatrix::rowSetpMatrixTrans()
{
    const int minOfRowAndCol = (this->rowNum <= this->colNum ? this->rowNum : this->colNum);
    for(int i = 0; i < minOfRowAndCol; ++i)
    {
        //对角线上的元素为0时，需要找到一行的第i个元素不为0，与当前行进行替换
        if(!mpq_cmp_si(&(this->mpqMatrix[i][i]),0,1))
        {
            bool allZero = true;
            for(int k = i+1; k < this->rowNum; ++k)
            {
                if(mpq_cmp_si(&(this->mpqMatrix[k][i]),0,1))
                {
                    allZero = false;
                    this->swapRR(i,k);//找到一行，与这一行进行交换
                    break;
                }
            }
            //如果从[i][i]向下开始整个列都为0元素的话，向上找一行满足：除了第i个元素不为零，前面的所有元素都为零；
            if(allZero)
            {
                bool findRow = false;
                for(int k = 0; k < i; ++k)
                {
                    //即第一个非零元的下标就为i
                    if(this->getFirstNonZeroIndexAtRow(k) == i)
                    {
                        this->swapRR(i,k);
                        findRow = true;
                        break;
                    }    
                }
                if(!findRow)
                    continue;
            }
        }
        //如果对角线上的元素不等于1
        if(mpq_cmp_si(&(this->mpqMatrix[i][i]),1,1))
        {
            MP_RAT temp;
            mpq_init(&temp);
            mpq_set(&temp,&(this->mpqMatrix[i][i]));
            for(int k = i; k < this->colNum; ++k)
            {
                mpq_div(&(this->mpqMatrix[i][k]),&(this->mpqMatrix[i][k]),&temp);
                mpq_canonicalize(&(this->mpqMatrix[i][k]));
            }
            mpq_clear(&temp);
        }
        //把[i][i]这个下标列的下面的所有元素变为0
        for(int k = i + 1; k < this->rowNum; ++k)
        {
            if(!mpq_cmp_si(&(this->mpqMatrix[k][i]),0,1))//如果这个元素为0，则进行下一行
                continue;
            else
            {
                MP_RAT temp;
                mpq_init(&temp);
                MP_RAT temp1;
                mpq_init(&temp1);
                mpq_set(&temp1,&(this->mpqMatrix[k][i]));
                for(int j = i; j < this->colNum; ++j)
                {
                    mpq_mul(&temp,&(this->mpqMatrix[i][j]),&temp1);
                    mpq_canonicalize(&temp);
                    mpq_sub(&(this->mpqMatrix[k][j]),&(this->mpqMatrix[k][j]),&temp);
                    mpq_canonicalize(&(this->mpqMatrix[k][j]));
                }
                mpq_clear(&temp);
                mpq_clear(&temp1);
            }
        }
        //把这个对角线上的元素都变为0
        for(int k = 0; k < i; ++k)
        {
            MP_RAT temp;
            mpq_init(&temp);
            MP_RAT temp1;
            mpq_init(&temp1);
            mpq_set(&temp1,&(this->mpqMatrix[k][i]));
            for(int j = i; j < this->colNum; ++j)
            {
                mpq_mul(&temp,&(this->mpqMatrix[i][j]),&temp1);
                mpq_canonicalize(&temp);
                mpq_sub(&(this->mpqMatrix[k][j]),&(this->mpqMatrix[k][j]),&temp);
                mpq_canonicalize(&(this->mpqMatrix[k][j]));
            }
            mpq_clear(&temp);
            mpq_clear(&temp1);
        }
    }
    mpq_t temp1;
    mpq_t temp2;
    mpq_init(temp1);
    mpq_init(temp2);
    for(int i = minOfRowAndCol - 1; i >=0; --i)
    {
        int nonZeroIndex = this->getFirstNonZeroIndexAtRow(i);
        if(nonZeroIndex == i)
            continue;
        else
        {
            for(int k = i - 1; k >= 0; --k)
            {
                mpq_set(temp1,&(this->mpqMatrix[k][nonZeroIndex]));
                for(int j = nonZeroIndex; j < this->colNum; ++j)
                {
                    mpq_mul(temp2,&(this->mpqMatrix[i][nonZeroIndex]),temp1);
                    mpq_canonicalize(temp2);
                    mpq_sub(&(this->mpqMatrix[k][j]),&(this->mpqMatrix[k][j]),temp2);
                    mpq_canonicalize(&(this->mpqMatrix[k][j]));
                }
            }
        }
        
    }
    mpq_clear(temp1);
    mpq_clear(temp2);
}

int
MPQMatrix::getFirstNonZeroIndexAtRow(int index)
{
    if(index >= this->rowNum || index < 0)
    {
        std::cout<<"this index is out range of the row number of matrix\n";
        exit(1);
    }
    for(int i = 0; i < this->colNum; ++i)
    {
        //i下标处的元素不为0，则返回该下标
       if(mpq_cmp_si(&(this->mpqMatrix[index][i]),0,1))
            return i;
    }
    return -1;//代表这一行全是0元素
}

void
MPQMatrix::swapRR(int r1,int r2)
{
    if(r1 == r2)
        return;
    if(r1 >= this->rowNum || r1 < 0 || r2 >= this->rowNum || r2 < 0)
    {
        std::cout<<"the index is out range of the row number of matrix\n";
        exit(1);
    }
    for(int i = 0; i < this->colNum; ++i)
        mpq_swap(&(this->mpqMatrix[r1][i]),&(this->mpqMatrix[r2][i]));
}

//根据index行的第一个非零元素，整行除以这个非零元
void 
MPQMatrix::setOneOfFirstElementAt(int index)
{
    if(index >= this->rowNum || index < 0)
    {
        std::cout<<"the index is out range of the row number of matrix\n";
        exit(1);
    }
    int firstNonZeroIndex = this->getFirstNonZeroIndexAtRow(index);
    if(firstNonZeroIndex == -1)
    {
        std::cout<<"Row "<< index + 1<<" is all zero\n";
        return;
    }
    MP_RAT temp;
    mpq_init(&temp);
    mpq_set(&temp,&(this->mpqMatrix[index][firstNonZeroIndex]));
    for(int i = firstNonZeroIndex; i < this->colNum; ++i)
    {
        mpq_div(&(this->mpqMatrix[index][i]),&(this->mpqMatrix[index][i]),&temp);
        mpq_canonicalize(&(this->mpqMatrix[index][i]));
    }
    mpq_clear(&temp);
}

void
MPQMatrix::rowISubRowJTimeTemp(int ri,int rj,MP_RAT& temp)
{
    if(ri >= this->rowNum || ri < 0 || rj >= this->rowNum || rj < 0)
    {
        std::cout<<"this index is out range of the row of matrix\n";
        exit(1);
    }
    MP_RAT mp;
    mpq_init(&mp);
    for(int i = 0; i < this->colNum; ++i)
    {
        mpq_mul(&mp,&(this->mpqMatrix[rj][i]),&temp);
        mpq_canonicalize(&mp);
        mpq_sub(&(this->mpqMatrix[ri][i]),&(this->mpqMatrix[ri][i]),&mp);
        mpq_canonicalize(&(this->mpqMatrix[ri][i]));
    }
    mpq_clear(&mp);
}

int
MPQMatrix::getRowNum()
{
    return this->rowNum;
}

int
MPQMatrix::getColNum()
{
    return this->colNum;
}

void
MPQMatrix::printMPQMatrix()
{
    for(int i = 0; i < this->rowNum; ++i)
    {
        for(int j = 0; j < this->colNum; ++j)
            gmp_printf("%Zd/%Zd\t",mpq_numref(&(this->mpqMatrix[i][j])),mpq_denref(&(this->mpqMatrix[i][j])));
            //gmp_printf("%+Qd\t",&(this->mpqMatrix[i][j]));
        std::cout<<'\n';
    }
    std::cout<<"-------------------------------------------------------------------------\n";
}

