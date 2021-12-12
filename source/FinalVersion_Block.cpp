#pragma optimize(2)
#include<bits/stdc++.h>
#include "../tool/io.cpp"
ios io;
const int N = 24e6 + 10, NN = 1e6 + 10, M = 58333355;
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
int D[N], cursz;
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
                        cursz = tp;
                        be[u] = ++cnt;
                        for(;st[tp + 1] != v;)
                            be[st[tp--]] = cnt;
                        cursz -= tp - 1;
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
g++ -O2 FinalVersion_Block.cpp -o FinalVersion_Block.exe -Wl,--stack=1073741824
*/