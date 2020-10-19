/*
 * author: dengweiwei
 * 2020-09-30
 */
#include"elementmatrix.h"

ElementMatrix::
        ElementMatrix(const std::string& accesspolicy,pairing_t& pairing)
{
    LSSSMatrix lsssmatrix{accesspolicy};
    rowNum = lsssmatrix.getRowNum();
    colNum = lsssmatrix.getColumnNum();
    elementMatrix = new element_s* [rowNum];
    for(int i = 0; i < rowNum; ++i)
    {
        rho[i] = lsssmatrix.attributeSet[i];
        elementMatrix[i] = new element_s[colNum];
        for(int j = 0; j < colNum; ++j)
        {
            element_init_Zr(&(elementMatrix[i][j]),pairing);
            element_set_si(&(elementMatrix[i][j]),lsssmatrix.getAtIJ(i,j));
        }
    }
}

ElementMatrix::ElementMatrix(){}

ElementMatrix::
        ~ElementMatrix()
{
    for(int i = 0; i < rowNum; ++i)
    {
        for(int j = 0; j < colNum; ++j)
        {
            element_clear(&(elementMatrix[i][j]));
        }
        delete[] elementMatrix[i];
    }
    delete[] elementMatrix;
}

//复制构造函数
ElementMatrix::
ElementMatrix(const ElementMatrix& em)
{
    rowNum = em.rowNum;
    colNum = em.colNum;
    rho = em.rho;
    elementMatrix = new element_s* [em.rowNum];
    for(int i = 0; i < em.rowNum; ++i)
    {
        elementMatrix[i] = new element_s[em.colNum];
        for(int j = 0; j < em.colNum; ++j)
        {
            element_init_same_as(&(elementMatrix[i][j]),&(em.elementMatrix[i][j]));
            element_set(&(elementMatrix[i][j]),&(em.elementMatrix[i][j]));
        }
    }
}

ElementMatrix
ElementMatrix::trans()
{
    ElementMatrix trans;
    trans.rowNum = colNum;
    trans.colNum = rowNum;
    trans.elementMatrix = new element_s* [colNum];
    for(int i = 0; i < colNum; ++i)
    {
        trans.elementMatrix[i] = new element_s [rowNum];
        for(int j = 0; j < rowNum; ++j)
        {
            element_init_same_as(&(trans.elementMatrix[i][j]),&(elementMatrix[j][i]));
            element_set(&(trans.elementMatrix[i][j]),&(elementMatrix[j][i]));
        }
    }
    return trans;
}

ElementVector
ElementMatrix::solve(ElementVector& b)
{
    assert(b.size() == rowNum && "ElementMatrix::solve(ElementVector& b):vector's length is not equal to the column of matrix");
    //方程的连接矩阵
    ElementMatrix A_b = addColumuAtLast(b);
    //对连接矩阵进行行阶梯变换
    A_b.rowSetpMatrixTrans();
    //获得行列数的最小值
    const int minOfRowAndCol = (A_b.rowNum <= A_b.colNum ? A_b.rowNum : A_b.colNum);
    ElementVector solution;
    solution.length = colNum;
    solution.elementVector = new element_s[colNum];
    for(int i = 0; i < colNum; ++i)
    {
        element_init_same_as(&(solution.elementVector[i]),&(elementMatrix[0][0]));
        element_set0(&(solution.elementVector[i]));
    }
    for(int i = minOfRowAndCol - 1; i >= 0; --i)
    {
        int nonZeroIndex = A_b.findFirstNonZeroIndexAt(i);
        assert(nonZeroIndex != (A_b.colNum - 1) && "ElementMatrix::solve(ElementVector& b):this equation doesn't have solution");
        if(nonZeroIndex == -1)
            continue;
        else
            element_set(&(solution.elementVector[nonZeroIndex]),&(A_b.elementMatrix[i][A_b.colNum - 1]));
    }
    return solution;
}

std::string&
ElementMatrix::rowMapToAtts(int index)
{
    assert(index >= 0 && index < rowNum && "ElementMatrix::rowMapToAtts(int index):index is out range of matrix's row");
    return rho[index];
}

