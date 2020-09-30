/*
 * author: dengweiwei
 * 2020-09-30
 */
#include"ElementInZrMatrix.h"

ElementMatrix::
        ElementMatrix(const std::vector<std::vector<int>>& lsssmatrix,pairing_t pairing)
{
    const int rowN = lsssmatrix.size();
    const int colN = lsssmatrix.at(0).size();
    this->elementMatrix = new element_s* [rowN];
    for(int i = 0; i < rowN; ++i)
    {
        this->elementMatrix[i] = new element_s[colN];
        for(int j = 0; j < colN; ++j)
        {
            element_init_Zr(&(this->elementMatrix[i][j]),pairing);
            element_set_si(&(this->elementMatrix[i][j]),lsssmatrix.at(i).at(j));
        }
    }
    this->rowNum = rowN;
    this->colNum = colN;
    //std::cout<<"ElementMatrix Construction\n";
}

ElementMatrix::ElementMatrix(MPQMatrix& empq,pairing_t pairing)
{
    this->rowNum = empq.getRowNum();
    this->colNum = empq.getColNum();
    this->elementMatrix = new element_s* [this->rowNum];
    element_t fenzi;
    element_t fenmu;
    element_init_Zr(fenzi,pairing);
    element_init_Zr(fenmu,pairing);
    mpq_t rat;
    mpq_init(rat);
    for(int i = 0; i < this->rowNum; ++i)
    {
        this->elementMatrix[i] = new element_s[this->colNum];
        for(int j = 0; j < this->colNum; ++j)
        {
            element_init_Zr(&(this->elementMatrix[i][j]),pairing);
            mpq_set(rat,&(empq.getMPQAt(i,j)));
            mpq_abs(rat,rat);//取绝对值
            element_set_mpz(fenzi,mpq_numref(rat));//分子从mpq_t类型转为Zr上的元素
            element_set_mpz(fenmu,mpq_denref(rat));//分母从mpq_t类型转为Zr上的元素
            element_div(fenzi,fenzi,fenmu);
            if(mpq_sgn(&(empq.getMPQAt(i,j))) < 0)//元素小于零
                element_neg(&(this->elementMatrix[i][j]),fenzi);// e = - fenzi/fenmu
            else//大于等于零
                element_set(&(this->elementMatrix[i][j]),fenzi);// e = fenzi / fenmu
        }
    }
    mpq_clear(rat);
    element_clear(fenzi);
    element_clear(fenmu);
}

ElementMatrix::ElementMatrix(){}

ElementMatrix::
        ~ElementMatrix()
{
    for(int i = 0; i < this->rowNum; ++i)
    {
        for(int j = 0; j < this->colNum; ++j)
        {
            element_clear(&(this->elementMatrix[i][j]));
        }
        delete[] this->elementMatrix[i];
    }
    delete[] elementMatrix;
    //std::cout<<"ElementMatrix Deconstrucion\n";
}

//复制构造函数
ElementMatrix::
ElementMatrix(const ElementMatrix& em)
{
    this->rowNum = em.rowNum;
    this->colNum = em.colNum;
    this->elementMatrix = new element_s* [em.rowNum];
    for(int i = 0; i < em.rowNum; ++i)
    {
        this->elementMatrix[i] = new element_s[em.colNum];
        for(int j = 0; j < em.colNum; ++j)
        {
            element_init_same_as(&(this->elementMatrix[i][j]),&(em.elementMatrix[i][j]));
            element_set(&(this->elementMatrix[i][j]),&(em.elementMatrix[i][j]));
        }
    }
}

ElementMatrix
ElementMatrix::trans()
{
    ElementMatrix trans;
    trans.rowNum = this->colNum;
    trans.colNum = this->rowNum;
    trans.elementMatrix = new element_s* [this->colNum];
    for(int i = 0; i < this->colNum; ++i)
    {
        trans.elementMatrix[i] = new element_s [this->rowNum];
        for(int j = 0; j < this->rowNum; ++j)
        {
            element_init_same_as(&(trans.elementMatrix[i][j]),&(this->elementMatrix[j][i]));
            element_set(&(trans.elementMatrix[i][j]),&(this->elementMatrix[j][i]));
        }
    }
    return trans;
}

ElementVector
ElementMatrix::solve(ElementVector& b)
{
    if(b.size() != this->rowNum)   
    {
        std::cout<<"this equation is not exit!\n";
        exit(1);
    }
    //方程的连接矩阵
    ElementMatrix A_b = this->addColumuAtLast(b);
    //对连接矩阵进行行阶梯变换
    A_b.rowSetpMatrixTrans();
    //获得行列数的最小值
    const int minOfRowAndCol = (A_b.rowNum <= A_b.colNum ? A_b.rowNum : A_b.colNum);
    ElementVector solution;
    solution.length = this->colNum;
    solution.elementVector = new element_s [this->colNum];
    for(int i = 0; i < this->colNum; ++i)
    {
        element_init_same_as(&(solution.elementVector[i]),&(this->elementMatrix[0][0]));
        element_set0(&(solution.elementVector[i]));
    }
    for(int i = minOfRowAndCol - 1; i >= 0; --i)
    {
        
        int nonZeroIndex = A_b.findFirstNonZeroIndexAt(i);
        if(nonZeroIndex == -1)
            continue;
        else if(nonZeroIndex == A_b.colNum - 1)
        {
            std::cout<<"this equation doesn't have solutions\n";
            exit(0);
        }
        else
            element_set(&(solution.elementVector[nonZeroIndex]),&(A_b.elementMatrix[i][A_b.colNum - 1]));
    }
    return solution;
}


