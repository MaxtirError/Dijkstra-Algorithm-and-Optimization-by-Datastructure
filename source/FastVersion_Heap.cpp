#pragma optimize(2)
#include<bits/stdc++.h>
#include "../tool/io.cpp"
ios io;
const int N = 24e6 + 10, M = 58333355, NT = 6e7, HSZ = 1e5;
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
g++ -O2 FastVersion_Heap.cpp -o FastVersion_Heap.exe 
*/