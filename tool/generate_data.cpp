#include<bits/stdc++.h>
char FILENAME[100], NAME[100];
FILE *wfp;
int s, t;
int Rand() {
    return (rand() << 16) | rand();
}
int Rand(int L, int R) {
    return Rand() % (R - L + 1) + L;
}

void Print(const char *s) {
    fwrite(s, 1, strlen(s), wfp);
}
void Print(const char c) {
    fwrite(&c, 1, 1, wfp);
}
void Print(int x) {
    static char st[20];
    static int tp;
    tp = 0;
    if(x == -1)
        return Print("-1");
    for(;x; x /= 10)
        st[++tp] = x % 10 + '0';
    if(!tp)
        st[tp = 1] = '0';
    for(int i = tp; i; --i)
        fwrite(st + i, 1, 1, wfp);
}
void Print(int x, int y, int z) {
    Print(x); Print(' '); Print(y); Print(' '); Print(z);
    Print('\n');
}
void Tree() {
    int n = 1e7, m = 1e6; int j = 2;
    int W = 1e3;
    for(int i = 1;i <= n && j <= n - m; ++i) {
        int d = Rand(20, 30);
        for(int k = 1;k <= d && j <= n - m; ++k, ++j)
            Print(i, j, Rand() % W);
    }
    for(int k = j;k <= n; ++k)
        Print(k - 1, k, Rand() % W);
    s = 1; t = n;
}
void Smalln() {
    int n = 1e5, m = 1e7;
    int W = 1e5;
    for(int i = 1;i <= m; ++i) {
        int u = Rand(1, n), v;
        for(v = Rand(1, n);v == u;v = Rand(1, n)) ;
        Print(u, v, Rand(1, 2));
    }
    s = 1; t = n;
}
void MapLike() {
    int n = 2e6, m = 1e5, B = 1e4;
    int W = 1e3;
    for(int i = 1;i <= n; i += B) {
        for(int j = 1;j <= m; ++j) {
            int u = Rand(i, i + B - 1), v = Rand(i, i + B - 1);
            for(;u == v; v = Rand(i, i + B - 1)) ;
            Print(u, v, Rand() % W);
        }
        Print(i, i + B, Rand() % W);
    }
    s = 1; t = n;
}
void GENERAL() {
    int n = 2e4, m = 1e5;
    int W = 1e3;
    for(int i = 1;i <= m; i++) {
        int u = Rand(1, n), v = Rand(1, n);
        for(;u == v; v = Rand(1, n)) ;
        Print(u, v, Rand() % W);
    }
    s = 1; t = n;
}
int main() {
    srand(time(0));
    puts("INPUTDATANAME:");
    scanf("%s", FILENAME);
    strcpy(NAME, "../textdata/");
    strcat(NAME, FILENAME);
    strcat(NAME, ".txt");
    wfp = fopen(NAME, "wb");
    puts("MODE:");
    int k;
    scanf("%d", &k);
    switch (k)
    {
        case 0:Tree();
            break;
        case 1:Smalln();break;
        case 2:MapLike();break;
        default:GENERAL();
            break;
    }
    
    fclose(wfp);
    strcpy(NAME, "../textdata/");
    strcat(NAME, FILENAME);
    strcat(NAME, "_st.txt");
    wfp = fopen(NAME, "w");
    fprintf(wfp, "%d %d\n", s, t);
    fclose(wfp);
    return 0;
}