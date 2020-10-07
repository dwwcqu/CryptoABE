<script id="MathJax-script" async src="<https://github.com/dwwcqu/CryptoABE/mathjax/tex-chtml.js"></script>

# CryptoABE

这个仓库主要是用来实现一些 ***ABE(attribute-based encryption)*** 方案。 因为个人时间和能力问题，整个仓库只是实现一些 ***ABE*** 方案的小 demo,但是，也有一些特别的地方，如我们的访问策略树和 ***LSSS*** 秘密分享方案与 ***ABE*** 方案的结合。

## ***访问策略树 (Access Policy Threshold Tree)***

在我实现的 ***ABE*** 方案中，我使用了特殊形式的字符串来表示加密阶段使用的策略， 例如 *((A,B,C,2),(D,E,F,2),2)*。我们可以把这个访问策略看成是一颗树，整个字符串代表是我们的根节点，子串就代表了根节点的孩子节点，孩子节点就可以作为一个子树并作为这个新子树的根节点，一直递归到叶子节点，即到一个没有孩子节点的位置上。其中，我们用圆括号代表一个非叶子节点，在这一对括号中，里面包含的圆括号的个数代表这个节点的孩子节点的个数，而最后的数字代表了这个节点的阈值，没有用圆括号括起来的就是我们的属性值，也就是树的叶子节点。例如前面举的例子，用这个思路还原出整个访问策略树结构的步骤如下：

