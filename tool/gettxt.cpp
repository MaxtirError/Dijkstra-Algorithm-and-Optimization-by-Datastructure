#include<bits/stdc++.h>
struct ios {
    FILE *rfp, *wfp;
    char num;
    ios() {rfp = wfp = NULL; num = -1;}
    void Open() {
        char FILEPATH[100], PRESSPATH[100], NAME[100], n;
        while(1) {
            puts("INPUTFILE:");
            scanf("%s", FILEPATH);
            n = strlen(FILEPATH);
            strcpy(NAME, "../textdata/");
            strcat(NAME, FILEPATH);
            strcat(NAME, ".txt");
            if((rfp = fopen(NAME, "r")) == NULL)
                puts("FAILOPEN!");
            else {
                puts("DATA SUCCESSFULLY LOAD");
                break;
            }
        }
        strcpy(NAME, "../predata/");
        strcat(NAME, FILEPATH);
        strcat(NAME, ".bxt");
        wfp = fopen(NAME, "wb");
    }
    inline char getchar() {
        static const int IN_LEN=1<<18|1;
        static char buf[IN_LEN], *s = buf, *t = buf;
        return (s==t) && (t = (s = buf) + fread(buf, 1, IN_LEN, rfp)),s==t?-1:*s++;
    }
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
    void Work() {
        for(;rewrite();) ;
    }
    void Close() {
        fclose(rfp);
        fclose(wfp);
    }
}io;
int main() {
    io.Open();
    io.Work();
    io.Close();
    return 0;
}