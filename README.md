# Dijkstra Algorithm and Optimization by Datastructure

big homework for datasturcture, optimize the dijstrak algorithm use different datastructure.

## 项目环境

运行环境:windows10,g++

开发环境：vscode

库依赖：c++标准库

源文件编译指令，在目录下

~~~cmd
g++ -O2 file.cpp -o file.exe -Wl,--stack=1073741824
~~~

## 问题描述

经典的最短路问题，经典的Dijstrak算法。

## 项目框架

>Dijkstra:.
>│   README.md
>│
>├───feedback
>│       log.xlsx #一些运行时间数据的对比
>│
>├───predata # 预处理的二进制文件，用于优化读入
>│       textinfo.bxt
>│
>├───reference_file # 作业要求文件
>│       Lab3_DS_2021Aut.pdf
>│
>├───results # 结果文件，输出最短路
>│       result_textinfo.txt
>│
>├───source #不同版本的Dijstrak算法
>│       EasyVersion.cpp
>│       FastVersion_Heap.cpp
>│       FastVersion_zkw.cpp
>│       FinalVersion.cpp
>│       FinalVersion_Block.cpp
>│       SpecialVersion_Block.cpp
>│
>├───textdata #初始的图文件和源汇文件
>│       textinfo.txt
>│       textinfo_st.txt
>│
>└───tool #一些工具
>        generate_data.cpp #用于产生数据
>        gettxt.cpp #用于压缩二进制文件
>        io.cpp #用于读入
>        testio.cpp # 用于测试读入效率

## 开始的开始：数据处理与读入优化

首先，数据的格式是每行三个数据，$u,v,w$

### fread读入优化

我们可以采用$fread$处理数据流，并按char读入数据。通过读到一个buf流和指针流动来实现快速的读入优化

~~~c++
struct nios {
    FILE *rfp, *wfp, *sfp;
    char c;
    char FILEPATH[100], NAME[100], n;
    nios() {
        c = -1; rfp = wfp = NULL;
    }
    void Open_R() { //openfile
        ...
    }
    inline char getchar() {
        static const int IN_LEN=1<<18|1;
        static char buf[IN_LEN], *s = buf, *t = buf;
        return (s==t) && (t = (s = buf) + fread(buf, 1, IN_LEN, rfp)),s==t?-1:*s++;
    }

    int read() {
        char ch = getchar();int x = 0;
        if(ch == -1) return -1;
        for(;ch < '0' || ch > '9'; ch = getchar()) 
            if(ch == -1)
                return -1;
        for(;ch >= '0' && ch <= '9'; ch = getchar())
            x = x * 10 + ch - '0';
        return x;
    }
    int GetEdge(int &u, int &v, int &w) {
        u = read();
        if(u == -1) return -1;
        v = read(); w = read();
        return 1;
    }
    void LoadGraph(int &n, int &m, void (*adds)(int, int, int)) {
        n = 0, m = 0; int u, v, w;
        for(;~GetEdge(u, v, w);++m) {
            n = std::max(n, u);
            n = std::max(n, v);
            adds(u, v, w);
        }
    }
    void Close() {
        if(rfp != NULL)
            fclose(rfp);
        if(wfp != NULL)
            fclose(wfp);
    }
}io;
//source:testio
~~~

### 二进制压缩文件

而在此基础上，由于数据量庞大，我们可以尝试使用二进制文件压缩。由于一个char是一个字节，然而事实上，我们只需要0~9和分隔符就可以得到整张图的信息，因此可以用一个字节存储两位的信息。将0到9和用0xa标志的分隔符压缩整个图的文件。