void
ElementMatrix::rowSetpMatrixTrans()
{
    if(this->rowNum == 0 || this->colNum == 0)
        return;
    //获得行列长度的最小值
    const int  minOfRowAndCol = (this->rowNum <= this->colNum ? this->rowNum : this->colNum);
    element_t elementO;//0元素
    element_t elementI;//1元素
    element_init_same_as(elementO,&(this->elementMatrix[0][0]));
    element_init_same_as(elementI,&(this->elementMatrix[0][0]));
    element_set0(elementO);
    element_set1(elementI);
    element_t temp1;
    element_init_same_as(temp1,&(this->elementMatrix[0][0]));
    element_t temp2;
    element_init_same_as(temp2,&(this->elementMatrix[0][0]));
    for(int i = 0; i < minOfRowAndCol; ++i)
    {
        //如果第i行的对角线元素为0，需要往下找一行在第i个位置的元素不为零，并与这一行进行交换
        if(!element_cmp(&(this->elementMatrix[i][i]),elementO))
        {
            bool allZero = true;
            //往下面找一个行
            for(int k = i + 1; k < this->rowNum; ++k)
            {
                if(element_cmp(&(this->elementMatrix[k][i]),elementO))
                {
                    allZero = false;
                    this->swapRR(i,k);
                    break;
                }
            }
            if(allZero)
            {
                //找不到一行时，从第一行往下找
                bool findRow {false};
                for(int k = 0; k < i; ++k)
                {
                    if(this->findFirstNonZeroIndexAt(k) == i)
                    {
                        findRow = true;
                        this->swapRR(k,i);
                        break;
                    }
                }
                //依旧找不到,获得这一行的第一个非零元，并把这列下面的元素变为0
                if(!findRow)
                {
                    int thisNonZeroIndex = this->findFirstNonZeroIndexAt(i);
                    if(element_cmp(&(this->elementMatrix[i][thisNonZeroIndex]),elementI))
                    {
                        element_set(temp1,&(this->elementMatrix[i][thisNonZeroIndex]));
                        for(int k = thisNonZeroIndex; k < this->colNum; ++k)
                            element_div(&(this->elementMatrix[i][k]),&(this->elementMatrix[i][k]),temp1);
                    }
                    for(int k = i + 1; k < minOfRowAndCol; ++k)
                    {
                        element_set(temp1,&(this->elementMatrix[i][thisNonZeroIndex]));
                        for(int j = thisNonZeroIndex; j < this->colNum; ++j)
                        {
                            element_mul(temp2,&(this->elementMatrix[i][j]),temp1);
                            element_sub(&(this->elementMatrix[k][j]),&(this->elementMatrix[k][j]),temp2);
                        }
                    }
                    continue;
                }
            }
        }
        //如果对角线上的元素不为0且不为1，需要把这一行变成以1开始的行
        if(element_cmp(&(this->elementMatrix[i][i]),elementI))
        {
            element_set(temp1,&(this->elementMatrix[i][i]));
            for(int k = i; k < this->colNum; ++k)
                element_div(&(this->elementMatrix[i][k]),&(this->elementMatrix[i][k]),temp1);
        }
        //剩下的就是把这个对角元素下面的元素都变为0
        for(int k = i + 1; k < this->rowNum; ++k)
        {
            //如果这一行的第一个元素就是0，那么进行下一行
            if(!element_cmp(&(this->elementMatrix[k][i]),elementO))
                continue;
            element_set(temp1,&(this->elementMatrix[k][i]));
            for(int j = i; j < this->colNum; ++j)
            {
                element_mul(temp2,&(this->elementMatrix[i][j]),temp1);
                element_sub(&(this->elementMatrix[k][j]),&(this->elementMatrix[k][j]),temp2);
            }
        }
    }
    int nonZeroIndexArr[minOfRowAndCol];//每行的第一个非零元素的下标
    for(int i = 0; i < minOfRowAndCol; ++i)
        nonZeroIndexArr[i] = this->findFirstNonZeroIndexAt(i);
    //对矩阵进行行阶梯化，根据每行的第一个非零元素的下标进行行的变换
    for(int i = 0; i < minOfRowAndCol; ++i)
    {
        for(int k = i; k < minOfRowAndCol; ++k)
        {
            if(nonZeroIndexArr[i] > nonZeroIndexArr[k])
            {
                int temp = nonZeroIndexArr[i];
                nonZeroIndexArr[i] = nonZeroIndexArr[k];
                nonZeroIndexArr[k] = temp;
                this->swapRR(i,k);
            }
        }
    }
    //自下而上对非零元素上面的元素变为零
    for(int i = minOfRowAndCol - 1; i >= 0; --i)
    {
        //如果第一个非零元素不为1，需要把整行变成以1开始的行
        if(element_cmp(&(this->elementMatrix[i][nonZeroIndexArr[i]]),elementI))
        {
            element_set(temp1,&(this->elementMatrix[i][nonZeroIndexArr[i]]));
            for(int k = nonZeroIndexArr[i]; k < this->colNum; ++k)
                element_div(&(this->elementMatrix[i][k]),&(this->elementMatrix[i][k]),temp1); 
        }
        //剩下的就是把这个非零元列上面的元素都变为0
        
        for(int k = i - 1; k >= 0; --k)
        {
            element_set(temp1,&(this->elementMatrix[k][nonZeroIndexArr[i]]));
            for(int j = nonZeroIndexArr[i]; j < this->colNum; ++j)
            {
                element_mul(temp2,&(this->elementMatrix[i][j]),temp1);
                element_sub(&(this->elementMatrix[k][j]),&(this->elementMatrix[k][j]),temp2);
            }
        }
    }
    element_clear(temp1);
    element_clear(temp2);
}


