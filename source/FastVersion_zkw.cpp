#pragma optimize(2)
#include<bits/stdc++.h>
#include "../tool/io.cpp"
ios io;
const int N = 24e6 + 10, M = 58333355, NT = 6e7;
int pr[N], n, m, Nt, s, t;
int T[NT];
struct Edge {
    int to, w, nxt;
}Pool1[M], Pool2[M];
int tp; bool flag = true;
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
void Output(long long d) {
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
int main() {
    io.Open_R();
    io.LoadGraph(n, m, adds);
    printf("graphsize:%d %d\n", n, m);
    io.Get_st(s, t);
    int time = clock();
    long long d = Dij(s, t);
    printf("run %dms time\n", clock() - time);
    Output(d);
    io.Close();
    system("pause");
    return 0;
}
/*
g++ -O2 FastVersion_zkw.cpp -o FastVersion_zkw.exe 
*/