~~~c++
struct ios {
    FILE *rfp, *wfp;
    char num;
    ios() {rfp = wfp = NULL; num = -1;}
    void Open() {...}
    inline char getchar() {...}
    bool rewrite() {
        char c = getchar();
        if(!~c) {
            if(~num)
                num = num << 4 | 0xa,
                fwrite(&num, 1, 1, wfp);
            return false;
        }
        if(c >= '0' && c <= '9')
            c -= '0';
        else c = 10;
        if(!~num)
            num = c;
        else {
            num = num << 4 | c;
            fwrite(&num, 1, 1, wfp);
            num = -1;
        }
        return true;
    }
    void Work() {}
    void Close() { }
}io;
//source:gettxt.cpp
~~~

使用方式是：

~~~cmd
>gettxt.exe
>INPUTFILE:
textinfo文件名（无后缀）
DATA SUCCESSFULLY LOAD
~~~

注意文件的格式必须为txt，并放在textdata目录下，最后会生成一个.bxt文件在predata文件夹下。

而相应地也需要修改读入的方式，每次读取的是两个数据。

~~~c++

struct ios {
    FILE *rfp, *wfp, *sfp;
    char c;
    char FILEPATH[100], NAME[100], n;
    ios() {
        c = -1; rfp = wfp = NULL;
    }
    void Open_R() {...}
    inline char getchar() {...}

    int read() {
        int x = c & 0xf;
        if(x >= 10) x = -1;
        c = getchar();
        if(!~c) return x;
        if(~x && ((c >> 4 & 0xf) == 0xa))
            return x;
        else {
            for(;c == 0xaa;) c = getchar();
            if(!~c)
                return -1;
        }
        if(!~x) x = 0;
        for(;1;) {
            if((c >> 4 & 0xf) == 0xa)
                break;
            x = (x << 1) + (x << 3) + (c >> 4 & 0xf);
            if((c & 0xf) == 0xa)
                break;
            x = (x << 1) + (x << 3) + (c & 0xf);
            c = getchar();
            if(!~c) break;
        }
        return x;
    }
    int GetEdge(int &u, int &v, int &w) {...}
    void LoadGraph(int &n, int &m, void (*adds)(int, int, int)) {...}
    void Get_st(int &s, int &t) {//获取源汇文件
    ...
    }
    void Close() {
        if(rfp != NULL)
            fclose(rfp);
        if(wfp != NULL)
            fclose(wfp);
    }
    void Open_W() {
    ...
    }
    template<typename T>void Print(T x) {
        static char st[20];
        static int tp;
        tp = 0;
        if(x == -1)
            return Print("-1");
        for(;x; x /= 10)
            st[++tp] = x % 10 + '0';
        for(int i = tp; i; --i)
            fwrite(st + i, 1, 1, wfp);
    }
    void Print(const char *s) {
        fwrite(s, 1, strlen(s), wfp);
    }
    void Print(const char c) {
        fwrite(&c, 1, 1, wfp);
    }
};
//soucrce:io.cpp
~~~

顺带地，采用了fwrite快速输出答案。全部放在io工具类中，在别的文件中include

### 读入效率对比

| freadio           | bitstreamio       |
| ----------------- | ----------------- |
| 7081ms            | 5790ms            |
| SIZEOFN：23947347 | SIZEOFM：58333344 |
|                   |                   |

可以看到二进制压缩提升了1秒多的读入时长。

## 最初的起点：朴素的Dijstrak算法

主要的算法流程为，每轮寻找到当前距离原点最近的节点，将这个距离作为这个节点到原点的最短路径，而后用这个节点根据三角不等式更新其他节点的答案。

复杂度分析，每轮都要寻找最短路径的节点，最坏情况下搜索n次，所谓为$O(n^2+m)$

~~~c++
long long Dij(int s, int t) {
    if(s == t) return 0LL;
    D[s] = 0;
    for(int i = 1;i <= n; ++i) {
        int mn = 0; long long d;
        if(D[s] == 0) {
            mn = s;
            d = 0;
            D[s] = -1;
        }
        else {
            for(int j = 1;j <= n; ++j)
                if(D[j] > 0 && (!mn || D[mn] > D[j]))
                    mn = j;
            if(mn == t) return D[t];
            d = D[mn]; D[mn] = -1;
        }
        for(int i = pre[mn]; i; i = nxt[i])
            if(D[to[i]] != -1 && (!D[to[i]] || D[to[i]] > d + w[i]))
                D[to[i]] = d + w[i], pr[to[i]] = mn;
    }
    return -1;
}
//Source: EasyVersion.cpp
~~~

