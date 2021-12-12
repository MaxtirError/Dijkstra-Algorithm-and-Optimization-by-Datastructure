#pragma optimize(2)
#include<bits/stdc++.h>
#include "../tool/io.cpp"
ios io;
const int N = 24e6 + 10, NN = 2e6 + 10, M = 58333355;
int pr[N], n, m, s, t;
struct Edge {
    int to, w, nxt;
}Pool1[M], Pool2[M];
int tp = 1; bool flag = true;
Edge *GetEdge(int tp) {
    if(tp < M)
        return Pool1 + tp;
    else return Pool2 + tp - M;
}
int pre[N];
void add(int u, int v, int w) {
    Edge *e = GetEdge(++tp);
    e->to = v; e->w = w; e->nxt = pre[u];
    pre[u] = tp;
}
void adds(int u, int v, int w) {
    add(u, v, w);
    add(v, u, w);
}
int D[N];
struct Data {
    int u, d;
    Data(int _u = 0, int _d = 0) : u(_u), d(_d) {}
};
bool operator < (Data a, Data b) {return a.d < b.d;}
struct Heap {
    int sz, curlen;
    Data heap[NN];
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
int dfn[N], st[N], cnt, be[N], tm, nx, d;
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
void Output(int d) {
    io.Open_W();
    io.Print(d);
    io.Print('\n');
    if(d == -1)
        return ;
    for(int v = t;v != s; v = pr[v]) {
        io.Print(v);
        io.Print("<-");
    }
    io.Print(s);
    io.Print('\n');
}
int Solve(int s, int t) {
    nx = t;
    tp = 0;
    int nouse;
    Dfs(s, 0, nouse);
    return d;
}
int main() {
    io.Open_R();
    io.LoadGraph(n, m, adds);
    printf("graphsize:%d %d\n", n, m);
    io.Get_st(s, t);
    int time = clock();
    int d = Solve(s, t);
    printf("run %dms time\n", clock() - time);
    Output(d);
    io.Close();
    system("pause");
    return 0;
}
/*
g++ -O2 FinalVersion.cpp -o FinalVersion.exe -Wl,--stack=1073741824
*/