ElementMatrix
ElementMatrix::getSubMatrix(const std::vector<std::string>& useratts)
{
    std::vector<int> id;
    //找到用户属性集合中对应矩阵的每一行的行下标
    for(auto att:useratts)
    {
        int index{-1};
        for(std::map<int,std::string>::iterator ite = rho.begin(); ite != rho.end(); ite++)
        {
            if(ite->second == att)
                index = ite->first;
        }
        if(index == -1)
            continue;
        else
            id.push_back(index);
    }
    ElementMatrix re;
    int subRowNum = id.size();
    re.rowNum = subRowNum;
    re.colNum = colNum;
    re.elementMatrix = new element_s*[subRowNum];
    for(int i = 0; i < subRowNum; ++i)
    {
        re.rho[i] = useratts[i];
        re.elementMatrix[i] = new element_s[colNum];
        for(int j = 0; j < colNum; ++j)
        {
            element_init_same_as(&(re.elementMatrix[i][j]),&(elementMatrix[id[i]][j]));
            element_set(&(re.elementMatrix[i][j]),&(elementMatrix[id[i]][j]));
        }
    }
    return re;
}

void
ElementMatrix::rowSetpMatrixTrans()
{
    assert(rowNum != 0 && colNum != 0 && "ElementMatrix::rowSetpMatrixTrans():the number of row and column must bigger than 0");
    //获得行列长度的最小值
    const int minOfRowAndCol = (rowNum <= colNum ? rowNum : colNum);
    element_t elementO;//0元素
    element_t elementI;//1元素
    element_init_same_as(elementO,&(elementMatrix[0][0]));
    element_init_same_as(elementI,&(elementMatrix[0][0]));
    element_set0(elementO);
    element_set1(elementI);
    element_t temp1;
    element_init_same_as(temp1,&(elementMatrix[0][0]));
    element_t temp2;
    element_init_same_as(temp2,&(elementMatrix[0][0]));
    for(int i = 0; i < minOfRowAndCol; ++i)
    {
        //如果第i行的对角线元素为0，需要往下找一行在第i个位置的元素不为零，并与这一行进行交换
        if(!element_cmp(&(elementMatrix[i][i]),elementO))
        {
            bool allZero = true;
            //往下面找一个行
            for(int k = i + 1; k < rowNum; ++k)
            {
                if(element_cmp(&(elementMatrix[k][i]),elementO))
                {
                    allZero = false;
                    swapRR(i,k);
                    break;
                }
            }
            if(allZero)
            {
                //找不到一行时，从第一行往下找
                bool findRow {false};
                for(int k = 0; k < i; ++k)
                {
                    if(findFirstNonZeroIndexAt(k) == i)
                    {
                        findRow = true;
                        swapRR(k,i);
                        break;
                    }
                }
                //依旧找不到,获得这一行的第一个非零元，并把这列下面的元素变为0
                if(!findRow)
                {
                    int thisNonZeroIndex = findFirstNonZeroIndexAt(i);
                    if(element_cmp(&(elementMatrix[i][thisNonZeroIndex]),elementI))
                    {
                        element_set(temp1,&(elementMatrix[i][thisNonZeroIndex]));
                        for(int k = thisNonZeroIndex; k < colNum; ++k)
                            element_div(&(elementMatrix[i][k]),&(elementMatrix[i][k]),temp1);
                    }
                    for(int k = i + 1; k < rowNum; ++k)
                    {
                        element_set(temp1,&(elementMatrix[k][thisNonZeroIndex]));
                        for(int j = thisNonZeroIndex; j < colNum; ++j)
                        {
                            element_mul(temp2,&(elementMatrix[i][j]),temp1);
                            element_sub(&(elementMatrix[k][j]),&(elementMatrix[k][j]),temp2);
                        }
                    }
                    continue;
                }
            }
        }
        //如果对角线上的元素不为0且不为1，需要把这一行变成以1开始的行
        if(element_cmp(&(elementMatrix[i][i]),elementI))
        {
            element_set(temp1,&(elementMatrix[i][i]));
            for(int k = i; k < colNum; ++k)
                element_div(&(elementMatrix[i][k]),&(elementMatrix[i][k]),temp1);
        }
        //剩下的就是把这个对角元素列下面的元素都变为0
        for(int k = i + 1; k < rowNum; ++k)
        {
            //如果这一行的第一个元素就是0，那么进行下一行
            if(!element_cmp(&(elementMatrix[k][i]),elementO))
                continue;
            element_set(temp1,&(elementMatrix[k][i]));
            for(int j = i; j < colNum; ++j)
            {
                element_mul(temp2,&(elementMatrix[i][j]),temp1);
                element_sub(&(elementMatrix[k][j]),&(elementMatrix[k][j]),temp2);
            }
        }
    }
    int nonZeroIndexArr[minOfRowAndCol];//每行的第一个非零元素的下标
    for(int i = 0; i < minOfRowAndCol; ++i)
        nonZeroIndexArr[i] = findFirstNonZeroIndexAt(i);
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
                swapRR(i,k);
            }
        }
    }
    //自下而上对非零元素上面的元素变为零
    for(int i = minOfRowAndCol - 1; i >= 0; --i)
    {
        //如果第一个非零元素不为1，需要把整行变成以1开始的行
        if(element_cmp(&(elementMatrix[i][nonZeroIndexArr[i]]),elementI))
        {
            element_set(temp1,&(elementMatrix[i][nonZeroIndexArr[i]]));
            for(int k = nonZeroIndexArr[i]; k < colNum; ++k)
                element_div(&(elementMatrix[i][k]),&(elementMatrix[i][k]),temp1); 
        }
        //剩下的就是把这个非零元列上面的元素都变为0
        
        for(int k = i - 1; k >= 0; --k)
        {
            element_set(temp1,&(elementMatrix[k][nonZeroIndexArr[i]]));
            for(int j = nonZeroIndexArr[i]; j < colNum; ++j)
            {
                element_mul(temp2,&(elementMatrix[i][j]),temp1);
                element_sub(&(elementMatrix[k][j]),&(elementMatrix[k][j]),temp2);
            }
        }
    }
    element_clear(temp1);
    element_clear(temp2);
}


