#include<bits/stdc++.h>
struct ios {
    FILE *rfp, *wfp, *sfp;
    char c;
    char FILEPATH[100], NAME[100], n;
    ios() {
        c = -1; rfp = wfp = NULL;
    }
    void Open_R() {
        while(1) {
            puts("Input Graphfile:");
            scanf("%s", FILEPATH);
            strcpy(NAME, "../predata/");
            strcat(NAME, FILEPATH);
            strcat(NAME, ".bxt");
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
    int GetEdge(int &u, int &v, int &w) {
        u = read();
        if(u == -1) return -1;
        v = read(); w = read();
        return 1;
    }
    void LoadGraph(int &n, int &m, void (*adds)(int, int, int)) {
        n = 0, m = 0; int u, v, w;
        for(;~GetEdge(u, v, w);++m) {
            n = std::max(n, v);
            n = std::max(n, u);
            (*adds)(u, v, w);
        }
    }
    void Get_st(int &s, int &t) {
        strcpy(NAME, "../textdata/");
        strcat(NAME, FILEPATH);
        strcat(NAME, "_st.txt");
        if((sfp = fopen(NAME, "rb")) == NULL)
            puts("FAILOPEN!");
        fscanf(sfp, "%d%d", &s, &t);
    }
    void Close() {
        if(rfp != NULL)
            fclose(rfp);
        if(wfp != NULL)
            fclose(wfp);
    }
    void Open_W() {
        strcpy(NAME, "../results/result_");
        strcat(NAME, FILEPATH);
        strcat(NAME, ".txt");
        wfp = fopen(NAME, "wb");
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