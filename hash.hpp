#ifndef HAsh
#define HAsh
#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include"vector.hpp"
struct ticket{
    int ticket_num[101];
    ticket(int s, int t) {
            for (int j = 0; j < s - 1; ++j) ticket_num[j] = t;
    }
    ticket(){}
};
struct trans {
    int st_lev_time;
    int trans_arr_time;
    int price; size_t pl; int st, ed;
    char tr_id[22]; char st_name[32];
    trans() {}
    trans(const trans& other) {
        st_lev_time = other.st_lev_time;
        trans_arr_time = other.trans_arr_time;
        price = other.price; pl = other.pl;
        st = other.st; ed = other.ed;
        strcpy(tr_id, other.tr_id); strcpy(st_name, other.st_name);
    }
};
struct trans_ans{
    char tr_id1[22], tr_id2[22]; char tr_st[32];
    int lev1, arr1, lev2, arr2;
    int pr1, pr2; size_t pl1, pl2; int st1, ed1, st2, ed2;
    trans_ans& operator = (const trans_ans& other) {
        if (this == &other) return *this;
        strcpy(tr_id1, other.tr_id1); strcpy(tr_id2, other.tr_id2); strcpy(tr_st, other.tr_st);
        lev1 = other.lev1; lev2 = other.lev2; arr1 = other.arr1; arr2 = other.arr2;
        pl1 = other.pl1; pl2 = other.pl2; pr1 = other.pr1; pr2 = other.pr2;
        st1 = other.st1; st2 = other.st2; ed1 = other.ed1; ed2 = other.ed2;
        return *this;
    }
};
class find_hash{
    private:
        struct node{
            trans* v;
            node* nxt;    
            node(const trans& u) {nxt = nullptr; v = new trans(u);}
            node() {nxt = nullptr; v = nullptr;}
            ~node() {if (v != nullptr) delete v;}
        }head[499];
        int hash(const char* ch) {
            int len = strlen(ch);
            int hs = 0;
            for (int i = 0; i < len; ++i) {
                hs = (47 * hs + (unsigned char)(ch[i])) % 499;
            }
            return hs;
        }
    public:
        find_hash()
        {
           // for (int i = 0; i < 499; ++i) head[i].nxt = nullptr, head[i].v = nullptr;
            memset(this, 0, sizeof(*this));
        }
        void insert(const trans& u) {
            int o = hash(u.st_name);
            node* w = new node(u);
            w->nxt = head[o].nxt;
            head[o].nxt = w;
        }
        void find(const char* tr_id, const char* st_name,int sn, int en, int st, int ed, int lev, int arr, int pr, size_t pl, sjtu::vector<trans_ans>& ans) {
            int u = hash(st_name);
            if (head[u].nxt == nullptr) return;
            node* p = head[hash(st_name)].nxt;
            while (p != nullptr) {
                if (strcmp(p->v->st_name, st_name) == 0) {
                    if (strcmp(p->v->tr_id, tr_id) == 0) {p = p->nxt; continue;}
                    int day = p->v->trans_arr_time / 1440;
                    if (p->v->trans_arr_time % 1440 > lev % 1440) day++;
                    day -= lev / 1440;
                    if (day < st) day = st;
                    if (day > ed) {p = p->nxt; continue;}
                    trans_ans r; 
                    r.lev1 = p->v->st_lev_time; r.arr1 = p->v->trans_arr_time; 
                    r.lev2 = day * 1440 + lev; r.arr2 = day * 1440 + arr;
                    r.pr1 = p->v->price; r.pr2 = pr;
                    r.pl1 = p->v->pl; r.pl2 = pl + sizeof (ticket) * (day - st); 
                    r.st1 = p->v->st; r.ed1 = p->v->ed; r.st2 = sn; r.ed2 = en;
                    strcpy(r.tr_id1, p->v->tr_id); strcpy(r.tr_id2, tr_id); strcpy(r.tr_st, p->v->st_name);
                    ans.push_back(r);
                }
                p = p->nxt;
            }
        }
        ~find_hash() {
            for (int i = 0; i < 499; ++i) {
                node* p = head[i].nxt;
                while (p != nullptr) {
                    node* q = p->nxt;
                    delete p;
                    p = q;
                }         
            }
        }
};
#endif