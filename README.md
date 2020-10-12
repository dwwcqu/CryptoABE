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
2. 根据选定的安全参数，生成整个系统需要使用的配对代数结构及其参数；生成阶为 $p$ 的群 $\mathbb{G}$，确定一个群 $\mathbb{G}$ 上的一个生成元 $g$。同时，为系统的 $U$ 个属性分别分配一个 $\mathbb{G}$ 上的元素：$ h_1, h_2, ..., h_U \in \mathbb{G}$。
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

![IBE 方案](https://github.com/dwwcqu/CryptoABE/images/ibe.jpg)

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

1. ***Setup***. $(pp, mk) \leftarrow setup(1^{\lambda})$，主要用来生成公钥/主秘钥对；
2. ***Keygen***. $sk \leftarrow keygen(mk,k)$，根据 $k\in K$ 和系统的主秘钥来生成 $k$ 对应的私钥；
3. ***Encrypt***. $c\leftarrow enc(pp,x)$，加密消息 $x\in X$，获得密文；
4. ***Decrypt***. $y \leftarrow dec(sk,c)$，使用 $sk$ 从密文 $c$ 中计算 $F(k,x)$。

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

******

# CryptoABE

This repository is some implementation of ***ABE (attribute-based encryption)*** scheme. Thanks to personal abilities, this repository is just a few ABE demos. However, these demos will give some ideas about cryptography to you. For example, in our implementations, I use a special *string* to express the access policy, such as *((A,B,C,2),(D,E,F,2),2)*. In each access policy, we could regard the string as a tree and substrings are child tree. The whole string is located in the root node.

## Waters Brent's CPABE scheme 
This scheme's implementation is located in __inlude/Waters11Scheme.h__ and __source/Waters11Scheme.cpp__.

### 1. Setup Phase

The ***setup*** algorithm is used to initialize the full parameters of ***CPABE*** scheme. Thanks to ***ABE*** scheme is based on ***bilinear pairing***, at first step, we need to initialize the parameters of ***bilinear pairing***. In order to fulfill this purpose, we need the [PBC library](https://crypto.stanford.edu/pbc/) implemented by *Ben Lynn* in Stanford University. 