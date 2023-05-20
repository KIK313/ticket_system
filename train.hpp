#ifndef train_control
#define train_control
#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<fstream>
#include"simple_file_LRU.hpp"
#include"bpt.hpp"
#include"time.hpp"
#include"new_bpt.hpp"
#include"vector.hpp"
using namespace std;
;
#pragma pack(push,1)
struct train {
    char train_id[22];
    int station_num;
    char name[102][32];
    int seat_num;
    int arr_time[101];
    int lev_time[101];
    int price[101];
    char type;
    int start_day; // 0 -> 91;
    int ed_day; // 0 -> 91;
    int start_min; // 0 -> 24*3600
};
struct ticket{
    int ticket_num[101];
    ticket(int s, int t) {
            for (int j = 0; j < s - 1; ++j) ticket_num[j] = t;
    }
    ticket(){}
};
struct pass_tr{
    size_t tic_add;
    int arr, lev, p, st, ed, st_num;
    char tr_id[22];
    pass_tr() {}
    bool operator < (const pass_tr& other) const {
        return tic_add < other.tic_add;
    }
    bool operator > (const pass_tr& other) const {
        return tic_add > other.tic_add;
    }
    bool operator == (const pass_tr& other) const {
        return tic_add == other.tic_add;
    }
};
#pragma pack(pop)
struct qr{
    char st[32],ed[32];
    int st_day; bool is_time; 
};
struct train_id {
    char id[22];
    public:
    train_id() {}
    train_id(const char* s) {
        strcpy(id,s);
    }
    bool operator < (const train_id& other) const {
        return (strcmp(id,other.id)<0);
    }
    bool operator < (const char* s) const {
        return (strcmp(id,s)<0);
    }
    bool operator > (const char* s) const {
        return (strcmp(id,s)>0);
    }
    bool operator > (const train_id& other) const {
        return (strcmp(id,other.id)>0);
    }
    bool operator == (const train_id& other) const {
        return (strcmp(id, other.id) == 0);
    }
};
struct qr_ticket{
    char tr_id[22];
    int price; int st; int ed; int seat_num;

};
struct station_id {
    char id[32];
    public:
    station_id() {}
    station_id(const char* s) {
        strcpy(id,s);
    }
    bool operator < (const station_id& other) const {
        return (strcmp(id,other.id)<0);
    }
    bool operator < (const char* s) const {
        return (strcmp(id,s)<0);
    }
    bool operator > (const char* s) const {
        return (strcmp(id,s)>0);
    }
    bool operator > (const station_id& other) const {
        return (strcmp(id,other.id)>0);
    }
    bool operator == (const station_id& other) const {
        return (strcmp(id, other.id) == 0);
    }
};
class trains {
private:
    file<train> train_inf;
    file<ticket> ticket_inf;
    BPT<train_id, size_t> nors_id_addr;
    BPT<train_id, std::pair<size_t, size_t> > rs_id_addr;
    new_BPT<station_id, pass_tr> pass_st_tr;
    void print(const char* ch) {
        int len = strlen(ch);
        for (int i = 0; i < len; ++i) printf("%c",ch[i]);
        printf(" ");
    }
public:
    trains(char c1[], char c2[], char c3[], char c4[], char c5[]) :
        train_inf(c1, 100),ticket_inf(c2, 20),
        nors_id_addr(c3),rs_id_addr(c4), pass_st_tr(c5) {
    }
    void add_train(const train& tr) {
        std::pair<int, size_t> g = nors_id_addr.find(tr.train_id);
        if (g.first > 0) {printf("-1\n"); return;}
        std::pair<int, std::pair<size_t,size_t> > gg = rs_id_addr.find(tr.train_id);
        if (g.first > 0) {printf("-1\n"); return;}
        printf("0\n");
        size_t pl = train_inf.write(tr);
        nors_id_addr.insert(tr.train_id, pl); 
    }
    void delete_train(const char* tr_id) {
        std::pair<int, size_t> g = nors_id_addr.find(tr_id);
        if (g.first == 0) {printf("-1\n"); return;}
        printf("0\n");
        nors_id_addr.del(tr_id, g.second);
    }
    void release_train(const char* tr_id) {
        std::pair<int, size_t> g = nors_id_addr.find(tr_id);
        if (g.first == 0) {printf("-1\n"); return;}
        printf("0\n"); nors_id_addr.del(tr_id, g.second);
        train* p = train_inf.read(g.second);
        ticket tk(p->station_num, p->seat_num);
        std::pair<size_t, size_t> pl = std::make_pair(g.second, ticket_inf.write(tk));
        rs_id_addr.insert(tr_id, pl);
        for (int i = p->start_day + 1; i <= p->ed_day; ++i) ticket_inf.write(tk);
        for (int i = 0; i < p->station_num; ++i) {
            pass_tr it; strcpy(it.tr_id,tr_id); it.arr = p->arr_time[i]; it.lev = p->lev_time[i];
            it.p = p->price[i]; it.arr = pl.second; it.st = p->start_day; it.ed = p->ed_day;
            it.st_num = i;
            pass_st_tr.insert(p->name[i], it); 
        }  
    }
    void query_train(const train& tr) {
        std::pair<int, size_t> g = nors_id_addr.find(tr.train_id);
        if (g.first > 0) {
            train* p = train_inf.read(g.second);
            if (tr.start_day < p->start_day || tr.start_day > p->ed_day) {printf("-1\n"); return;}
            int len = strlen(tr.train_id);
            for (int i = 0; i < len; ++i) printf("%c",tr.train_id[i]);
            printf(" "); printf("%c\n", p->type); int tm = tr.start_day * 1440;
            for (int i = 0; i < p->station_num; ++i) {
                int ll = strlen(p->name[i]);
                for (int j = 0; j < ll; ++j) printf("%c",p->name[i][j]);
                printf(" "); 
                if (i == 0) printf("xx-xx xx:xx "); else prt(p->arr_time[i] + tm);
                printf("-> ");
                if (i + 1 == p->station_num) printf("xx-xx xx:xx "); else prt(p->lev_time[i] + tm);
                printf("%d ",p->price[i]); 
                if (i + 1 == p->station_num) printf("x\n"); 
                    else printf("%d\n",p->seat_num); 
            }
        } else {
            std::pair<int,std::pair<size_t, size_t> > gg = rs_id_addr.find(tr.train_id);
            if (gg.first == 0) {printf("-1\n"); return;}
            train* p = train_inf.read(gg.second.first);
            if (tr.start_day < p->start_day || tr.start_day > p->ed_day) {printf("-1\n"); return;}
            ticket* t = ticket_inf.read(gg.second.second + (tr.start_day - p->start_day)*sizeof(ticket));
            int len = strlen(tr.train_id);
            for (int i = 0; i < len; ++i) printf("%c",tr.train_id[i]);
            printf(" "); printf("%c\n", p->type); int tm = tr.start_day * 1440;
            for (int i = 0; i < p->station_num; ++i) {
                int ll = strlen(p->name[i]);
                for (int j = 0; j < ll; ++j) printf("%c",p->name[i][j]);
                printf(" ");
                if (i == 0) printf("xx-xx xx:xx "); else prt(p->arr_time[i] + tm);
                printf("-> ");
                if (i + 1 == p->station_num) printf("xx-xx xx:xx "); else prt(p->lev_time[i] + tm); 
                printf("%d ",p->price[i]); 
                if (i + 1 == p->station_num) printf("x\n"); 
                    else printf("%d\n",t->ticket_num[i]);                 
            }
        }
    }
    void merge_sort(sjtu::vector<qr_ticket>& a, qr_ticket* b, int l, int r) {
        if (l == r) return;
        int mid = (l + r) >> 1;    
        merge_sort(a, b, l, mid); merge_sort(a, b, mid + 1, r);
        int t1 = l; int t2 = mid + 1; int h = l;
        while (t1 <= mid && t2 <= r) {
            if (a[t1].ed - a[t1].st < a[t2].ed - a[t2].st ||
                ((a[t1].ed - a[t1].st == a[t2].ed - a[t2].st)&&(strcmp(a[t1].tr_id, a[t2].tr_id)<0)))
                    b[h++] = a[t1++]; else b[h++] = a[t2++];
        }
        while (t1 <= mid) b[h++] = a[t1++];
        while (t2 <= r) b[h++] = a[t2++];
        for (int i = l; i <= r; ++i) a[i] = b[i];
    }
    void Merge_sort(sjtu::vector<qr_ticket>& a, qr_ticket* b, int l, int r) {
        if (l == r) return;
        int mid = (l + r) >> 1;    
        Merge_sort(a, b, l, mid); Merge_sort(a, b, mid + 1, r);
        int t1 = l; int t2 = mid + 1; int h = l;
        while (t1 <= mid && t2 <= r) {
            if (a[t1].price < a[t2].price ||
                ((a[t1].price == a[t2].price)&&(strcmp(a[t1].tr_id, a[t2].tr_id)<0)))
                    b[h++] = a[t1++]; else b[h++] = a[t2++];
        }
        while (t1 <= mid) b[h++] = a[t1++];
        while (t2 <= r) b[h++] = a[t2++];
        for (int i = l; i <= r; ++i) a[i] = b[i];
    }
    void query_ticket(const qr& q) {
        sjtu::vector<pass_tr> s1;
        sjtu::vector<pass_tr> s2;
        sjtu::vector<qr_ticket> answer;
        int cnt1 = pass_st_tr.find(q.st, s1);
        int cnt2 = pass_st_tr.find(q.ed, s2);
        int p1 = 0; int p2 = 0;
        while (p1 < cnt1 && p2 < cnt2) {
            if (s1[p1].tic_add < s2[p2].tic_add) {p1++; continue;}
            if (s1[p1].tic_add > s2[p2].tic_add) {p2++; continue;}
            if (s1[p1].p > s2[p2].p) {p1++; p2++; continue;}
            int st_day = q.st_day - (s1[p1].lev / 1440);
            s1[p1].lev = s1[p1].lev % 1440;
            if (st_day < s1[p1].st || st_day > s1[p1].ed) {p1++; p2++; continue;}
            qr_ticket o; o.price = s2[p1].p - s1[p1].p; 
            o.st = st_day * 1440 + s1[p1].lev;
            o.ed = st_day * 1440 + s2[p2].arr; strcpy(o.tr_id,s1[p1].tr_id);
            ticket* r = ticket_inf.read(s1[p1].tic_add + sizeof(ticket) * (st_day - s1[p1].st));
            int mm = 1e7;
            for (int i = s1[p1].st_num; i < s2[p2].st_num; ++i) mm = min(mm, r->ticket_num[i]);
            o.seat_num = mm;
            answer.push_back(o);
            p1++; p2++;
        }
        printf("%d\n", (int)answer.size());
        if (answer.size() == 0) return;
        qr_ticket* u = new qr_ticket[answer.size()-1];
        if (q.is_time) merge_sort(answer, u, 0, answer.size()-1);
        else Merge_sort(answer, u, 0, answer.size()-1);
        for (int i = 0; i < (int)answer.size(); ++i) {
            print(answer[i].tr_id);
            print(q.st);
            prt(answer[i].st); printf("-> "); prt(answer[i].ed);
            printf("%d %d\n",answer[i].price, answer[i].seat_num);
        }
        delete []u;
    }
    void query_transfer(const qr& q) {

    }
    ~trains(){

    }
};
#endif