## 数据结构优化：zkw线段树和堆

我们主要的优化着力点在于寻找最小的到原点距离的节点。实际上就是要寻找某种数据结构可以维护某个区间，支持单点修改，删除和全局最小值的查询。

### zkw线段树优化

线段树是一种基于分治的数据结构，其基本思想是将某个区间分成均等的两半，当前节点维护当前区间的信息，而左右子节点维护左右子区间的信息，依次递归形成一棵树形结构，查询的时候自顶向下查询，更新的时候自底向上更新。由于树的层数不超过$O(logn)$,更新的复杂度是$O(logn)$,查询的复杂度是$O(1)$，总复杂度为$O(mlogn+n)$

zkw线段树则是线段树的一种基于位操作的实现技巧。基本思想是直接构造一棵完全二叉树，维护$[0,2^n-1]$大小的区间，这样的好处是父子节点的编号关系为2倍或者2倍+1，可以快速寻址并且更新，而不需要递归。

~~~c++
int Be(int x, int y) {
    return D[x] < D[y] ? x : y;
}
void Up(int i, int v) {
    for(T[i += Nt] = v; i >>= 1; T[i] = Be(T[i << 1], T[i << 1 | 1])) ;
}
int Dij(int s, int t) {
    Nt = 1;
    for(;Nt <= n;)
        Nt <<= 1;
    D[0] = 1e9;
    D[s] = 0; Up(s, s);
    for(;T[1];) {
        int u = T[1]; Up(u, 0);
        if(u == t) return D[t];
        static int v, w; Edge *e;
        for(int i = pre[u]; i; i = e->nxt) {
            e = GetEdge(i); v = e->to; w = e->w;
            if(!D[v] || D[v] > D[u] + w)
                D[v] = D[u] + w, Up(v, v), pr[v] = u;
        }
    }
    return -1;
}
//Source:FastVersion_zkw.cpp
~~~

### 堆优化

堆是另一种可以维护上述操作的数据结构。其本质仍是用一棵完全二叉树维护所有的数据，性质在于父亲的序优先于儿子。

插入节点在结尾插入，并且一旦不满足序关系则交换父子。

删除节点弹出堆顶节点将堆最末尾的节点放到堆顶，若不满足序关系，和左右儿子中序优先的那个节点交换并如此递归。

虽然堆是完全二叉树，但是需要注意堆优化中可能有点的重复，所以复杂度和线段树优化略有不同，是$O(mlogm+n)$的。

~~~c++
struct Data {
    int u, d;
    Data(int _u = 0, int _d = 0) : u(_u), d(_d) {}
};
bool operator < (Data a, Data b) {return a.d < b.d;}
struct Heap {
    int sz, curlen;
    Data heap[N];
    bool Empty() {return !sz;}
    Data Pop() {
        Data u = heap[1], l = heap[sz--];
        int ch = 2, cur = 1;
        for(;ch <= sz;) {
            if(ch < sz && heap[ch + 1] < heap[ch])
                ++ch;
            if(l < heap[ch])
                break;
            heap[cur] = heap[ch];
            cur = ch;
            ch <<= 1;
        }
        heap[cur] = l;
        return u;
    }
    void Push(Data x) {
        int u = ++sz;
        for(;u != 1 && x < heap[u >> 1];)
            heap[u] = heap[u >> 1], u >>= 1;
        heap[u] = x;
    }
}Q;
int Dij(int s, int t) {
    D[0] = 1e9;
    D[s] = 0; Q.Push(Data(s, 0));
    for(;!Q.Empty();) {
        Data x = Q.Pop(); 
        if(D[x.u] == -1)
            continue;
        int d = D[x.u];
        if(x.u == t) return D[t];
        D[x.u] = -1;
        static int v, w; Edge *e;
        for(int i = pre[x.u]; i; i = e->nxt) {
            e = GetEdge(i); v = e->to; w = e->w;
            if(!D[v] || D[v] > d + w)
                D[v] = d + w, Q.Push(Data(v, D[v])), pr[v] = x.u;
        }
    }
    return -1;
}
//Source:FastVersion_Heap.cpp
~~~

