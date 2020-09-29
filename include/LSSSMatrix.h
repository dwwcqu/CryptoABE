#ifndef LSSSMATRIX_H
#define LSSSMATRIX_H
#include<string>
#include<iostream>
#include<vector>
#include<stack>
class LSSSMatrix
{
private:
    std::vector<std::vector<int>> lsssmatrix{{1}};
    std::vector<std::string> attributeSet;
    int rowNum {1};
    int columnNum {1};
    int getAttIndex(std::string& att);

public:
    LSSSMatrix(const std::string& accesspolicy);
    LSSSMatrix(int threshold,int childNum);
    LSSSMatrix();
    ~LSSSMatrix();
    const int getRowNum();
    const int getColumnNum();
    std::vector<std::string>& getAttributesSet();
    void printLsssMatrix();
    std::vector<int> getRowAt(int index);
    std::vector<int> getColumnAt(int index);
    void insertNodeMatrixAt(std::vector<std::vector<int>> nodematrix,int index);
    void printAttributes();
    std::vector<std::vector<int>>& getLsssMatrix();
    //根据一个属性集合，获得其对应的LSSS子矩阵；
    std::vector<std::vector<int>> getSubMatrix(const std::vector<std::string>& attrs);
};

int pow(int base,int index);

std::vector<std::vector<int>> getNodeMatrix(int threshold,int childNum);

int findLastComma(std::string& str);

bool isAttribute(std::string& str);

int* parseStringPolicy(std::string accesspolicy,std::stack<std::string>  &policyStack);
#endif