void
ElementMatrix::swapRR(int r1,int r2) 
{
    if(r1 >= this->rowNum || r1 < 0 || r2 >= this->rowNum || r2 < 0)
    {
        std::cout<<"this row index is out range of the matrix in swapRR\n";
        exit(1);
    }
    element_t temp;
    element_init_same_as(temp,&(this->elementMatrix[0][0]));
    for(int i = 0; i < this->colNum; ++i)
    {
        element_set(temp,&(this->elementMatrix[r1][i]));
        element_set(&(this->elementMatrix[r1][i]),&(this->elementMatrix[r2][i]));
        element_set(&(this->elementMatrix[r2][i]),temp);
    }
    element_clear(temp);
}


int
ElementMatrix::findFirstNonZeroIndexAt(int index)  
{
    if(index >= this->rowNum || index < 0)
    {
        std::cout<<"this row index is out range of the matrix in findFirstNonZeroIndexAt!\n";
        exit(1);
    }
    element_t element0;
    element_init_same_as(element0,(&this->elementMatrix[index][0]));
    element_set0(element0);
    for(int i = 0; i < this->colNum; ++i)
    {
        if(element_cmp(&(this->elementMatrix[index][i]),element0))
            return i;
    }
    element_clear(element0);
    return -1;
}

ElementVector ElementMatrix::multiVector(ElementVector& ev)
{
    if(this->colNum != ev.size())
    {
        std::cout<<"The Matrix's column is not equal to Vector's\n";
        exit(1);
    }
    //乘法的返回结果
    ElementVector re;
    re.length = this->rowNum;
    re.elementVector = new element_s [this->rowNum];
    element_t mul;
    element_init_same_as(mul,&(this->elementMatrix[0][0]));
    for(int i = 0; i < this->rowNum; ++i)
    {
        element_init_same_as(&(re.elementVector[i]),&(this->elementMatrix[i][0]));
        element_set0(&(re.getElementAt(i)));
        for(int j = 0; j < this->colNum; ++j)
        {
            element_mul(mul
                           ,&(this->elementMatrix[i][j])
                           ,&(ev.getElementAt(j)));
            element_add(&(re.getElementAt(i)),&(re.getElementAt(i)),mul);
        }
    }
    element_clear(mul);
    return re;
}

ElementMatrix
ElementMatrix::addColumuAtLast(ElementVector& ev)
{
    if(ev.size() != this->rowNum)
    {
        std::cout<<"this vector's size is not equal to the row number of matrix!\n";
        exit(1);
    }
    ElementMatrix re;
    re.rowNum = this->rowNum;
    re.colNum = this->colNum + 1;
    re.elementMatrix = new element_s* [this->rowNum];
    for(int i = 0; i < this->rowNum; ++i)
    {
        re.elementMatrix[i] = new element_s [this->colNum + 1];
        for(int j = 0; j < this->colNum; ++j)
        {
            element_init_same_as(&(re.elementMatrix[i][j]),&(this->elementMatrix[i][j]));
            element_set(&(re.elementMatrix[i][j]),&(this->elementMatrix[i][j]));
        }
        element_init_same_as(&(re.elementMatrix[i][this->colNum]),&(ev.getElementAt(i)));
        element_set(&(re.elementMatrix[i][this->colNum]),&(ev.getElementAt(i)));
    }
    return re;
}



void ElementMatrix::
        printElementMatrix()
{
    for(int i = 0; i < this->rowNum; ++i)
    {
        for(int j = 0; j < this->colNum; ++j)
            element_printf("Element[%d][%d] = %B\t\t\t",i+1,j+1,&(this->elementMatrix[i][j]));
        std::cout<<'\n';
    }
    std::cout<<"------------------------------------------------------------------------------------------------------------\n";
}


int 
ElementMatrix::getRowNum()
{
    return this->rowNum;
}

int 
ElementMatrix::getColNum()
{
    return this->colNum;
}