void
ElementMatrix::swapRR(int r1,int r2) 
{
    assert(r1 >=0 && r1 < rowNum && r2 >= 0 && r2 < rowNum && "ElementMatrix::swapRR(int r1,int r2):the row index is out range of the matrix");
    element_t temp;
    element_init_same_as(temp,&(elementMatrix[0][0]));
    for(int i = 0; i < colNum; ++i)
    {
        element_set(temp,&(elementMatrix[r1][i]));
        element_set(&(elementMatrix[r1][i]),&(elementMatrix[r2][i]));
        element_set(&(elementMatrix[r2][i]),temp);
    }
    element_clear(temp);
}


int
ElementMatrix::findFirstNonZeroIndexAt(int index)  
{
    assert(index >= 0 && index < rowNum && "ElementMatrix::findFirstNonZeroIndexAt(int index):the row index is out range of the matrix");
    element_t element0;
    element_init_same_as(element0,(&elementMatrix[index][0]));
    element_set0(element0);
    for(int i = 0; i < colNum; ++i)
    {
        if(element_cmp(&(elementMatrix[index][i]),element0))
            return i;
    }
    element_clear(element0);
    return -1;
}

ElementVector ElementMatrix::multiVector(ElementVector& ev)
{
    assert(colNum == ev.size() && "ElementMatrix::multiVector(ElementVector& ev):The Matrix's column is not equal to Vector's");
    //乘法的返回结果
    ElementVector re;
    re.length = rowNum;
    re.elementVector = new element_s [rowNum];
    element_t mul;
    element_init_same_as(mul,&(elementMatrix[0][0]));
    for(int i = 0; i < rowNum; ++i)
    {
        element_init_same_as(&(re.elementVector[i]),&(elementMatrix[i][0]));
        element_set0(&(re.getElementAt(i)));
        for(int j = 0; j < colNum; ++j)
        {
            element_mul(mul
                           ,&(elementMatrix[i][j])
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
    assert(rowNum == ev.size() && "ElementMatrix::addColumuAtLast(ElementVector& ev):this vector's size is not equal to the row number of matrix");
    ElementMatrix re;
    re.rowNum = rowNum;
    re.colNum = colNum + 1;
    re.elementMatrix = new element_s* [rowNum];
    for(int i = 0; i < rowNum; ++i)
    {
        re.elementMatrix[i] = new element_s [colNum + 1];
        for(int j = 0; j < colNum; ++j)
        {
            element_init_same_as(&(re.elementMatrix[i][j]),&(elementMatrix[i][j]));
            element_set(&(re.elementMatrix[i][j]),&(elementMatrix[i][j]));
        }
        element_init_same_as(&(re.elementMatrix[i][colNum]),&(ev.getElementAt(i)));
        element_set(&(re.elementMatrix[i][colNum]),&(ev.getElementAt(i)));
    }
    return re;
}



void ElementMatrix::
        printElementMatrix()
{
    for(int i = 0; i < rowNum; ++i)
    {
        std::cout<<rho[i]<<" : ";
        for(int j = 0; j < colNum; ++j)
            element_printf("Element[%d][%d] = %B\t\t\t",i+1,j+1,&(elementMatrix[i][j]));
        std::cout<<'\n';
    }
    std::cout<<"------------------------------------------------------------------------------------------------------------\n";
}


int 
ElementMatrix::getRowNum() 
{
    return rowNum;
}

int 
ElementMatrix::getColNum()
{
    return colNum;
}