## 基于点复杂度的优化：分块

首先思考一个问题，线段树和堆优化的复杂度真的完全优于朴素算法吗？

可以想想，当图为完全图的情况下，也就是$O(m)=O(n^2)$时，我们可以发现，线段树和堆优化的复杂度退化为$O(n^2logn)$。

瓶颈在于，线段树和堆优化扩大了边更新的复杂度开销。

事实上，我们可以尝试另一种优化方式，将点分成$\sqrt n$个块，记录每个块到源最小距离的点，这样的好处是更新的复杂度是$O(1)$的，而查询的复杂度是$O(\sqrt n)$的，那么总复杂度就是$O(n\sqrt n+m)$的，在稠密图的情况下理论效率高于堆优化和线段树优化。

~~~c++

struct Data {
    int u, d;
    Data(int _u = 0, int _d = 0) : u(_u), d(_d) {}
};
int bsz, mn[HSZ];
int Be(int x, int y) {return D[x] < D[y] ? x : y;}
void Up(int s) {
    int id = (s - 1) / bsz;
    mn[id] = Be(mn[id], s);
}
int Dij(int s, int t) { 
    bsz = sqrt(n) + 1;
    D[0] = 1e9;
    D[s] = 0; 
    Up(s);
    for(int i = 1;i <= n; ++i) {
        int u = 0;
        for(int j = 0;j * bsz + 1 <= n; ++j)
            u = Be(u, mn[j]);
        if(u == t) return D[t];
        int d = D[u]; D[u] = -1; 
        int id = (u - 1) / bsz, l = id * bsz + 1, r = std::min(n, (id + 1) * bsz);
        mn[id] = 0;
        for(int j = l;j <= r; ++j)
            if(D[j] != -1 && D[j])
                mn[id] = Be(mn[id], j);
        static int v, w; Edge *e;
        for(int i = pre[u]; i; i = e->nxt) {
            e = GetEdge(i); v = e->to; w = e->w;
            if(!D[v] || D[v] > d + w)
                D[v] = d + w, Up(v), pr[v] = u;
        }
    }
    return -1;
}
//source:SpecialVersion_Block.cpp
~~~

## 进阶探索：Dijstrak算法基于连通性的优化

到此为止，Dijstrak的复杂度上限的套论事实上已经基本结束。然而，在一些特殊情况下的图，Dijstrak仍有优化的空间。

分块算法解决了稠密图的情况下Dijstrak算法的缺陷。然而，当图较为稀疏时，例如，图退化为一棵树的时候，logn的复杂度开销其实是不必要的。

### 点双联通分量算法

事实上，从连通性的角度考虑，树的特殊性在于，所有点都是图的割点。

而我们可以想象，从源点走到汇点，实际上有一些点是必须经过的。我们对图运用Tarjan的点双联通分量算法，所有的点双联通分量事实上形成了一个树形结构。（从广义圆方树的角度理解，实际上就是圆方树上的路径）。那么我们实际上只需要通过s，t所在点双之间的树上路径上的所有点双联通分量上的割点，这样就可以吧最短路径转化为各个点双联通分量内部的问题。

那么在此基础上，我们设点双连通分量的节点为$S_i$，复杂度就可优化为$O(\sum Dijstrak(|S_i|,|E[S_i]|))$，其中$Dijstrak(n,m)$表示应用n个点m条边的点双连通分量算法。

这样，在不改变算法总体复杂度的前提下，我们优化了Dijstrak算法的下限。在树的情况下，每个点双连通分量大小都为2，复杂度退化为线性的。

