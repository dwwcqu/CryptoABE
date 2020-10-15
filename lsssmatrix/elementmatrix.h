#ifndef ELEMENTINZRMATRIX_H
#define ELEMENTINZRMATRIX_H
#include<vector>
#include"elementvector.h"
#include"lsssmatrix.h"
#include"curve_param.h"
//在环Zr上的元素矩阵
//元素矩阵
class ElementMatrix
{

private:
        element_s** elementMatrix;
        int rowNum{0};
        int colNum{0};
public:
        ElementMatrix(const std::string& accesspolicy, pairing_t pairing);
        ElementMatrix();
        ElementMatrix(const ElementMatrix& em);
        ElementMatrix trans();//对矩阵进行转置
        ElementMatrix addColumuAtLast(ElementVector& ev);//在矩阵的最后一列添加一列
        void rowSetpMatrixTrans();
        void printElementMatrix()const;
        int findFirstNonZeroIndexAt(int index);//找到第index行的第一个非零元素的下标，如果都为零，返回-1
        void swapRR(int r1,int r2);
        ~ElementMatrix();
        int getRowNum();
        int getColNum();
        ElementVector multiVector(ElementVector& ev);
        ElementVector solve(ElementVector& b);//解一个线性方程组，返回一个特殊解
};


#endif