#include<bits/stdc++.h>
#include "io.cpp"
struct nios {
    FILE *rfp, *wfp, *sfp;
    char c;
    char FILEPATH[100], NAME[100], n;
    nios() {
        c = -1; rfp = wfp = NULL;
    }
    void Open_R() {
        while(1) {
            puts("Input Graphfile:");
            scanf("%s", FILEPATH);
            strcpy(NAME, "../textdata/");
            strcat(NAME, FILEPATH);
            strcat(NAME, ".txt");
            n = strlen(FILEPATH);
            if((rfp = fopen(NAME, "rb")) == NULL)
                puts("FAILOPEN!");
            else {
                puts("DATA SUCCESSFULLY LOAD");
                break;
            }
        }
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
ios io2;
void adds(int u, int v, int w) {}
int main() {
    io.Open_R();
    int n, m;
    int time = clock();
    io.LoadGraph(n, m, adds);
    printf("normalioruntime%d\n", clock() - time);
    printf("%d %d\n", n, m);
    io.Close();
    io2.Open_R();
    time = clock();
    io2.LoadGraph(n, m, adds);
    printf("fastioruntime%d\n", clock() - time);
    printf("%d %d\n", n, m);
    io2.Close();
    return 0;
}