并且，根据不同图的类型，我们还可以对每个点双连通分量应用不同的算法。也就是选择堆优化或者分块优化。

除了树以外，还可以考虑这样的真实情景：在不同大城市之间的交通链路，不同大城市之间可能只有唯一的一条交通方式，例如飞机。而大城市内部则可能有很多不同的交通方式，也就是好几个稠密图通过几条边连接成一个树-稠密图的结构。这种情况下用分块优化Dijstrak在理论上是较为优秀的。

两种算法的代码如下：

~~~c++

struct Block {
    int bsz, tot, mxb, mn[NN], ps[NN], id[N];
    void init(int s, int t) {
        bsz = sqrt(cursz) + 1;
        id[s] = id[t] = 0;
        tot = 0; mxb = -1;
    }
    int Be(int x, int y) {
        return D[ps[x]] < D[ps[y]] ? x : y;
    }
    void Up(int s) {
        if(!id[s])
            id[s] = ++tot, ps[tot] = s;
        int bid = (id[s] - 1) / bsz;
        if(mxb < bid)
            mxb = bid, mn[bid] = id[s];
        else
            mn[bid] = Be(mn[bid], id[s]);
    }
    int Getmn() {
        int u = 0;
        for(int i = 0;i <= mxb; ++i)
            u = Be(u, mn[i]);
        return ps[u];
    }
    void Pop(int u) {
        if(!id[u])
            printf("%d %d %d %d %d\n", u, cursz, tot, bsz, mxb);
        assert(id[u]);
        int bid = (id[u] - 1) / bsz;
        int l = bid * bsz + 1, r = std::min(tot, (bid + 1) * bsz);
        mn[bid] = 0;
        for(int j = l;j <= r; ++j)
            if(D[ps[j]] != -1 && D[ps[j]])
                mn[bid] = Be(mn[bid], j);
    }
}B;
int dfn[N], st[NN], cnt, be[N], tm, nx, d;
int Dij(int s, int t) {
    if(s == t) return 0;
    assert(be[s] == be[t]);
    static int v, w; Edge *e;
    if(cursz == 2) {
        D[t] = D[s] = 0;
        for(int i = pre[s]; i; i = e->nxt) {
            e = GetEdge(i); v = e->to; w = e->w;
            if(be[v] != be[s])
                continue;
            assert(t == v);
            if(!D[v] || D[v] > w)
                D[v] = w, pr[v] = s;
        }
        return D[t];
    }
    B.init(t, s);
    D[0] = 1e9;
    D[t] = D[s] = 0;
    B.Up(s);
    for(int i = 1;i <= n; ++i) {
        int u = B.Getmn();
        if(u == t) return D[t];
        int d = D[u]; D[u] = -1; 
        B.Pop(u);
        static int v, w; Edge *e;
        for(int i = pre[u]; i; i = e->nxt) {
            e = GetEdge(i); v = e->to; w = e->w;
            if(!D[v] || D[v] > d + w)
                D[v] = d + w, B.Up(v), pr[v] = u;
        }
    }
    return -1;
}
//source:FinalVersion_Block.cpp
~~~

~~~c++