| ***((A,B,C,2),(D,E,F,2),2)*** | ![access tree 01](https://github.com/dwwcqu/CryptoABE/blob/master/images/accesstree01.png) |    步骤  1    |
| :---------------------------: | :----------------------------------------------------------: | :-----------: |
|  ***(A,B,C,2), (D,E,F,2)***   | **![access tree 02](https://github.com/dwwcqu/CryptoABE/blob/master/images/accesstree02.png)** |  **步骤  2**  |
|       ***A,B,C,D,E,F***       | **![access tree 03](https://github.com/dwwcqu/CryptoABE/blob/master/images/accesstree03.png)** | ***步骤  3*** |

在这个访问策略中，如果只有在根节点的两个子结点都满足的情况下，整个访问策略才是满足的。同时，要使根节点的两个子结点满足的话，那么左边一个节点至少要满足三个属性中的两个属性时才能保证这个节点满足策略，同样的道理对右边这个节点。因此，***“A,B,D,E”*** 就是一个满足访问策略的属性集合，而 ***“A,D,E,F“*** 就不满足访问策略。

> 访问策略树表示的思路来自于 Liu 等人在“Efficient Generation of Linear Secret Sharing Scheme Matrices from Threshold Access Trees“这篇文章中的工作。在这篇文章中，作者们实现了把访问策略变为一个秘密分享矩阵，进而使用这个秘密分享矩阵去获得每个属性对象所对应的秘密值。

具体关于访问策略树的代码实现，可以参考仓库中的 [*include/LSSSMatrix.h*](https://github.com/dwwcqu/CryptoABE/blob/master/include/LSSSMatrix.h) 和 [*source/LSSSMatrix.cpp*](https://github.com/dwwcqu/CryptoABE/blob/master/source/LSSSMatrix.cpp) 这两个文件。

## 线性秘密分享方案 *(LSSS)*

在 *Waters* 2011年的论文 *[Ciphertext-Policy Attribute-Based Encryption:An Expressive, Efficient, and Provably Secure Realization](https://dl.acm.org/doi/10.5555/1964658.1964664)* 中使用了 ***LSSS*** 秘密分享方案设计了一个可证明安全 ***CPABE*** 方案。截至到现在 *2020年9月30日*，因为没有查以前是否有工作完成了用 ***LSSS*** 去实现 ***CPABE*** 方案，如果，已经存在这样的工作，那么大家就当作是一种复习。

详细的关于 ***LSSS (Linear secret sharing scheme)*** 大家可以参卡 *Beimel* 的名为 [*Secure Schemes for Secret Sharing and Key Distribution*](https://www.cs.bgu.ac.il/~beimel/Papers/thesis.pdf) 的博士论文。我在这里主要解决的是：***CPABE*** 方案在加密阶段使用了一个用 ***LSSS*** 矩阵 *（从访问策略得来）*,来得到访问策略中每个属性所拥有的秘密值；而到了解密阶段时，在我看到的论文当中，所有的作者都是用一句"根据密文中 ***LSSS***矩阵和每个属性获得的秘密值，可以在多项式时间内还原出最开始分享的秘密值。"来解释解密过程。但是，关于这个秘密值的还原的方法，作者并没有给出来。我在这里就是要找出这个秘密值还原的方法，并用代码去实现。

首先，我介绍还原秘密值的方法。在介绍方法时，我先给出一些方法中使用到的符号表示：

| **符号**  |                 **解释**                 |
| :-------: | :--------------------------------------: |
| ***$M$*** |      $M$ ***代表我们的 LSSS 矩阵***      |
| ***$v$*** | ***$v$* 代表秘密分享向量，用列向量表示** |
| ***$m$*** |           ***$M$ 矩阵的行数***           |
| ***$n$*** |           ***$M$ 矩阵的列数***           |

例如，我们有一个秘密分享矩阵 ***$M$*** 为一个 *$m$\*$n$* 的矩阵：
$$
M=\left(\begin{matrix}a_{11}&a_{12}&\begin{matrix}\cdots&a_{1n}\\\end{matrix}\\\begin{matrix}a_{21}\\\vdots\\\end{matrix}&\begin{matrix}a_{22}\\\vdots\\\end{matrix}&\begin{matrix}\begin{matrix}\ldots\\\ddots\\\end{matrix}&\begin{matrix}a_{2n}\\\vdots\\\end{matrix}\\\end{matrix}\\a_{m1}&a_{m2}&\begin{matrix}\ldots&a_{mn}\\\end{matrix}\\\end{matrix}\right)
$$
和一个秘密分享向量 ***$v$*** 为一个 ***$n$*** 维的向量：
$$
v^T=\left(\begin{matrix}s&y_2&\begin{matrix}\ldots&y_n\\\end{matrix}\\\end{matrix}\right)，
$$
其中，*$s$* 代表我们需要分享的秘密值，而 $y_2,..., y_n $ 为 *$n - 1$* 个随机值。这样，我们就可以得到 *$m$* 个秘密值，分别属于 *$m$* 个属性。注意，在我实现的方案中，我只考虑属性不重复的情况。即，我们也可以得到一个秘密值向量 **$\lambda$**:
$$
\lambda^T=\left(\begin{matrix}\lambda_1&\lambda_2&\begin{matrix}\cdots&\lambda_m\\\end{matrix}\\\end{matrix}\right)
$$
即，我们的秘密分享阶段的过程满足以下公式：
$$
\\{{M}}*{{v}}\ =\ \left[\begin{matrix}a_{11}&a_{12}&\begin{matrix}\ldots&a_{1n}\\\end{matrix}\\a_{21}&a_{22}&\begin{matrix}\cdots&a_{2n}\\\end{matrix}\\\begin{matrix}\vdots\\a_{m1}\\\end{matrix}&\begin{matrix}\vdots\\a_{m2}\\\end{matrix}&\begin{matrix}\begin{matrix}\ddots\\\ldots\\\end{matrix}&\begin{matrix}\vdots\\a_{mn}\\\end{matrix}\\\end{matrix}\\\end{matrix}\right]\ast\left[\begin{matrix}s\\y_2\\\begin{matrix}\vdots\\y_n\\\end{matrix}\\\end{matrix}\right]=\left[\begin{matrix}\lambda_1\\\lambda_2\\\begin{matrix}\vdots\\\lambda_m\\\end{matrix}\\\end{matrix}\right]=\mathbf{\lambda}
$$
在只知道秘密值向量$\lambda$ 和秘密分享矩阵 $M$ 的情况下，为了获得一开始的秘密值 $s$ 。我的思路从 *公式4* 出发，如果我们有一个特殊的向量 $u^T=\left(\begin{matrix}1&0&\begin{matrix}\ldots&0\\\end{matrix}\\\end{matrix}\right)$,可以通过求得 $\lambda$ 向量和 $u$ 向量的内积即可得到我们的秘密值 $s$. 即，有
$$
\left(\lambda,\ u\right)=s
$$
于是，我们对*公式4*两边取转置，可以获得：
$$
v^TM^T=\lambda^T
$$
如果我们能对*公式6*两边乘上一个向量 $X$ 就可以得到秘密值 $s$ 的话，那么这个向量 $X$ 一定满足以下方程：
$$
M^T*X\ =\ u
$$
那么，现在我们的问题就变为求解一个非齐次线性方程组问题，只要我们能找到这么一个解 $X$ 的话，那么我们就可以得到秘密值 $s$ 。因为线性秘密分享矩阵为一种样式比较特殊的矩阵，其中，因为其行数大于等于列数，对其转置矩阵而言就是，行数小于列数，故而这个线性方程组一定是有解的，且至少有一个解。那么我们就可以用 $X$ 这个解与获得秘密分享值向量 $\lambda$ 进行内积，就可以得到最开始的秘密值 $s$，即：
$$
\left(\lambda,\ X\right)=s
$$
到目前为止，关于获得秘密值 $s$ 的思路已经解决。但是，如何通过代码来实现，这是在方案实现部分面临的问题。

具体的方案实现部分，读者可以参考 [*include/ElementInZrMatrix.h*](https://github.com/dwwcqu/CryptoABE/tree/master/include/ElementInZrMatrix.h), *[include/ElementInZrVector.h](https://github.com/dwwcqu/CryptoABE/tree/master/include/ElementInZrVector.h)*, *[source/ElementInZrMatrix.cpp](https://github.com/dwwcqu/CryptoABE/tree/master/source/ElementInZrMatrix.cpp)* 和 [*source/ElementInZrVector.cpp*](https://github.com/dwwcqu/CryptoABE/tree/master/source/ElementInZrVector.cpp)这几个源码进行上面思路的具体实现。

## Brent Waters CPABE Scheme

### 预备知识 Preliminary



# CryptoABE

This repository is some implementation of ***ABE (attribute-based encryption)*** scheme. Thanks to personal abilities, this repository is just a few ABE demos. However, these demos will give some ideas about cryptography to you. For example, in our implementations, I use a special *string* to express the access policy, such as *((A,B,C,2),(D,E,F,2),2)*. In each access policy, we could regard the string as a tree and substrings are child tree. The whole string is located in the root node.

## Waters Brent's CPABE scheme 
This scheme's implementation is located in __inlude/Waters11Scheme.h__ and __source/Waters11Scheme.cpp__.

### 1. Setup Phase

The ***setup*** algorithm is used to initialize the full parameters of ***CPABE*** scheme. Thanks to ***ABE*** scheme is based on ***bilinear pairing***, at first step, we need to initialize the parameters of ***bilinear pairing***. In order to fulfill this purpose, we need the [PBC library](https://crypto.stanford.edu/pbc/) implemented by *Ben Lynn* in Stanford University. 