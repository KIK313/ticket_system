#ifndef _TIme
#define _TIme

#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>

inline int getday(const char ch[]) {
    int d = (ch[3]-'0') * 10 + (ch[4]-'0') - 1;
    if (ch[1]-'0' > 6) d += 30;
    if (ch[1]-'0' > 7) d += 31;
    return d;
}

#endif