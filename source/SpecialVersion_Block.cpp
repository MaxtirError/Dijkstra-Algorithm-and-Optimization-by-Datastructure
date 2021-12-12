#pragma optimize(2)
#include<bits/stdc++.h>
#include "../tool/io.cpp"
ios io;
const int N = 24e6 + 10, M = 58333355, NT = 6e7, HSZ = 5e4;
int pr[N], n, m, Nt, s, t;
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
int main() {
    io.Open_R();
    io.LoadGraph(n, m, adds);
    printf("graphsize:%d %d\n", n, m);
    puts("INPUT START AND END:");
    io.Get_st(s, t);
    int time = clock();
    int d = Dij(s, t);
    printf("run %dms time\n", clock() - time);
    Output(d);
    io.Close();
    system("pause");
    return 0;
}
/*
g++ -O2 FastVersion_Heap.cpp -o FastVersion_Heap.exe 
*/