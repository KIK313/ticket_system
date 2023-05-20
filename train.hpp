#ifndef train_control
#define train_control
#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<fstream>
#include"simple_file_LRU.hpp"
#include"bpt.hpp"
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
    bool is_G;
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
    void query_train() {

    }
    void qury_ticket() {
        
    }
    ~trains(){

    }
};
#endif