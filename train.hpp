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
;
#pragma pack(push,1)
struct train {
    char train_id[22];
    int station_num;
    char name[102][32];
    int seat_num;
    int travel_time[101];
    int stop_time[101];
    int price[101];
    char type;
    int start_day; // 0 -> 91;
    int ed_day; // 0 -> 91;
    int start_min; // 0 -> 24*3600
};
struct ticket{
    int ticket_num[92][101];
    int st, ed, sn;
    ticket(int st, int ed, int s, int t) :st(st), ed(ed), sn(s) {
        for (int i = st; i <= ed; ++i) 
            for (int j = 0; j < s - 1; ++j) ticket_num[i][j] = t;
    }
    ticket(){}
};
#pragma pack(pop)
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
class trains {
private:
    file<train> train_inf;
    file<ticket> ticket_inf;
    BPT<train_id, size_t> nors_id_addr;
    BPT<train_id, std::pair<size_t, size_t> > rs_id_addr;
public:
    trains(char c1[], char c2[], char c3[], char c4[]) :
        train_inf(c1, 100),ticket_inf(c2, 20),
        nors_id_addr(c3),rs_id_addr(c4) {
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
        ticket tk(p->start_day, p->ed_day, p->station_num, p->seat_num);
        std::pair<size_t, size_t> pl = std::make_pair(g.second, ticket_inf.write(tk));
        rs_id_addr.insert(tr_id, pl);
    }
    void query_train(const train& tr) {
        std::pair<int, size_t> g = nors_id_addr.find(tr.train_id);
        if (g.first > 0) {
            train* p = train_inf.read(g.second);
            if (tr.start_day < p->start_day || tr.start_day > p->ed_day) {printf("-1\n"); return;}
            int len = strlen(tr.train_id);
            for (int i = 0; i < len; ++i) printf("%c",tr.train_id[i]);
            printf(" "); printf("%c\n", p->type); int tm = p->start_min + tr.start_day * 1440;
            for (int i = 0; i < p->station_num; ++i) {
                int ll = strlen(p->name[i]);
                for (int j = 0; j < ll; ++j) printf("%c",p->name[i][j]);
                printf(" "); 
                if (i == 0) printf("xx-xx xx:xx "); else prt(tm);
                if (i > 1 && i < p->station_num - 1) tm += p->stop_time[i];
                printf("-> ");
                if (i + 1 == p->station_num) printf("xx-xx xx:xx "); else prt(tm);
                printf("%d ",p->price[i]); 
                if (i + 1 == p->station_num) printf("x\n"); 
                    else printf("%d\n",p->seat_num), tm += p->travel_time[i]; 
            }
        } else {
            std::pair<int,std::pair<size_t, size_t> > gg = rs_id_addr.find(tr.train_id);
            if (gg.first == 0) {printf("-1\n"); return;}
            train* p = train_inf.read(g.second);
            if (tr.start_day < p->start_day || tr.start_day > p->ed_day) {printf("-1\n"); return;}
            ticket* t = ticket_inf.read(gg.second.second);
            printf(" "); printf("%c\n", p->type); int tm = p->start_min + tr.start_day * 1440;
            for (int i = 0; i < p->station_num; ++i) {
                int ll = strlen(p->name[i]);
                for (int j = 0; j < ll; ++j) printf("%c",p->name[i][j]);
                printf(" ");
                if (i == 0) printf("xx-xx xx:xx "); else prt(tm);
                if (i > 1 && i < p->station_num - 1) tm += p->stop_time[i];
                printf("-> ");
                if (i + 1 == p->station_num) printf("xx-xx xx:xx "); else prt(tm);
                printf("%d ",p->price[i]); 
                if (i + 1 == p->station_num) printf("x\n"); else printf("%d\n",p->seat_num); 
                printf("%d ",p->price[i]); 
                if (i + 1 == p->station_num) printf("x\n"); 
                    else printf("%d\n",t->ticket_num[tr.start_day][i]), tm += p->travel_time[i];                 
            }
        }
    }
    void query_ticket() {
        
    }
    ~trains(){

    }
};
#endif