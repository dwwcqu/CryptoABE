#ifndef ELEMENTINMPQMATRIX_H
#define ELEMENTINMPQMATRIX_H

#include"ElementInMPQVector.h"

//mpq元素矩阵
class MPQMatrix
{
private:
        MP_RAT** mpqMatrix;//多精度有理数矩阵
        int rowNum{0};
        int colNum{0};
public:
        MPQMatrix(std::vector<std::vector<int>>& lsssmatrix);
        MPQMatrix();
        MPQMatrix(const MPQMatrix& mpqm);
        ~MPQMatrix();
        void printMPQMatrix();
        MPQMatrix trans();
        MPQMatrix addOneColumnAtLast(MPQVector &mpqv);
        //解非线性方程组的一个特解，这里主要考虑b = (1,0,..,0)的形式
        MPQVector fsolve(MPQVector& b);
        //交换两行的元素
        void swapRR(int r1,int r2);
        int getRowNum();
        int getColNum();
        //返回第index行中的第一个非零元素的下标
        int getFirstNonZeroIndexAtRow(int index);
        void rowSetpMatrixTrans();
        void rowISubRowJTimeTemp(int ri,int rj,MP_RAT& temp);//ri = ri - rj * temp
        void setOneOfFirstElementAt(int index);
        MP_RAT& getMPQAt(int r, int c);
};

#endif