int Dij(int s, int t) {
    if(s == t) return 0;
    assert(be[s] == be[t]);
    for(;!Q.Empty();)
        Q.Pop();
    D[0] = 1e9;
    D[t] = D[s] = 0; Q.Push(Data(s, 0));
    for(;!Q.Empty();) {
        Data x = Q.Pop(); 
        if(D[x.u] == -1)
            continue;
        int d = D[x.u];
        if(x.u == t) return D[t];
        D[x.u] = -1;
        static int v, w; Edge *e;
        for(int i = pre[x.u]; i; i = e->nxt) {
            e = GetEdge(i); v = e->to; w = e->w;
            if(be[v] != be[x.u])
                continue;
            if(!D[v] || D[v] > d + w)
                D[v] = d + w, Q.Push(Data(v, D[v])), pr[v] = x.u;
        }
    }
    return -1;
}
bool Dfs(int u, int fa, int &lowu) {
    lowu = dfn[u] = ++tm; 
    st[++tp] = u;
    bool flag = false;
    int v, w, lowv; Edge *e;
    for(int i = pre[u]; i; i = e->nxt) {
        e = GetEdge(i);
        if(i != (fa ^ 1)) {
            v = e->to; w = e->w;
            if(!dfn[v]) {
                if(Dfs(v, i, lowv)) {
                    flag = true;
                    if(lowv >= dfn[u]) {
                        be[u] = ++cnt;
                        for(;st[tp + 1] != v;)
                            be[st[tp--]] = cnt;
                        d += Dij(u, nx);
                        nx = u;
                    }
                }
                if(lowv >= dfn[u]) {
                    for(;st[tp + 1] != v;)
                        be[st[tp--]] = 0;
                }
                assert(lowv != 0);
                lowu = std::min(lowu, lowv);
            }
            else {
                assert(dfn[v] != 0);
                lowu = std::min(lowu, dfn[v]);
            }
        }
    }
    return u == t | flag;
}
//Source:FinalVersion.cpp
~~~

## 效率对比

### 产生数据

通过generate_data文件，可以产生不同的数据集。使用方法如下

~~~cmd
>generate_data.exe
>INPUTDATANAME:
text(文件名)
MODE:
0/1/2(模式)
~~~

0/1/2模式分别代表了tree，稠密图以及稠密-树的数据格式。执行完毕后会在textdata文件下产生相应的.txt和_st.txt文件。之后使用gettxt.exe压缩即可产生不同的数据。

### 运行结果

运行之后的结果如下

| 堆优化  | 线段树优化 | 块优化 | 点双块 | 点双堆  | 图类型   |
| ------- | ---------- | ------ | ------ | ------- | -------- |
| 13974ms | 20187ms    | *      | *      | 21706ms | 超大图   |
| 4260ms  | 19515ms    | *      | 370ms  | 369ms   | 树大图   |
| 31ms    | 38ms       | 154ms  | 200ms  | 100ms   | 中图     |
| 902ms   | 200ms      | 216ms  | 3966ms | 3310ms  | 稠密图   |
| 2012ms  | 1871ms     | *      | 1034ms | 1087ms  | 树稠密图 |

### 数据分析

在此环境下，我们得到了一下几点结论

#### 堆优化的优越性

从综合能力上来看，堆优化能适用于在大多数情况。虽然在边数较多的情况下理论上其复杂度高于线段树优化，然而实际环境下其效率比线段树优化高得多。

得到这一结论的分析是，由于堆优化的实现方式是动态加点删点，在随机的情况下堆实际上是不满的，由于只有在更新的条件才会把点插入堆中，只有在边权相差较小的情况下才会跑满$O(logn)$，其余情况下基本是上类线性的。

#### 点双堆下限的有效优化

可以看到，在树和树稠密图类型的图中，点双联通算法较为有效地优化了复杂度的下限。尤其是在树的算法中。

#### 分块优化的缺陷

事实上，除了分块优化，其余的算法基本上都展现出了应用的理论复杂度。然而分块优化的尴尬点在于，在稠密图的情况下，实际上Dijstrak算法的更新次数较少，只有在一些边权差距不大的图中才会有多次更新，而且在点数较小的情况下且更新次数较多的情况下，线段树优化的优越性逐渐展现了出来，常数较小的$logn$超越了分块算法。因此分块算法尽管理论上在稠密图中由于其余数据结构，在实际环境下的效果远不如堆优化和线段树优化，这一点在点双的堆和块中也可以看出来，这两者的复杂度实际上相差不大。

不过当面对更加庞大的图的情况，分块优化在理论上的复杂度优化仍不容忽视，而且也不排除作者本身随机构造数据缺陷，其真正的价值仍然有待研究。

