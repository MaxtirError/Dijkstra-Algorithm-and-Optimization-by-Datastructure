#include<bits/stdc++.h>
#include "../tool/io.cpp"
ios io;
const int N = 2e6 + 10, M = 2e6 + 10;
int pre[N], to[M], nxt[M], w[M], pr[N], tp, n, m, s, t;
long long D[N];
void add(int u, int v, int _w) {
    to[++tp] = v; 
    w[tp] = _w;
    nxt[tp] = pre[u];
    pre[u] = tp;
}
void adds(int u, int v, int w) {
    add(u, v, w);
    add(v, u, w);
}
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