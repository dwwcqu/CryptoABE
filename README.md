# ***CryptoABE***

这个仓库主要是用来实现一些 ***ABE(attribute-based encryption)*** 方案。 因为个人时间和能力问题，整个仓库只是实现一些 ***ABE*** 方案的小 demo,但是，也有一些特别的地方，如我们的访问策略树和 ***LSSS*** 秘密分享方案与 ***ABE*** 方案的结合。

## ***访问策略树 (Access Policy Threshold Tree)***

在我实现的 ***ABE*** 方案中，我使用了特殊形式的字符串来表示加密阶段使用的策略， 例如 *((A,B,C,2),(D,E,F,2),2)*。我们可以把这个访问策略看成是一颗树，整个字符串代表是我们的根节点，子串就代表了根节点的孩子节点，孩子节点就可以作为一个子树并作为这个新子树的根节点，一直递归到叶子节点，即到一个没有孩子节点的位置上。其中，我们用圆括号代表一个非叶子节点，在这一对括号中，里面包含的圆括号的个数代表这个节点的孩子节点的个数，而最后的数字代表了这个节点的阈值，没有用圆括号括起来的就是我们的属性值，也就是树的叶子节点。例如前面举的例子，用这个思路还原出整个访问策略树结构的步骤如下：

| ***((A,B,C,2),(D,E,F,2),2)*** | ![access tree 01](https://github.com/dwwcqu/CryptoABE/blob/master/images/accesstree01.png) |    步骤 1    |
| :---------------------------: | :----------------------------------------------------------: | :----------: |
|  ***(A,B,C,2), (D,E,F,2)***   | **![access tree 02](https://github.com/dwwcqu/CryptoABE/blob/master/images/accesstree02.png)** |  **步骤 2**  |
|       ***A,B,C,D,E,F***       | **![access tree 03](https://github.com/dwwcqu/CryptoABE/blob/master/images/accesstree03.png)** | ***步骤 3*** |

在这个访问策略中，如果只有在根节点的两个子结点都满足的情况下，整个访问策略才是满足的。同时，要使根节点的两个子结点满足的话，那么左边一个节点至少要满足三个属性中的两个属性时才能保证这个节点满足策略，同样的道理对右边这个节点。因此，***“A,B,D,E”*** 就是一个满足访问策略的属性集合，而 ***“A,D,E,F“*** 就不满足访问策略。

> 访问策略树表示的思路来自于 Liu 等人在“Efficient Generation of Linear Secret Sharing Scheme Matrices from Threshold Access Trees“这篇文章中的工作。在这篇文章中，作者们实现了把访问策略变为一个秘密分享矩阵，进而使用这个秘密分享矩阵去获得每个属性对象所对应的秘密值。

具体关于访问策略树的代码实现，可以参考仓库中的 [*include/LSSSMatrix.h*](https://github.com/dwwcqu/CryptoABE/blob/master/include/LSSSMatrix.h) 和 [*source/LSSSMatrix.cpp*](https://github.com/dwwcqu/CryptoABE/blob/master/source/LSSSMatrix.cpp) 这两个文件。

## **线性秘密分享方案** *(LSSS)*

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

## **Brent Waters CPABE Scheme**

### **预备知识 *Preliminary***

#### **库/包依赖**

在实现 ***ABE*** 加密算法过程时，您所需要的依赖库有 [*gmp*](https://gmplib.org) 和由斯坦福大学的 *Ben Lynn* 等人实现的 [*PBC Library*](https://crypto.stanford.edu/pbc/)。具体如何安装请参考这两个依赖包的安装说明文档，需要注意的是，在我实现的方案中，我是用 *C++* 语言取实现的 ***ABE*** 方案。因此，我安装的是 *PBC* 的 *C* 语言安装，如果你使用 *Java* 语言进行实现的话，我建议你使用 [*JPBC*](http://gas.dia.unisa.it/projects/jpbc/) 这个 *Java* 库。

#### **双线性映射 （*Bilinear Maps*)**

假设 $\mathbb{G}$ 和 $\mathbb{G_T}$ 为两个阶为 $p$ 的乘法循环群，设 $g$ 为群 $\mathbb{G}$ 的生成元，而 $e$ 为一个双线性映射满足，$e : \mathbb{G} \times \mathbb{G} \rightarrow \mathbb{G_T}$。而且这个双线性映射满足以下性质：

1. **双线性**： 对任意的元素 $u,v \in \mathbb{G}$ 和任意的元素 $a,b \in \mathbb{Z_p}$, 我们有 $e(u^a, v^b) = e(u,v)^ {ab}$ 成立。
2. **非简化性**： $e(g,g) \not= 1$
3. 这个双线性映射要是很容易计算，并且这个双线性映射满足对称性。

#### 具体方案 (*Scheme*)

##### 系统初始化阶段 (***Setup***)

1. 首先，需要生成整个系统所需要的使用的属性，包括属性的数量 $U$ 以及属性值；
2. 根据选定的安全参数，生成整个系统需要使用的配对代数结构及其参数；生成阶为 $p$ 的群 $\mathbb{G}$，确定一个群 $\mathbb{G}$ 上的一个生成元 $g$。同时，为系统的 $U$ 个属性分别分配一个 $\mathbb{G}$ 上的元素：$ h_1, h_2, ..., h_{|U|} \in \mathbb{G}$。
3. 选取两个随机元素：$\alpha, a \in \mathbb{Z_p}$。

根据上面的过程，*Waters* 的 *CPABE* 方案中的公钥为 $PK = g, e(g,g)^\alpha, g^a, h_1, ..., h_U$；而系统的主密钥为 $MSK = g^\alpha$。

##### **加密阶段 (*Encryption*)**

1. 在加密阶段，需要使用到系统初始化阶段生成的公共参数 $PK$，同时，需要一个用被加密的消息 $M \in \mathbb{G_T}$；

2. ***CPABE*** 加密方案的访问策略保存在密文中，因此，需要让加密方根据自己的需求去设定一个访问策略（在我的方案中，访问策略的形式在前面的内容已经给出）；因为 *Waters* 的这个 ***CPABE*** 方案是用 *LSSS 矩阵* 来实现的，因此，如何把一个访问策略变成一个 *LSSS 矩阵* 的方案也在前面给出，这里我们就直接使用这个为 $m*n$ 的矩阵。其中 $m$ 为访问策略中的属性数量（注意：在这个方案中，我们要求访问策略中的属性不能重复）；$n$ 与发送方设定的访问策略有关系。同时，我们还需要一个映射函数 $\rho$ ，它的功能就是把 *LSSS 矩阵* 的每一行与访问策略属性集中的某个属性进行关联。

3. 为了让访问策略中的每个属性获得一个秘密分享值，根据我前面提到的 *LSSS* 的思想，我们需要个随机矩阵 $ v = (s, y_2, ..., y_n) \in \mathbb{Z_p^n}$；其中，$s$ 就是我们需要被进行分享的秘密值，其他 $n-1$ 个为随机值；因此，我们可以给这个 $m$ 个属性分别获得对应的秘密值：$ i = 1 ... m$, 我们有 $\lambda_i = v * M_i$，其中 $M_i$ 为访问矩阵 $M$ 的第 $i$ 行。同时，加密方需要生成 $m$ 个随机元素：$r_1, ... , r_m \in \mathbb{Z_p}$。

4. 经过上面三个阶段以后，我们就可以计算密文：
   $$
   C = \mathcal{M}*e(g,g)^{\alpha s}
   $$

   $$
   C^{\prime} = g^s
   $$

   $$
   (C_1 = g^{a\lambda_1}h_{\rho(1)}^{-r_1}, D_1 = g^{r_1}),..., (C_m = g^{a\lambda_m}h_{\rho(m)}^{-r_m}, D_m = g^{r_m})
   $$

经过上面四个阶段，我们就可以得到加密阶段的密文：$C, C^\prime, (C_1,D_1),...,(C_m,D_m)$，同时，还有加密阶段使用的线性秘密分享矩阵 $M$ 和映射函数 $\rho$。

##### **用户密钥生成阶段 (*KeyGen*)**

这个阶段主要完成，根据用户具有的属性集合（这个集合为系统所有属性集的子集），生成其对应的用户私钥，解密使用。

1. 首先，需要系统的主密钥 $MSK$ 和用户的属性集合 $S$ ；

2. 生成每个用户的密钥时，随机生成一个值 $t \in \mathbb{Z_p}$，同时，计算其对应的密钥：
   $$
   K = g^\alpha g^{at}
   $$

   $$
   L = g^t
   $$

   $$
   \forall x\in S, K_x= h_x^t
   $$

因此，每个用户对应的私钥为：$K,L, \forall x \in S: K_x$。

##### 解密阶段 (***Dcryption***)

当有用户想要去访问发送方加密的明文时，接收方就需要对加密的密文进行解密，密文中含有的内容有：$C, C^\prime, (C_1,D_1),...,(C_m,D_m)$和秘密分享矩阵矩阵$M$ 和映射函数 $\rho$，具体解密过程如下：

1. 为了还原出明文 $\mathcal{M}$，我们需要用密文中的访问策略和接收方用户的属性集合得到秘密值 $s$；

2. 因为，在 *Waters* 的 ***CPABE*** 方案中，他仅仅是给出了可以在多项式时间内找到一个这个解向量 $\{\omega_i \in \mathbb{Z_p}\}_{i \in I}$，使得 $\sum_{i\in I}{\omega_i}*{\lambda_i} = s$成立，其中 $I=\{i:\rho(i) \in S\}$，$S$ 为用户的属性集合；但是 *Waters* 而没有给出具体的求解方案，因此在前面的内容中，我给出了具体的求解思路，在这里我们就直接使用这些根据用户的属性集合求解出来的解用于解密；
   $$
   e(C^\prime,K)/(\prod_{i\in I}(e(C_i,L)*e(D_i,K_{\rho(i)}))^{\omega_i})=
   e(g,g)^{\alpha s}*e(g,g)^{ast}/(\prod_{i \in I}e(g,g)^{at\lambda_i\omega_i})=e(g,g)^{\alpha s}
   $$

   $$
   C/e(g,g)^{\alpha s} = \mathcal{M}
   $$

只要用户的属性集合满足密文中的访问策略，那么使用我们求解方法，一定就可以得到对应属性集合上的解，进而完成对密文的解密。如果，接收方的属性集合不满足访问策略的话，那么其接受到密文 $C, C^\prime, (C_1,D_1),...,(C_m,D_m)$ 以及秘密分享矩阵矩阵 $M$ 和映射函数 $\rho$，再加上自己的私钥 $K,L, \forall x \in S: K_x$ 这些信息，也不能求解出秘密值（或者说这个可能性很低，这由一定的假设保证，在 *Waters* 方案中使用的就是一个叫做 ***Decisional Parallel Bilinear Diffie-Hellman Exponent Assumption***，简单理解就是：你能破解这个系统的话，等价于解决一个数学上的难题）。

## **思考**

目前，我所熟知的 ***IBE (Identity-based Encryption)*** 和 ***ABE (Attributes-based Encryption)***  加密方案都可以算作是加密访问控制的方案。但是， ***IBE*** 与 ***ABE*** 又存在不同的地方。***IBE*** 可以有传统公钥加密方案的特性，即它有公钥（即用户的身份为其公钥），而私钥是与公钥满足某种运算关系，如下图就是一个 ***IBE*** 加密方案的大致过程：

![IBE 方案](https://github.com/dwwcqu/CryptoABE/blob/master/images/ibe.png)

1. 如果 *Alice* 想要给 *Bob*发送消息，那么 *Alice* 可以使用 *Bob* 的公钥（$i$，身份即是用户的公钥）（例如：*Bob* 的邮箱 *IP*地址 *MAC*地址 身份证号等）对消息进行加密；
2. 消息通过一个不可信的通道发送给了 *Bob*；
3. *Bob* 用户可以根据自己的身份（$i$），通过一个 *Key Generation* 算法获得其对应的私钥钥（$kd$)；
4. 在 *Bob* 获得其私钥和密文以后，就可以根据自己的私钥对消息进行解密；
5. 同样，*Bob* 也可以给 *Alice* 发送消息，只是在加密时，用的是 *Alice* 的身份作为其公钥，*Alice* 的解密过程与上面一致，也需要获得自己身份对应的私钥。

### ***Dan and Franklin's IBE scheme in 2001***

在这里为了更加具体，我给出一个关于 ***IBE*** 加密方案的具体实例，这是 *Dan Boneh* 等人在2001年首次提出的全安全的 ***IBE*** 方案，论文的名称为 [*Identity-based encryption from the weil pairing*](https://dl.acm.org/doi/10.5555/646766.704155)。

首先，*Dan* 的等人的这个方案是建立在 ***BDH (Bilinear Diffie-Hellman)*** 假设上的一个方案，在这篇文章中，作者们提出了两个方案，一个是简单版的，但是效率更高；另一个是完全版，更加安全，但是效率会底一些。这里我就只介绍一下这个简单版的方案，方案的具体过程如下：

***系统初始化算法*** (Setup) 。给定一个系统的安全参数 $k\in\mathbb{Z^+}$，生成整个系统需要使用的配对代数结果的各项参数，包括：

1. 阶为 $q$ 的循环群 $\mathbb{G_1}$ 和 $\mathbb{G_2}$，以及一个双线性映射关系：$e : \mathbb{G_1} \times \mathbb{G_1} \rightarrow \mathbb{G_2}$；
2. 并且，选择一个 $\mathbb{G_1}$ 这个代数结构上的一个生成元 $P$；
3. 选择一个随机元素 $s \in \mathbb{Z_q^*}$，并且计算系统中的公钥 $P_{pub} = s*P$；
4. 选择两个加密哈希函数 $H_1$ 和 $H_2$，其中，$H_1 : \{0,1\}^* \rightarrow \mathbb\{G_1^*\}$，而 $H_2 : \mathbb{G_2} \rightarrow \{0,1\}^n$；

因此，这个阶段就生成了整个系统的参数：$param = <q, \mathbb{G_1}, \mathbb{ G_2}, e, n, P, P_{pub}, H_1, H_2>$，以及系统主私钥：$MSK = s$。

***用户密钥生成算法 (Key Generation)***。给定一个用户的身份 $ID \in \{0,1\}^*$:

1. 根据哈希函数 $H_1$ 将用户的身份映射到群 $\mathbb{G_1}$ 上一元素：$Q_{ID} = H_1(ID)$；
2. 使用系统的主密钥 *MSK* 生成用户身份对应的私钥：$d_{ID} = s * Q_{ID}$。

***加密算法 (Encryption)***。给定一个消息 $M \in \mathcal{M}$ 和想要接受消息的用户的公钥：

1. 根据接收方的身份 *ID*，使用  $H_1$ 密码哈希函数，计算其对应到群 $\mathbb{G_1}$ 上对应的元素：$Q_{ID} = H_1(ID) \in \mathbb{G_1^*}$；
2. 选择一个随机元素：$r \in \mathbb{Z_q^*}$；
3. 计算对应的密文：$C = <r*P, M \bigoplus H_2(g_{ID}^r)>$ where $g_{ID} = e(Q_{ID}, P_{pub}) \in \mathbb{G_2^*}$

***解密算法 (Decryption)***。根据接受到的密文 $C = <U, V> \in \mathcal{C}$ 和用户的私钥 $d_{ID}$：
$$
V \bigoplus H_2(e(d_{ID},U)) = M
$$
验证解密过程的正确性：
$$
e(d_{ID},U) = e(s*Q_{ID},r*P) = e(Q_{ID},P)^{sr} = e(Q_{ID},s*P)^r=e(Q_{ID},P_{pub})^r=g_{ID}^r
$$
整个 *Dan* 的 ***IBE*** 方案和经典的公钥加密方案（如：*RSA*, *ElGamal*,*ECC*等）一样，存在公/私钥对，也就可以用来作为签名方案。以下，我列举一个用 *IBE* 作为签名的方案，详细可以参考这篇论文 [*Short Signatures from the Weil Pairing* ](https://link.springer.com/article/10.1007/s00145-004-0314-9)。

### ***BLS 签名***

1. 密钥生成阶段：根据安全参数，生成系统需要使用的配对代数结果的各项参数，包括：阶为 $q$ 的循环群 $\mathbb{G_1}$ 和 $\mathbb{G_2}$，以及一个双线性映射关系：$e : \mathbb{G_1} \times \mathbb{G_1} \rightarrow \mathbb{G_2}$；以及，确定两个哈希函数 $H_1 : \{0,1\}^* \rightarrow \mathbb\{G_1^*\}$；确定群 $\mathbb{G_1}$ 上的生成元 $P$；
2. 用户公/私钥对生成：和私钥 $ x \in \mathbb{Z_p^*}$ 和对应的公钥$Q_{ID} = x*P$；
3. 签名阶段：对于消息 $m$，计算对消息 $m$ 的签名，$W = x*H_1(m)$；
4. 验证阶段：当受到签名消息后，使用用户的公钥验证等式是否成立：$e(P,W) = e(Q_{ID},H_1(m))$；

可以从上面的两个例子来看，***IBE*** 的加密和签名方案可以满足传统公钥加密中存在公/私钥对，但是对于 ***ABE*** 这中加密方案来讲。2005年 *Sahai* 和 *Waters* 在论文 [*Fuzzy Identity-Based Encryption*](https://link.springer.com/chapter/10.1007%2F11426639_27) 中提出 ***ABE*** 以来，它本身并没有传统公钥加密系统中的公/私钥对的概念，而是作为一种用特殊的加密技术用于访问控制场景中。例如，从 ***ABE*** 演变出的 ***KP-ABE*** 和 ***CP-ABE*** ，分别为密钥策略以及密文策略的属性基加密方案。自此，大家的研究就是对这两种方案进行改进研究，包括：实现对方案过程中使用的访问策略进行隐藏以保证用户的隐私；系统中使用的属性事关用户的隐私，如何保护这些属性信息；如何将这种加密访问策略方案用于数据的安全共享的应用场景；以及由 ***KP-ABE*** 演变的可搜索加密技术和多个属性授权方管理下的属性基加密方案等等（以上是我目前所看到过的）。

### ***ABE 加密***

为了和其他公钥加密系统的方案做一个对比，我在这里简单的介绍一下 *Sahai* 和 *Waters* 关于 ***ABE*** 的整个过程。在这篇文章中， *Sahai* 等人的想法是我们用了一个集合的元素 $\omega$（在这里我们就可以理解为属性）对明文进行加密，只有当解密方用户拥有的元素集合 $\omega^\prime$ 满足 $\omega \bigcap \omega^\prime \geq d$ 时，才能正确解密。在我们看来，这里的$\omega \bigcap \omega^\prime \geq d$ 就是所谓的访问策略，里面的所有元素都可以用属性来反映。

***系统初始化阶段***

根据安全参数，生成阶为 $p$ 的群  $\mathbb{G_1}$ 和 $\mathbb{G_2}$；

同时，确定一个在 $\mathbb{G_1}$ 上的生成元 $g$ 和 双线性映射运算：$e : \mathbb{G_1} \times \mathbb{G_1} \rightarrow \mathbb{G_2}$；

并且，定义了一个朗格朗日系数，$S$ 为元素的集合：
$$
\Delta_{i,S}(x)=\prod_{j\in S,j\not= i}\frac{x-j}{i-j}
$$
确定整个系统用于加密使用的元素及其个数，$\mathcal{U}$ 和 $|\mathcal{U}|$；对这 $|\mathcal{U}|$ 个元素进行编号，为 $1, \dots,|\mathcal{U}|$，同时选择 $|\mathcal{U}|$ 个随机元素：$t_1, \dots, t_{|\mathcal{U}|} \in \mathbb{Z_p^*}$；并且，也选择一个随机元素 $y \in \mathbb{Z_p^*}$；

接着，生成整个系统的公钥参数：
$$
T_1 = g^{t_1}, \dots, T_{|\mathcal{U}|}=g^{t_{|\mathcal{U}|}},Y=e(g,g)^y
$$
和系统主密钥为：$t_1, \dots, t_{|\mathcal{U}|} \in \mathbb{Z_p^*}, y$ 。

***密钥生成阶段***

给定一个用户的元素结合 $\omega \subseteq \mathcal{U}$ ，生成一个度为 $d$ 的多项式 $q$，其中这个多项式满足 $q(0) = y$ ;

紧接着生成这个集合下的私钥：$(D_i = g^{\frac{q(i)}{t_i}})_{i \in \omega}$ 

***加密阶段***

确定加密阶段的元素集合 $\omega^{\prime}\subseteq \mathcal{U}$ 和消息 $M\in\mathbb{G_2}$；

随机选择一个元素 $s\in\mathbb{Z_p}$， 根据上面的参数，生成密文：
$$
E = (\omega^\prime, E^\prime = MY^s, \{E_i=T_i^s\}_{i\in\omega^\prime})
$$
***解密阶段***

根据用户的元素集合 $\omega$ 满足 $\omega \bigcap \omega^\prime \geq d$，因此，可以取 $\omega$ 的一个包含由 $d$ 个元素的集合 $S = \omega \bigcap \omega^\prime$；
$$
E^\prime /\prod_{i\in S}(e(D_i,E_i))^{\Delta_{i,S}(0)}\\
=Me(g,g)^{sy}/\prod_{i\in S}(e(g^\frac{q(i)}{t_i},g^{st_i}))^{\Delta_{i,S}(x)}\\
=Me(g,g)^{sy}/\prod_{i\in S}(e(g,g)^{sq(i)})^{\Delta_{i,S}(x)}=M
$$
加密阶段通过一个度为 $d$ 多项式对秘密值 $y$ 进行秘密的分享到每个元素上，而在解密阶段，则是通过朗格朗日系数还原出这个秘密值，进而完成解密。

通过上面的 ***IBE*** 加密和签名与 *Sahai* 的 ***ABE*** 方案对比，我可以发现在 ***ABE*** 方案中，不存在所谓的公/私钥。其加解密过程使用的参数主要为建立在一个元素（属性）集合上，同时，在中间过程中规定了用户拥有了哪些元素（属性）集合才能对密文进行解密（这里的 ***ABE*** 主要是通过在密钥中建立了一个访问策略来实现，思路和 ***KP-ABE*** 是一致的）。

另外，与传统的公钥加密系统相比。传统的公钥加密系统中的密文长度是由算法来决定的，顶多不会超过两个或三个代数结构上的元素来表示；而到了 ***ABE*** 这种方案，密文的长度与方案使用的元素（属性）集合的大小成正比的，那么带来的结果就是：我们有一个消息用群 $\mathbb{G_1}$ 上表示的一个元素 $M$ 来表示的话，如果我们使用 $n$ 大小的元素集合，且我们假设一个群 $\mathbb{G_1}$ 上的元素需要 128bit = 16B来表示的话，那么我们最终的密文的长度将会是 $16\ O(n)$ 这么长，要是我们把这种方案应用到数据的加密存储场景，这种加密方案带来的存储代价将会是非常高；当然，我们可以通过与非对称加密技术相结合，使用非对称加密技术对密文进行加密存储，而 ***ABE*** 只负责对非对称密钥进行加密，只有获得这个非对称密钥的用户最终才能获得明文。正是因为与传统的公钥加密体制存在概念/形式上的不同，研究界对类似于 ***ABE*** 这样的加密体制一个新名词解释，叫做 ***函数加密 Functional Encryption*** 。

关于函数加密的内容，我看得不是很多，在这里简单介绍一函数加密的定义，详细的内容可以参考 *Dan Boneh* 的 2011 年的论文 [*Functional encryption: Definitions and challenges*](https://eprint.iacr.org/2010/543.pdf) 。

### ***函数加密 (Functional Encryption)***

函数加密方案主要包括四个多项式算法，并且需要一个定义在 $(K,X)$ 这样一个空间的函数 $F$；具体的四个算法分别如下：

1. ***Setup***. $\bf (pp, mk) \leftarrow setup(1^{\lambda})$，主要用来生成公钥/主秘钥对；
2. ***Keygen***. $\bf sk \leftarrow keygen(mk,k)$，根据 $k\in K$ 和系统的主秘钥来生成 $k$ 对应的私钥；
3. ***Encrypt***. $\bf c\leftarrow enc(pp,x)$，加密消息 $x\in X$，获得密文；
4. ***Decrypt***. $\bf y \leftarrow dec(sk,c)$，使用 $sk$ 从密文 $c$ 中计算 $F(k,x)$。

在这里需要注意的是，这里的函数 $F$ 是定义在 $(K,X)$ 这两个空间上。其中，函数 $F : K \times X \rightarrow \{0,1\}^*$，而集合 $K$ 是密钥空间， $X$ 则是作为明文空间，同时，要求满足等式 $y = F(k,x)$。其实，说实话，通过看上面的定义，我根本不能理解函数加密的本质是什么。为了具体的描述一下函数加密的概念以及特点，我找到一篇介绍 ***Functional Encryption*** 的文章，名字就叫做 ***[Functional  Encrypiton: A New Vision for Public-Key Cryptography](https://dl.acm.org/doi/10.1145/2366316.2366333)***，下面我花些板块来介绍一下这里面关于函数加密的解释，内容主要摘抄自这篇论文。

### ***Functional Encryption: A New Vision for Public-Key Cryptography***

#### ***摘要 (Abstract)***

在公钥加密技术被发明以前，要保证信息通信的双方能安全的通讯的方法就是，在双方之间形成一把都拥有的私钥，把这把钥匙来保证通讯的安全。这种方法拥有它自身的特性，可以满足一定场景的需求，但是对于大规模互联网应用场景而言，这种对称的加密方案存在其局限性。也就是在这样的背景下，30年前， *Diffie* 和 *Hellman* 提出了公钥加密的技术，这种加密技术的特点就是，通信双方不需要进行商量出共同的钥匙，就能进行安全的通信。

如今，公钥加密技术在Web通信（如HTTPS和SSH）、语音通信和存储系统中十分重要，且无处不在。然而，公钥加密技术又经过这么久的发展与研究，技术界一直以来根深蒂固的看法是：

1. 访问加密的数据，“要么获得数据的全部，要么什么都获取不了”，即，要么可以对密文进行解密，要么也就只能获得明文的长度，其他的什么都获得不了；
2. 加密的过程就是对数据进行编码，只有唯一的一个私钥才能解密；

这样的看法带来的一个可能问题是，如果我们想要加密一个数据，只有当解密方满足否中我们提前设计好的策略时，才能对其进行解密的话，使用已经存在的公钥加密技术存在的问题是：

1. 我们如何去所有满足策略个体的公钥？
2. 如果当数据已经被加密存储了，当新的用户加入到系统中或获得新的属性（凭证）时，如何保证其对数据访问？
3. 如果我们只想让某些人，根据他们自身的属性访问部分加密数据的话，又该如何？
4. 一个给定的用户是否应该被允许学习所有拥有特定证书的个人的身份？

因为，公钥加密技术本身的限制，在这篇文章中作者们倡议使用一个新的概念：函数加密 (Functional Encryption)。在这种加密技术中，解密密钥可以让用户去获得一个关于加密数据或者什么都没有的特殊函数。整个过程大致为：在这个加密技术中，存在一个可信的授权方，只有它能拥有一个叫做主秘钥的钥匙；当权威机构被给于一个关于函数 $f$ 的描述作为输入时，它用它拥有的主秘钥去产生一个与 $f$ 相关联的的派生私钥 $sk[f]$ ；如此的话，任何拥有 $sk[f]$ 的用户就可以从 $x$ 的加密 $c$ 中计算 $f(x)$ 。

用符号来表示的话，如果 $E(pk;x)$ 为 $x$ 的加密，那么给定 $E(pk;x)$ 和 $sk[f]$，我们就可以解密输出 $f(x)$，这个 $f(x)$ 就可以被私钥拥有一方所获得，按照这样的思路，函数加密可以拥有各种各样的函数。而关于这种技术的安全性的话，需要保证私钥的拥有方除了获得 $f(x)$ 外，关于 $x$ 的信息什么都学习不到，和安全多方计算的概念很相似，但是两者之间有一个重要的不同，那就是函数加密是一个不需要交互的方案。考虑以下函数加密可能的应用场景：

1. 加密邮件中的垃圾邮件过滤 (Spam filtering on encrypted mail)

   如果我们的邮件接收方想要一个邮件过滤机构对自己加密邮件中的垃圾邮件进行清除的话，他只需要使用自己的私钥 $sk$ 根据一个函数 $f(x)$ 生成一个私钥 $sk[f]$ 给于这个过滤机构；过滤器用 $sk[f]$ 对加密的邮件进行解密，当有 $D(sk[f],c) = f(x) =0$ 时，认为不是垃圾邮件，就可以推送给对应的用户；如果 $f(x) = 1$ 则代表为垃圾邮件，就需要把这个邮件进行过滤掉；

2. 可以表达的访问控制 (Expressive Access Control )

   用户在加密数据 $m$ 时，可以给数据一个访问策略 $P$ （用来表达自己共享数据的方法）。于是，加密时就可以对 $x= (P, m)$ 进行加密，而用户的私钥 $sk[f]$ 会去检查用户的身份和属性是否满足这个访问策略，进而获得明文 $m$ 。

3. 挖掘大规模数据集

   数据挖掘作为一门重要的数据处理技术，被应用于很多行业。但是，想要让人挖掘到数据背后价值的同时，却获取不了数据本身信息的话，函数加密技术就有了自己的长处。例如，医疗行业、金融行业、电子商务行业等。

#### ***Functional Encryption***

在公钥加密方案中，一般包含了三个算法：*Setup* 、*Encryption*、*Decryption* 。而函数加密系统中，需要四个算法来完成，比公钥加密系统多了一个算法：*KeyGen* 。其中，在 *Setup* 阶段产生的私钥被称为主私钥 $mk$ ，而 *KeyGen* 算法要根据 $mk$ 和一个关于函数 $f$ 的描述，产生一个关于函数 $f$ 的特殊钥匙 $sk[f]$ ，如果我们假设 $c$ 为 $x$ 的加密结果，那么我们通过计算 $D(sk[f],c)$ 可以得到 $f(x)$，这里的 $f(x)$ 仅仅是明文 $x$ 的一个函数，要想全部获得明文 $x$ ，就得使用另外的私钥。

以下的几个例子，用函数加密的思想来解释身份基加密 (***IBE***) 和属性基加密 (***ABE***)。

***Identity-based encryption***

如果用函数加密的思路来介绍身份基加密方案的话，身份基加密可以被看作是一个等式测试函数的函数加密。$pk$ 和 $msk$ 为函数加密的 *Setup* 算法产生的公钥和主秘钥；为了用身份 $id$ 加密消息 $m$ ，加密方调用加密算法 $E(pk;(id,m))$ 并获得密文 $c$ ；而接受方根据自己的身份 $id^*$ 在授权方获得私钥 $sk[f_{id^*}]$，而函数 $f_{id^*}$ 的定义如下：
$$
f_{id^*} =\left\{\begin{aligned}m \ \ \ \ if \ id  =  id^*\\
\bot\ \ \ \ otherwise
\end{aligned}\right.
$$
使用这个函数私钥，用户可以获得 $id^*$ 对应的明文，而不能获得其他身份对应的明文。

***Attributed-based encryption***

在 ***ABE*** 中，加密方给出一个建立在属性集合上访问策略 $\varphi$ ，以用来确定哪些人可以解密；为了去加密消息 $m$ ，加密方使用 $E(pk;(\varphi, m))$ 对消息进行加密，并获得密文 $c$；而对于系统中的每个用户而言，他/她都有自己的一个由授权方给于的属性集合，假设系统中的总的属性为 $n$ 的话，可以用一向量 $\vec{u}$ 来表示用户的属性集合，有这个属性的位置表示为1，没有这个属性的位置表示为0，那么每个用户在授权方那里获得对应属性集合的私钥 $sk[f_{\vec{u}}]$，其中，函数 $f_{\vec{u}}$ 的定义如下：
$$
f_{\vec{u}}(\varphi,m)=\left\{\begin{aligned}
m\ \ if \ \ \varphi(\vec{u})=1\\
\bot \ \ otherwise \\
\end{aligned}
\right.
$$
当解密方的属性集合满足这个访问策略时，就可以获得对应密文的明文，而不能获得其他密文的明文信息。

总之来讲，函数加密技术与公钥加密技术，两者的关系是一种属于关系，公钥加密可以看成是函数加密的一种特殊形式。

就我自己现在看的内容来讲，更多的还是关于 ***ABE*** 方案中的 ***CP-ABE***，且目前接触的方向为对访问策略和属性的隐藏以保护用户的隐私，策略隐藏有全策略隐藏和部分策略隐藏这两个方面。而访问策略的隐藏相关的技术，就我目前所接触到的，我觉得很有意思的一个方向，就是用布隆过滤器来实现对访问策略的隐藏。在这里，就介绍一篇 2017 年发表在 *IEEE Internet of Things Journal* 杂志上的一篇文章 [“*An Efficient and Fine-Grained Big Data Access Control Scheme with Privacy-Preserving Policy*”](https://ieeexplore.ieee.org/document/7476833) ，在这里面作者使用了布隆过滤器来实现用 *LSSS* 矩阵表示访问策略的 *CP-ABE* 加密方案的策略隐藏。

### ***Bloom Filter in CP-ABE with LSSS Matrix Access Policy***

关于布隆过滤器的知识，在这里就不多做介绍，有兴趣的可以去参考 [*Bloom Fileter*](https://www.jasondavies.com/bloomfilter/) 的介绍。

在这篇文章中的实现方案上，主要包括了四个算法：

***系统初始化 (System Setup)***: $\bf Setup(1^\lambda)\rightarrow(PK, MSK)$

1. 属性授权方根据安全参数，生成整个系统需要使用各项参数，包括：系统所有的属性、双线性配对代数结构；因为使用到了布隆过滤器，系统参数里面还需要包含：表示属性所需要的最大位长 $L_{att}$ 和表示 *LSSS* 矩阵行数的最大位长 $L_{rownum}$ ，以及用 $k$ 来表示在属性布隆过滤器 (***ABF***)中使用的哈希函数的数量，$L_{ABF}$ 代表属性布隆过滤器需要位数组的长度；
2. 属性授权方根据群 $\mathbb{G_1}$ 和阶 $p$ 生产一下参数。生成元 $g \in \mathbb{G_1}$；随机元素 $\alpha, a \in \mathbb{Z_p^*}$；和 $|\mathcal{U}|$ 个随机元素 $h_1,\dots, h_U \in \mathbb{G_1}$；同时，还需要选取 $k$ 个哈希函数 $H_1(), H_2(),\dots,H_k()$，把输入映射到 $[1,L_{ABF}]$ 这个空间上；

因此，系统的公钥为 $PK=<g, e(g,g)^\alpha, g^a, L_{att}, L_{rownum}, L_{ABF}, h_1, h_2, \dots, h_U, H_1(), \dots, H_k()>$；系统主秘钥为 $MSK=g^\alpha$。

***密钥生成阶段 (Key Generation)***: $\bf KeyGen(PK,MSK,S) \rightarrow SK$

每个用户都需要在属性授权机构那里授权获得自己的属性结合 $S \sube \mathcal{U}$，同时生成对应于这个属性集合的秘钥。

1. 首先，随机选取一个元素 $t\in\mathbb{Z_p^*}$;
2. 随后，根据每个用户授权的属性集合获得对应的秘钥。$K = g^\alpha g^{at}$； $L=g^t$；$\{K_x=h_x^t\}_{x \in S}$。

因此，用户的私钥为 $SK=<K,L,\{K_x\}_{x \in S}>$。

***数据加密阶段 (Data Encrypiton)***: $\bf Encrypt(PK,m,(\mathbb{M},\rho))\rightarrow (CT,ABF)$

在这篇文章中，作者使用的访问策略表达形式为 *LSSS* 矩阵，和 2011年 *Waters* 里面的方案是一模一样的。这里的 $\mathbb{M}$ 就是 *LSSS* 矩阵，我们依然假设为 $m \times n$ 的矩阵，而 $\rho$ 则是把矩阵的行与属性值进行映射的一个映射函数。因为，在这里需要对策略进行隐藏，作者使用布隆过滤器对映射函数进行替换，从而实现对策略的隐藏。在这个过程中，主要包含了两个不同的算法：

$\bf Enc(PK,m,(\mathbb{M},\rho))\rightarrow CT$

这个算法和前面介绍的 *Waters* 的那篇文章中加密过程一样，最终生成的密文信息为：

$CT=<C=me(g,g)^{\alpha s}, C^{\prime} = g^s,\{C_i=g^{a\lambda_i}h_{\rho(i)}^{-s} \}_{i=i,\dots,m}$ 

我在这里主要详细介绍一下第二个算法整个过程，因为在传统的 ***CP-ABE*** 中，访问策略和映射函数是要和密文一起发送给接受方，如此带来了用户的隐私问题。因此，在这篇文章中，作者移除了属性映射函数 $\rho$ ，但是，如果移除了映射函数的话，那么在解密过程就没办法实现。于是，作者使用了布隆过滤器来实现对属性进行定位，用于确定那个某个属性是属于矩阵的哪一行。

传统的布隆过滤器只能用来判断一个属性是否存在于集合当中，而这里需要准确的知道这个属性是位于那个行号；同时，传统的布隆过滤器存在假样例（属性实际不存在与集合，但是判断却显示存在）的情况，因此不能直接使用传统的布隆过滤器。因此，在这里，作者使用了一个叫做 "garbled 布隆过滤器"来对属性进行准确定位。传统的布隆过滤器使用的是多个位的数组来保存元素的映射，而 ”garbled BF” 使用的是长度为 $\lambda$ bit 长的数组来表示。因为，表达结构的不同，因此这个方案的假样例的概率十分的低，可以应用。$\lambda$ 位的前半部分为 $L_{rownum}$ 位的行下标，后半部分为 $L_{att}$ 位表示属性，因此有，$\lambda = L_{rownum}+L_{att}$。具体的过程通过以下这个算法完成：

$\bf ABFBuild(\mathbb{M},\rho) \rightarrow ABF$

首先，根据访问矩阵中的行号已经对应的属性，我们可一个获得一个元素集合 $S_e=\{i||att_e\}_{i\in [1,m]}$，其中，$att_e = \rho(i)$，同时，通过在最左边添加 0 来使行号和属性串表示成最大比特串；

把获得的元素集合 $S_e$ 作为 *garbled BF* 的输入，取元素 $e \in S_e$ ，根据 $(k,k)$ 秘密分享方案，随机的选取 $k-1$ 个 $\lambda$ 位的比特串 $r_{1,e},r_{2,e},\dots,r_{k-1,e}$，对 $e$ 进行秘密分享并取得:
$$
r_{k,e}=r_{1,e}\bigoplus r_{2,e},\dots,\bigoplus r_{k-1,e}\bigoplus e
$$
然后，使用 $k$ 个哈希函数对属性 $att_e$ 进行映射，并获得 $H_1(att_e),H_2(att_e),\dots,H_k(att_e)$，每个 $H_i(att_e)$ 代表这个属性在 ***ABF*** 中的索引位置，于是就把 $r_{i,e}$ 存储到 $H_i(att_e)$ 位置处。
$$
r_{1,e}\rightarrow H_1(att_e)\\
r_{2,e}\rightarrow H_2(att_e)\\
\dots\\
r_{k,e}\rightarrow H_k(att_e)
$$

当我们继续属性布隆过滤器中添加元素 $e^\prime$ 时，可能存在 $j= H_i(e^\prime)$ 位置已经被之前的元素给占了。当出现这种情况时，采取的方法就是依旧使用前一次的字符串，不对其进行替换，而是取 $r_{j,e^\prime} = r_{i,e}$。按照如此的办法，把元素集合中的每个元素都通过属性布隆过滤器映射到一个元素长度为 $\lambda$ 比特长的串数组中。

最终，加密阶段过程形成的密文信息为 $(CT,\mathbb{M},ABF)$，就可以把其上传到云服务器上进行保存，等待被其他用户进行解密访问。

***数据解密阶段 (Data Decryption)***：$\bf (\mathbb{M},ABF,PK,SK,CT)\rightarrow m$

当用户接受到密文形式的数据以后，就可以根据自身所拥有的属性集合对密文进行解密，只有当用户的属性集合满足访问策略时，才能正确完成对密文的解密；相反，如果用户的属性集合不满足访问策略，则什么都获得不了。

在本方案中，因为加入了属性布隆过滤器来实现对映射函数的隐藏。等加密数据到了用户端时，他/她不清楚自己的属性对应秘密矩阵的哪一行，进而就不能准确的完成解密过程。因为在加密阶段把属性以及其对应的行号通过属性布隆过滤器进行了保存，到了解密阶段就可以通过属性的查找来发现当前属性是否位于访问策略中，并且能准确的找到位于那行。具体的过程，通过一下两个算法来实现：

$\bf ABFQuery(S,ABF,PK) \rightarrow \rho^\prime$

在这里的 $S$ 为用户的属性集合，对属性集合中的每个属性 $att\in S$，这个算法首先通过 $k$ 个哈希函数计算获得 $H_1(att),H_2(att),\dots,H_k(att)$；然后，把这 $k$ 个哈希值作为 ***ABF*** 中的索引，依次获得：
$$
H_1(att) 位置处的串 \rightarrow r_{1,e}\\
\dots\\
H_k(att) 位置处的串 \rightarrow r_{k,e}
$$
然后，根据这 $k$ 个二进制串还原出一开始的 $i||att_e$ 的二进制形式：
$$
e=r_{1,e}\bigoplus r_{2,e}\bigoplus\dots\bigoplus r_{k,e}
$$
因为在 $e$ 中的前 $L_{rownum}$ 位为行号，后 $L_{att}$ 位代表的是属性；于是，通过这 $k$ 个二进制串的异或操作，便还原出当前属性对应的行号，可以用如下实行表达：$\rho^\prime = \{(rownum,att)\}_{att\in S}$。在获得访问策略 $(\mathbb{M},\rho^\prime)$ 后，就可以完成对密文的解析:

$\bf Dec(SK,CT,(\mathbb{M},\rho^\prime))\rightarrow m\ or\ \bot$

剩下的解密过程和前面 *Waters* 在 2011 年那篇一样，即可完成对密文的解析。通过上面的过程，我们可以发现，本文中的方案在 2011 年 *Waters* 的那篇文章中的方案很相似，不同的地方在于，本文使用一个被称为属性布隆过滤器的技术，实现对属性到对应行号映射函数的消除，以实现对访问策略的隐藏，保护用户的隐私。

通过上面介绍的关于用属性布隆过滤器实现对访问策略的隐藏，我心中就产生了一个疑问，那就是在布隆过滤器中使用的哈希函数是如何工作的；通过文章实验部分的介绍，作者使用的是一种被称为 *Murmurhash* 的哈希算法。关于 *Murmurha* 的具体实现方法，大家可以去参看 [*Austin Appleby*](https://github.com/aappleby/smhasher) 关于 *Murmurhash* 的源码实现与测试。

继续回到这篇文章的属性策略隐藏来看，虽然通过属性布隆过滤器 (*garbled BF*) 实现了对映射函数替代，在一定程度上实现了对访问策略的隐藏。但是，从整个方案过程来看，*LSSS* 矩阵是直接以明文的形式保存，而我们也知道 *LSSS* 矩阵是一种特殊的矩阵，加上用户自己的属性集合，在很大程度上可以还原出大致的访问策略，对用户隐私依旧形成影响。因此，有没有办法实现对 *LSSS* 矩阵的保护，以求得 100% 的策略隐藏，这是一个可以值得研究的思路。我想在自己的论文里面可以有对 *LSSS* 矩阵的隐藏能力，但是这将导致一个问题就是计算效率可能会降低，如何在隐私保护和计算效率之间形成一种折中，也是一个不错解决办法。

最近，也是在师兄的推荐下，看了一篇用到 *区块链* 技术加 *CP-ABE* 加密技术来实现可信访问控制的方案，这是一篇发表在 *IEEE Transactions on Vehicular Technology* 上，名为 *[TrustAccess: A Trustworthy Secure Ciphertext-Policy and Attribute Hiding Access Control Scheme Based on Blockchain](https://ieeexplore.ieee.org/document/8961176)*。而这篇文章中关于 *CP-ABE* 方面的策略隐藏及其安全性证明部分都是来自于 2011 年 *Lai* 等人在 [*Fully secure cipertext-policy hiding CP-ABE*](https://link.springer.com/chapter/10.1007%2F978-3-642-21031-0_3) 这篇文章中的工作，在这里我们先回顾一下 *Lai* 等人在 2011 年关于 *CP-ABE* 的工作内容。

### ***Fully secure cipertext-policy hiding CP-ABE in 2011***

#### ***背景 Background***

这篇文章与我以前看到关于 *CP-ABE* 不同的第一个地方在于，使用的是合数阶上面的双向性映射群。具体关于合数阶双线性映射群的介绍请参考论文 [*Evaluating 2-dnf formulas on ciphertexts*](https://link.springer.com/chapter/10.1007/978-3-540-30576-7_18)，在这里我做一个简单的关于合数阶双线性群映射的介绍：

根据安全参数 $\lambda$ 生成合数阶双线性映射群的各项参数列表 $<p,q,r，\mathbb{G},\mathbb{G}_T,e>$；其中，$p,q,r$  为三个素数，而 $\mathbb{G},\mathbb{G}_T$ 为阶为 $N=pqr$ 的循环群；而 $e: \mathbb{G} \times \mathbb{G} \rightarrow \mathbb{G}_T$ 为一个双线性映射，且满足一下性质：

1. 双线性。 $\forall g,h \in \mathbb{G},a,b\in \mathbb{Z}_N,e(g^a, h^b)=e(g,h)^{ab}$；
2. 非简化性。$\exists g\in \mathbb{G}$ 满足 $e(g,g)$ 的阶为 $N$。

同时，要求这个双线性映射计算可以在有限时间完成。以上特性和一般的双线性映射一致，这里是合数阶。另外，不同的一个特点就是正交性。我们假设 $\mathbb{G}_p,\mathbb{G}_q,\mathbb{G}_r$ 分别为 $ \mathbb{G}$ 的三个子群，三个群的阶分别为 $p,q,r$，那么我们可以取 $h_p\in \mathbb{G}_p$ 和 $h_q \in \mathbb{G}_q$ ，则根据双线性配对有 $e(h_p,h_q)=1$。即，$\mathbb{G}$ 上的不同子群上的元素进行双线性配对以后，映射到 $\mathbb{G}_T$ 上的单位元。

同时，为了实现对密文访问策略的隐藏，这篇文章中使用了一个叫做 *[内积谓词加密](https://iacr.org/archive/eurocrypt2008/49650145/49650145.pdf)* 技术，而这个概念其实就是前面介绍的函数加密里面的一种特殊函数，即我们熟悉的内积操作。其实这篇文章中的内积谓词加密和函数加密的标准定义有些不同的地方，但是只要清楚：用户的属性集合作为一个向量，访问策略中属性集合作为一个向量，只有当解密方的属性集合向量与密文策略中的向量的内积为 0 时，才能完成对密文的解析，否则不能解密。

其中，任何一个合取/析取范式和属性集合都可以使用一个  $(d+1)^t$ 大小的元素向量表示，$d$ 代表属性类的取值数量，$t$ 代表属性类别数量。作者们在文章中举了一个表示访问策略的例子：$\mathbb{A}=\ Department: CIA\ \\ \ \ \ \ \ \ \ \ \ AND (Position:\ Manager\ OR\ Seniority:\ Senior)$ 

这个访问策略通过多项式：
$$
p(x_1,x_2,x_3)\\
=r(x_1-I_1)+(x_2-I_2)\cdot(x_3-I_3)\\
=0\cdot x_1x_2x_3+0\cdot x_1x_2+0\cdot x_1x_3+1\cdot x_2x_3+r\cdot x_1\\
+(-I_3)\cdot x_2+(-I_2)\cdot x_3+(I_2I_3-rI_1)
$$
其中，$r\in \mathbb{Z}_N$ 随机选择的，$I_1=H(Department:\ CIA),\ I_2=H(Position:\ Manager),\ I_3=H(Seniority:\ Senior)$，而哈希函数 $H:\{0,1\}^*\rightarrow\mathbb{Z}_N$。于是，对于这样一个访问策略，便可以用一个大小为 $(d+1)^t=2^3=8$ 的向量来表示：

$\vec{x}=(0,\dots,0,1,r,-I_3,-I_2,I_2I_3-rI_1)$

而对于一个用户的属性集合为 $S=(Department:\ CIA,Position:\ Director,Seniority:\ Senior)$ 的用户而言，同样可以表示成一个大小为 8 的向量：

$\vec{v}=(I_1^\prime I_2^\prime I_3^\prime,I_1^\prime I_2^\prime,I_1^\prime I_3^\prime,I_2^\prime I_3^\prime,I_1^\prime,I_2^\prime,I_3^\prime,1)$

其中，$I_1^\prime=H(Department:\ CIA),I_2^\prime=H(Position:\ Director),I_3^\prime=H(Seniority:\ Senior)$。于是，如果用户的属性集合满足访问策略 $\mathbb{A}$，那么我们就有 $\vec{x}\cdot \vec{v} = 0$。但是，这种方法的一个缺点就是，它只能支持 AND 和 OR 这样形式的访问策略，对于阈值表达形式的访问策略，无法实现。尽管，可以通过布尔逻辑运算来实现阈值策略，但是表达起来有些不方便，这里可以存在一个潜在的研究思路。

现在我们具体来看一下，*Lai* 等人关于带有隐藏密文策略的 *CP-ABE* 的具体实现方案。

#### ***Ciphertext-Policy Hiding CP-ABE Scheme***

在这个只能支持有限访问结构的 *CP-ABE* 方案中，对系统中的属性进行划分，即属性=属性值的形式。所有的属性有 $m$ 种，每种属性可以最多取 $n$ 个不同的属性值，少于 $n$ 个的可以使用通配符来表示，因此，用向量表示的访问策略的向量长度就为 $(n+1)^m$ 种，可以看出这个长度还是挺长的了。同时，每个用户都会存在每类属性中的某一个属性值。

用符号表示的话，全局属性可以用 $m\times n$ 的矩阵来表示 $V=(V_1,\dots,V_i,\dots,V_m)$，其中 $V_i=(v_{i,1},\dots,v_{i,j},\dots,v_{i,n})$，且 $v_{i,j} \in \mathbb{Z}_N$。即，$V_i$ 为第 $i$ 类属性可能会取的属性值。而对于用户来讲，其可以拥有每个属性中的一个属性值，例如 $S=(v_{1,j_1},\dots,v_{i,j_i},\dots,v_{m,j_m})$，其中，$j_i\in \{1,\dots,n\}$。而对于访问策略 $\mathbb{A}=(W_1,\dots,W_m)$，就一定有 $W_i\subseteq V_i$，对于用户集合 $S$ 满足访问策略的话，就一定有 $v_{i,j_i}\in W_i$，其中 $1 \leq i \leq n$。以下就是本文方案的四个算法：

$\bf Setup(1^\lambda)$

1. 通过给定安全参数，生成合数阶上的双线性配对映射代数结构：$\mathcal{G}(1^\lambda)\rightarrow(p,q,r,\mathbb{G},\mathbb{G}_T,e)$，其中，$\mathbb{G},\mathbb{G}_T$ 的阶为 $N=pqr$；
2. 同时，选取 $\mathbb{G}_p,\mathbb{G}_r$ 上的生成元分别为 $g_p,g_r$，$a_{i,j}\in \mathbb{Z}_N,R_{i,j}\in \mathbb{G}_r$，且 $1 \leq i \leq m, 1 \leq j \leq n$，即针对系统中的每个属性对应的每个属性值，都要分配一个随机值；
3. 最后，再随机的选取 $\omega \in \mathbb{Z}_N$ 和$R_0\in \mathbb{G}_r$；

因此，最后形成公钥：$PK=<A_0 = g_p\cdot R_0,\{A_{i,j}=g_p^{a_{i,j}}\cdot R_{i,j}\}_{1\leq i \leq m,1 \leq j \leq n},g_r,Y=e(g_p,g_p)^\omega>$；主秘钥为：$MSK=<g_p,\{a_{i,j}\}_{1\leq i \leq m,1 \leq j \leq n},\omega>$。

$\bf KeyGen(PK,MSK,S)$

在这里的集合 $S$ 依旧代表用户的属性集合，$S=(v_{1,j_1},v_{2,j_2},\dots,v_{i,j_i},\dots,v_{m,j_m})$，其中 $j_i\in \{1,\dots,n \}$。然后，就可以根据系统的主秘钥和公钥参数生成用户自己属性值集合上的秘钥。

1. 首先，随机的选取 $m$ 个值 $t_i \in \mathbb{Z}_N$，并且取 $t=\sum _{i=1} ^ n t_i$；
2. 然后，就可以计算 $D_0=g_p^{\omega - t}$ 和 $\{g_p^{t_i/a_{i,j_i}} \}_{1 \leq i \leq m}$；

于是乎，我们就可以得到 $S$ 属性集合下的私钥：$SK_S=<D_0,\{D_i \}_{1\leq i \leq m}>$。

$\bf Encrypt(PK,m,\mathbb{A})$

确定访问策略 $\mathbb{A}=(W_1,\dots,W_m)$，其中，$W_i\subseteq V_i$。

1. 首先，算法随机的选取 $s\in \mathbb{Z}_N$ 和 $R_0^\prime \in \mathbb{G}_r$；

2. 接着，选取 $s_{i,j}\in \mathbb{Z}_N$ 和 $R_{i,j}^\prime \in \mathbb{G}_r$；其中，$1 \leq i \leq m, 1\leq j \leq n$；

3. 然后，就可以计算密文与密文策略：$C=m\cdot Y^s$,$C_0=A_0^s \cdot R_0^\prime$；而对于密文策略而言，这篇文章中却是对系统中的所有属性进行了参与运算，只是分是否位于访问策略中：
   $$
   C_{i,j}=\left\{
   \begin{aligned}
   A_{i,j}^s\cdot R_{i,j}^\prime,\ \ if\ v_{i,j}\in W_i\\
   A_{i,j}^{s_{i,j}}\cdot R_{i,j}^\prime, \ \ otherwise
   \end{aligned}
   \right.
   $$

因此，最终的密文及密文策略为：$c=<C,C_0,\{C_{i,j} \}_{1 \leq i \leq m, 1\leq j \leq n}>$。

$\bf Decrypt(PK,SK_S,c)$

在这里，就需要根据用户的属性集合 $S$ 来完成密文的解密，如果给定用户的属性集合 $S$ 满足访问策略 $\mathbb{A}$ 的话， 那么用户即可完成解密：
$$
\frac{C}{e(C_0,D_0)\cdot \prod_{i=1}^m e(C_{i,j_i},D_i)}
$$
通过，简单的运算就可以得出上面的式子的最后结果为 $m$，即可完成解密。安全性证明的话，可以参考论文中的附录部分。下面，我还是回到 2020 年哪一篇论文用区块链技术实现可信访问控制方案。

### ***TrustAccess: A Trustworthy Secure Ciphertext-Policy and Attribute Hiding Access Control Scheme Based on Blockchain***

在前面的内容，我也提到了本文中的 *CP-ABE* 方面内容和前面讲的 *Lai* 在 2011 年的那篇工作一样，唯一不同的地方在于系统初始化阶段，减少了系统主秘钥参数，用一个参数 $a\in \mathbb{Z}_N$ 去替换了 $a_{i,j} \in \mathbb{Z}_N $ 的作用，并且，用 $A_1$ 去替换了 $A_{i,j}$，但是后面的秘钥生成、加密和解密过程就只需要这一个主秘钥参数 $a$ 和公钥 $A_1$ 即可。

在这里，我主要介绍这篇文章中如何把带有策略隐藏的 *CP-ABE* 方案与区块链技术进行结合。因为，涉及到了区块链的知识，这是一个我不熟悉的领域。不过这篇文章中，更多是使用区块链去中心化思想，加上区块链中最为重要*事务*和*智能合约*这两个工具，给出了实现可信访问的四个事务和智能合约算法，而没有在以太坊上实现事务与智能合约的算法，但这也可以作为后续工作的方向，大家有兴趣的话，可以沿着这个思路做一做。关于区块链的知识，大家可以去网上进行搜索了解，这里给出一个[*以太坊*](https://ethereum.org/zh/) 和智能合约编程语言 *[Solidity](https://solidity-cn.readthedocs.io/zh/develop/#)* 的链接。

这篇文章除了使用区块链技术以实现可信的访问，为了保护用户属性的隐私，作者们还是用建立在 *ElGamal* 加密系统之上的乘法同态算法，而主要是在智能合约中实现用户属性向量与访问策略向量的同态乘，以保护用户侧的属性隐私。

整个 ***TrustAccess*** 的系统架构如下图：

![TrustAccess的系统架构](https://github.com/dwwcqu/CryptoABE/blob/master/images/trustaccess.png)

在这个架构中步骤 1、步骤 7和步骤 8 和 *Lai* 的 *CP-ABE* 方案是一样的，在这里我们重点介绍步骤 2、步骤 3、步骤 4、步骤 5 和步骤 6，关于区块链事务与智能合约实现部分。现在，重点看一下本文中的四个过程：

***Setup Phase***

$\bf Setup(1^\lambda)$

确定合数阶上配对运算的代数结构 $(p,q,r,\mathbb{G},\mathbb{G}_T,e)$；生成元 $g_p\in \mathbb{G}_p,g_r \in \mathbb{G}_r $和 $N=pqr$。

随机选取 $a,\omega\in \mathbb{Z}_N$ 和 $R_0,R_1\in \mathbb{G}_r$;

最终确定，公钥：$PK=<A_0 = g_p\cdot R_0,A_1=g_p^a\cdot R_1,g_r,Y=e(g,g)^\omega>$和主秘钥 $MSK=<g_p,a,\omega>$。

根据这篇文章中构建的模型和内容介绍说，系统的初始化阶段是由数据拥有者 *DO* 来完成，意思就是说每个人要上传数据时，都要进行这个过程，而这些需要参与运算的参数也都由数据拥有者来保管，符合区块链去中心化特性，不需要一个中心化机构来集中式管理整个系统的参数。但是，从用户侧来看，只要自己生成了一次参数，可以用一段时间以后，更新自己的各项参数，以达到安全目的，消耗依旧是用户侧算力，也算是一种折中。

***Encryption Phase***

$\bf Enc(PK,m,\mathbb{A})$

这里的访问策略 $\mathbb{A}=(W_1,\dots,W_m)$ 与 *Lai* 的策略一样；随机选择元素 $s,s_{i,j}\in \mathbb{Z}_N$ 和 $R_0^\prime,R_{i,j}^\prime \in \mathbb{G}_r$；然后，计算密文 $C=m\cdot Y^s,C_0=A_0^s\cdot R_0^\prime$；和密文策略：
$$
C_{i,j}=\left\{
\begin{aligned}
A_{1}^s\cdot R_{i,j}^\prime,\ \ if\ v_{i,j}\in W_i\\
A_{1}^{s_{i,j}}\cdot R_{i,j}^\prime, \ \ otherwise
\end{aligned}
\right.
$$
其中，$1 \leq i \leq m, 1 \leq j \leq n$，再把访问策略 $\mathbb{A}$ 变成向量形式 $\vec{x}$。

最终，可以获得加密阶段的密文和密文策略：$CT=<\vec{x},C,C_0,\{C_{i,j} \}_{1 \leq i \leq m, 1 \leq j \leq n}>$。

***On-blockchain Phase***

当数据拥有者 *DO* 完成对数据和策略的加密后，在本文中作者假设的场景是把数据保留在本地；然后，*DO* 根据算法 1，即生成密文和密文策略的存储事务：

| 算法 1：生成区块链上的存储事务                               |
| :----------------------------------------------------------- |
| **Input**: $S$ 存储事务标识符；$storeAddress$ 密文存储地址；$CT$ 密文；$BSK_{DO}$ *DO*的私钥； |
| **Output**：$Tx_{storage}$ 区块链中的存储事务                |
| 1. $checkCode = H(CT)$ 生成密文的校验码                      |
| 2. $MD=H(S,storeAddress,checkCode)$ 计算事务的数据摘要       |
| 3. $sign = Sign_{BSK_{DO}}(MD)$ 使用 *DO* 的私钥对数据摘要进行签名 |
| 4. $Tx_{storage}=<S,storeAddress,checkCode,sign>$ 生成存储事务 |
| 5. $return\ \ Tx_{storage}$                                  |

在数据拥有者 *DO* 完成存储事务生成算法以后，它就会被广播到区块链的其他节点上，以便用于认证，主要是通过里面的密文校验码认证密文的完整性；用签名认证密文的合法性。验证过程，主要通过算法 2来实现。主要是通过比较前后的数字摘要和密文校验码是否相同，来验证存储事务的合法性。

| 算法 2：验证存储事务 $Tx_{storage}$                          |
| ------------------------------------------------------------ |
| **Input**: $Tx_{storage} = <S,storeAddress,checkCode,sign>$;$BPK_{DO}$ 数据拥有者的公钥； |
| **Output**：$Tx$ 与 $CT$ 的合法/完整否                       |
| 1. $MD^\prime = H(S,storeAddress,checkCode)$ 根据存储事务中的地址和校验码再次进行哈希运算 |
| 2. $MD=Compute_{BPK_{DO}}(sign)$ 根据公钥和签名对签名进行认证 |
| 3. 如果 $MD^\prime = MD$                                     |
| 4.        $checkCode^\prime = H(CT)$ 根据密文地址获得密文后，对消息进行校验码计算 |
| 5.        如果 $checkCode^\prime=checkCode$                  |
| 6.                $return\ \ True$                           |
| 7. $return \ \ False$                                        |

一旦完成对存储事务的验证，这个事务经过[*拜占庭容错协议* ](https://dl.acm.org/doi/10.5555/296806.296824)就会被打包成块，保存到区块链中去。

如果存在数据用户想要访问这个被加密的数据时，为了保护用户的属性隐私，只需要在用户侧判断用户的属性集合是否满足访问策略，即，用我前面讲的内积谓词加密技术。如果，用户的属性向量 $\vec{v}$ 满足 $\vec{x}\cdot \vec{v}=0$，那么就反应用户的属性集合满足访问策略，*DU* 于是产生一个证明：

$\bf Proof = <BPK_{DU},storeAddress,E(\vec{v}),sign>$

其中，$BPK_{DU}$ 为用户的公钥；$storeAddress$ 代表 *DU* 想要访问的密文地址；$E(\vec{v})$ 代表用户属性向量经过 *ElGamal* 加密系统产生的向量；$sign$ 代表 *DU* 对 **Proof** 的签名，

一旦，*DU* 完成了 **Proof**，就要产生一个验证过程，以确定当前用户是满足访问需求的，主要通过算法 3来完成。其中，在这个过程中使用了 [*ElGamal*](https://link.springer.com/chapter/10.1007/3-540-39568-7_2) 加密方案的同态乘特性，以实现对用户属性的隐私保护。

| 算法 3：验证 *DU* 是否具有访问权限                           |
| ------------------------------------------------------------ |
| **Input**：$E(\vec{v})=(E(v_1),E(v_2),\dots,E(v_n))$ 用户的属性经过ElGamal加密；$\vec{x}=(x_1,x_2,\dots,x_n)$ 密文中表示向量形式的访问测了；$(\mathbb{G}_1,q_1,g_1,h)$ ElGamal 加密系统的参数；$r \in \mathbb{G}_1$ 随机元素； |
| **Output**：用户访问权限的验证结果                           |
| 1. $\bf for \ \ 1 \leq j\leq n\ \ do$ 根据 ElGamal 加密系统完成对访问策略中每个属性的加密 |
| 2.         $E(x_j)=(g_1^r\ mod\ q_1,x_jh^r\ mod\ q_1)$       |
| 3. $E(\vec{x})=(E(x_1),E(x_2),\dots,E(x_n))$ 形成向量形式    |
| 4. $result = E(v_1)\cdot E(x_1)+E(v_2)\cdot E(x_2)+\dots+E(v_n)\cdot E(x_n)$ 进行同态乘 |
| 5. 如果 $result = E(0)$，则                                  |
| 6.         $return \ \ True$                                 |
| 7. $return \ \ False$                                        |

一旦 *DU* 的 **Proof** 被验证为合法，那么就要产生一个访问事务，以记录 *DU* 的属性集合满足 *DO* 密文的访问策略，这个访问事务的创建过程用算法 4来完成。随后，就是把这个事务进行打包成块，并用一致性协议同步到其他区块链节点上。

| 算法 4：生成一个 *DU* 的访问事务                             |
| ------------------------------------------------------------ |
| **Inpu**： $A$ 代表这是访问事务；${\bf Proof} = <BPK_{DU},storeAddress,E(\vec{v}),sign>$ *DU* 的 **Proof** |
| **Output**：$Tx_{access}$ 访问事务                           |
| 1. 如果 **Proof** 合法，则                                   |
| 2.         $time$ 获取当前世界                               |
| 3.         $MD=H(A,BPK_{DU},storeAddress,time)$ 计算这个事务的数据摘要 |
| 4.         $sign= Sign_{BSK_{DU}}(MD)$ 用 *DU* 的私钥对事务进行签名 （原文中写的是 *DO* 的私钥，我觉得错了，在这里改成 *DU* 。） |
| 5.         $Tx_{access}=<A,BPK_{DU},storeAddress,time,sign>$ 访问事务 |
| 6.         $return \ \ Tx_{access}$                          |
| 7. $return \ \ \bot$                                         |

***Access Phase***

这个访问阶段主要包括两个算法。因为，通过前面的认证过程，*DU* 已经被授权可以解密访问 *DO* 的密文，于是通过：

$\bf KeyGen(PK,MSK)$ 

1. 随机选取 $m$ 个元素 $t_i \in \mathbb{Z}_N$，并且，取 $t=\sum_{t=1}^mt_i$；
2. 计算密钥：$D_0=g_p^{\omega-t},D_i=g_p^{t_i/a}$

获取到用户 *DU* 的秘钥 $SK_{\vec{v}}=<D_0,\{D_i \}_{1\leq i \leq m}$，紧接着使用获得的秘钥完成解密。

$\bf Dec(PK,CT,S,SK_{\vec{v}})$

这里需要使用到用户 *DU* 的属性集合 $S=(v_{1,j_1},v_{2,j_2},\dots,v_{n,j_n})$ 来完成整个解密过程，其中，$j_i \in \{1,\dots,n\}$。当然，只有当 $v_{i,j_i}\in W_i$ 时，表明用户满足访问策略，进而可以完成解密操作：
$$
\frac{C}{e(C_0,D_0)\cdot \prod_{i=1}^me(C_{i,j_i},D_i)}=m
$$
以上，就是这篇用区块链实现可信访问，且支持密文策略隐藏和用户属性隐私保护方案的全部过程。原文的话，可以参考前面的链接，这里我重点介绍了这个方案的实现过程，而对于安全性证明过程，作者更多的还是参考了 *Lai* 等人在 2011 年那篇文章中的工作。

在这里，对这篇文章做一个总结：


******

# CryptoABE

This repository is some implementation of ***ABE (attribute-based encryption)*** scheme. Thanks to personal abilities, this repository is just a few ABE demos. However, these demos will give some ideas about cryptography to you. For example, in our implementations, I use a special *string* to express the access policy, such as *((A,B,C,2),(D,E,F,2),2)*. In each access policy, we could regard the string as a tree and substrings are child tree. The whole string is located in the root node.

## Waters Brent's CPABE scheme 
This scheme's implementation is located in __inlude/Waters11Scheme.h__ and __source/Waters11Scheme.cpp__.

### 1. Setup Phase

The ***setup*** algorithm is used to initialize the full parameters of ***CPABE*** scheme. Thanks to ***ABE*** scheme is based on ***bilinear pairing***, at first step, we need to initialize the parameters of ***bilinear pairing***. In order to fulfill this purpose, we need the [PBC library](https://crypto.stanford.edu/pbc/) implemented by *Ben Lynn* in Stanford University. 

