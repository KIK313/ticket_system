#ifndef _TIme
#define _TIme

#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>

inline int getday(const char ch[]) {
    int d = (ch[3]-'0') * 10 + (ch[4]-'0') - 1;
    if (ch[1] - '0' < 6) return -1;
    if (ch[1]-'0' > 6) d += 30;
    if (ch[1]-'0' > 7) d += 31;
    if (ch[1]-'0' > 8) d += 31;
    if (ch[1] - '0' > 9) return -1; 
    return d;
}
void prt(int t) {
    int d = t/1440;
    t = t % 1440;
    int month = 6;
    d++;
    if (d > 30) {
        d -= 30; month++;
        if (d > 31) d -= 31,month++;
        if (d > 31) d -= 31,month++;
    }
    printf("%02d-%02d %02d:%02d ", month, d, t/60, t%60);
}
#endif