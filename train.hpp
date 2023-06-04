#ifndef train_control
#define train_control
#include <endian.h>
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
#include"ord_bpt.hpp"
#include"pend_bpt.hpp"
#include"hash.hpp"
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
struct ref{
    char user_id[22];
    int ord;
};
struct by_ticket{
    char user_id[22],tr_id[22],st[32],ed[32];
    int day; int num;
    bool is_pend;
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
    bool operator >= (const pass_tr& other) const {
        return tic_add >= other.tic_add;
    }
};
struct Order_val {
    int status; // 0 -> success 1 -> pending 2 -> refunded
    int ts,lev,arv,st,ed,num,p; 
    char tr_id[22];
    size_t pl; 
    char st_name[32]; char ed_name[32];
    bool operator < (const Order_val& other) const {
        return ts > other.ts;
    }
    bool operator > (const int& x) const {
        return ts > x;
    }
    bool operator == (const int& x) const {
        return ts == x;
    }
    bool operator > (const Order_val& other) const {
        return ts < other.ts;
    }
    bool operator == (const Order_val& other) const {
        return ts == other.ts;
    }
    bool operator >= (const Order_val& other) const {
        return ts <= other.ts;
    }
};
struct pender {
    int num; int st,ed;
    int stt; char user_id[22];
    bool operator < (const pender& other) const {
        return stt < other.stt;
    }
    bool operator > (const pender& other) const {
        return stt > other.stt;
    }
    bool operator == (const pender& other) const {
        return stt == other.stt;
    }
    bool operator >= (const pender& other) const {
        return stt >= other.stt;
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
    qr_ticket(const qr_ticket& other) {
        strcpy(tr_id, other.tr_id);
        price = other.price; st = other.st; ed = other.ed; seat_num = other.seat_num;
    }
    qr_ticket() {}
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
    ord_BPT<train_id, Order_val> order_inf; 
    pend_BPT<size_t, pender> pend_list;
    void print(const char* ch) {
        int len = strlen(ch);
        for (int i = 0; i < len; ++i) printf("%c",ch[i]);
        printf(" ");
    }
public:
    trains(char c1[], char c2[], char c3[], char c4[], char c5[], char c6[], char c7[]) :
        train_inf(c1, 100),ticket_inf(c2, 20),
        nors_id_addr(c3),rs_id_addr(c4), pass_st_tr(c5), order_inf(c6), pend_list(c7) {
    }
    void add_train(const train& tr) {
        std::pair<int, size_t> g = nors_id_addr.find(tr.train_id);
        if (g.first > 0) {printf("-1\n"); return;}
        std::pair<int, std::pair<size_t,size_t> > gg = rs_id_addr.find(tr.train_id);
        if (gg.first > 0) {printf("-1\n"); return;}
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
            it.p = p->price[i]; it.tic_add = pl.second; it.st = p->start_day; it.ed = p->ed_day;
            it.st_num = i;
            pass_st_tr.insert(p->name[i], it); 
        }
    }
    void query_train(const train& tr) {
        if (tr.start_day < 0) {printf("-1\n"); return;}
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
        if (q.st_day < 0) {printf("0\n"); return;} 
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
            if (st_day < s1[p1].st || st_day > s1[p1].ed) {p1++; p2++; continue;}
            qr_ticket o; o.price = s2[p2].p - s1[p1].p; 
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
        qr_ticket* u = new qr_ticket[answer.size()];
        if (q.is_time) merge_sort(answer, u, 0, answer.size()-1);
        else Merge_sort(answer, u, 0, answer.size()-1);
        for (int i = 0; i < (int)answer.size(); ++i) {
            print(answer[i].tr_id);
            print(q.st);
            prt(answer[i].st); printf("-> "); print(q.ed); prt(answer[i].ed);
            printf("%d %d\n",answer[i].price, answer[i].seat_num);
        }
        delete []u;
    }
    void query_transfer(const qr& q) {
        if (q.st_day < 0) {printf("0\n"); return;}
        sjtu::vector<pass_tr> o;
        pass_st_tr.find(q.st, o);
        if (o.empty()) {printf("0\n"); return;}    
        find_hash e;
        for (int i = 0; i < o.size(); ++i) {
            int st_dy = q.st_day - o[i].lev / 1440;
            if (st_dy < o[i].st || st_dy > o[i].ed) continue;
            train* tr = train_inf.read(rs_id_addr.find(o[i].tr_id).second.first);
            size_t pl = o[i].tic_add + sizeof(ticket) * (st_dy - o[i].st);
            for (int j = o[i].st_num + 1; j < tr->station_num; ++j) {
                trans w; w.pl = pl; w.st = o[i].st_num; w.ed = j;
                w.price = tr->price[j] - tr->price[o[i].st_num];
                w.st_lev_time = o[i].lev + st_dy * 1440; 
                w.trans_arr_time = tr->arr_time[j] + st_dy * 1440;
                strcpy(w.st_name, tr->name[j]);
                strcpy(w.tr_id, tr->train_id);
                e.insert(w);
            }
        }
        sjtu::vector<pass_tr> oo;
        pass_st_tr.find(q.ed, oo);
        if (oo.empty()) {printf("0\n"); return;}
        sjtu::vector<trans_ans> ans;
        for (int i = 0; i < oo.size(); ++i) {
            train* tr = train_inf.read(rs_id_addr.find(oo[i].tr_id).second.first);
            for (int j = oo[i].st_num - 1; j >= 0; --j) {
                e.find(tr->train_id, tr->name[j], j, oo[i].st_num,  tr->start_day, tr->ed_day, tr->lev_time[j],
                tr->arr_time[oo[i].st_num], tr->price[oo[i].st_num] - tr->price[j], oo[i].tic_add ,ans);
            } 
        }
        if (ans.empty()) {printf("0\n"); return;}
        for (int i = 1; i < ans.size(); ++i) {
            if (q.is_time) {
                if (ans[i].arr2 - ans[i].lev1 < ans[0].arr2 - ans[0].lev1) ans[0] = ans[i];
                else if (ans[i].arr2 - ans[i].lev1 == ans[0].arr2 - ans[0].lev1) {
                    if (ans[i].pr2 + ans[i].pr1 < ans[0].pr2 + ans[0].pr1) ans[0] = ans[i];
                    else {
                        if (ans[i].pr2 + ans[i].pr1 == ans[0].pr2 + ans[0].pr1) {
                            if (strcmp(ans[i].tr_id1,ans[0].tr_id1) < 0) {
                                ans[0] = ans[i];
                            } else if(strcmp(ans[i].tr_id1,ans[0].tr_id1) == 0 
                                && strcmp(ans[i].tr_id2,ans[0].tr_id2) < 0) ans[0] = ans[i];                            
                        }

                    }
                }
            } else {
                if (ans[i].pr2 + ans[i].pr1 < ans[0].pr2 + ans[0].pr1) ans[0] = ans[i];
                else if (ans[i].pr2 + ans[i].pr1 == ans[0].pr2 + ans[0].pr1) {
                    if (ans[i].arr2 - ans[i].lev1 < ans[0].arr2 - ans[0].lev1) ans[0] = ans[i];
                    else {
                        if (ans[i].arr2 - ans[i].lev1 == ans[0].arr2 - ans[0].lev1) {
                            if (strcmp(ans[i].tr_id1,ans[0].tr_id1) < 0) {
                                ans[0] = ans[i];
                            } else if(strcmp(ans[i].tr_id1,ans[0].tr_id1) == 0 
                                && strcmp(ans[i].tr_id2,ans[0].tr_id2) < 0) ans[0] = ans[i];                            
                        }
                    }
                }
            }
        }
        ticket* tk = ticket_inf.read(ans[0].pl1);
        int tk1 = 1e7;
        for (int i = ans[0].st1; i < ans[0].ed1; ++i) tk1 = min(tk1, tk->ticket_num[i]);
        tk = ticket_inf.read(ans[0].pl2);
        int tk2 = 1e7;
        for (int i = ans[0].st2; i < ans[0].ed2; ++i) tk2 = min(tk2, tk->ticket_num[i]);
        print(ans[0].tr_id1); print(q.st); prt(ans[0].lev1); printf("-> ");
        print(ans[0].tr_st); prt(ans[0].arr1); printf("%d %d\n", ans[0].pr1, tk1);
        print(ans[0].tr_id2); print(ans[0].tr_st); prt(ans[0].lev2); printf("-> ");
        print(q.ed); prt(ans[0].arr2); printf("%d %d\n",ans[0].pr2, tk2);    
    }
    void buy_ticket(const by_ticket& b, int time_stamp) {
        if (b.day < 0) {printf("-1\n"); return;}
        pair<int,pair<size_t,size_t> > o = rs_id_addr.find(b.tr_id);
        if (o.first == 0) {printf("-1\n"); return;}
        train* tr = train_inf.read(o.second.first);
        if (b.num > tr->seat_num) {printf("-1\n"); return;}
        int st = -1; int ed = -1;
        for (int i = 0; i < tr->station_num; ++i) {
            if (strcmp(b.st, tr->name[i]) == 0) st = i;
            if (strcmp(b.ed, tr->name[i]) == 0) ed = i;
        }
        if (st == -1 || ed == -1 || st >= ed) {printf("-1\n"); return;} 
        int st_dy = b.day - tr->lev_time[st] / 1440;
        if (st_dy < tr->start_day || st_dy > tr->ed_day) {printf("-1\n"); return;} 
        Order_val it; it.st = st; it.ed = ed; 
        it.lev = st_dy * 1440 + tr->lev_time[st]; 
        it.arv = st_dy * 1440 + tr->arr_time[ed];
        strcpy(it.tr_id, b.tr_id); strcpy(it.st_name, b.st); strcpy(it.ed_name, b.ed);
        it.num = b.num; it.p = tr->price[ed] - tr->price[st]; it.ts = time_stamp;
        it.pl = o.second.second + sizeof(ticket) * (st_dy - tr->start_day);
        int mm = 1e7;
        ticket* e = ticket_inf.read(it.pl);
        for (int i = st; i < ed; ++i) mm = min(mm, e->ticket_num[i]);
        if (mm >= b.num) {
            it.status = 0; printf("%lld\n", 1ll * b.num * it.p);
            for (int i = st; i < ed; ++i) e->ticket_num[i] -= b.num;
        } else {
            if (b.is_pend == 0) {printf("-1\n"); return;}
            it.status = 1; printf("queue\n");
            pender o; o.st = st; o.ed = ed; o.num = b.num; o.stt = time_stamp;  
            strcpy(o.user_id, b.user_id);
            pend_list.insert(it.pl, o);
        }
        order_inf.insert(b.user_id, it);
    }
    void query_order(const char* u) {
        sjtu::vector<Order_val> o;
        order_inf.find(u, o);
        printf("%d\n", (int)o.size());
        for (int i = 0; i < (int)o.size(); ++i) {
            printf("["); 
            if (o[i].status == 0) printf("success] ");
            if (o[i].status == 1) printf("pending] ");
            if (o[i].status == 2) printf("refunded] ");
            print(o[i].tr_id); print(o[i].st_name); prt(o[i].lev);
            printf("-> "); print(o[i].ed_name); prt(o[i].arv); 
            printf("%d %d\n",o[i].p, o[i].num);
        }
    }
    void refund(const char* u, int n) {
        sjtu::vector<Order_val> o;
        order_inf.find(u,o);
        if (n > (int)o.size()) {printf("-1\n"); return;}
        if (o[n-1].status == 2) {printf("-1\n"); return;}
        printf("0\n");
        if (o[n-1].status == 0) {
            ticket* tk = ticket_inf.read(o[n-1].pl);
            order_inf.modify(u, o[n-1].ts, 2);
            for (int i = o[n-1].st; i < o[n-1].ed; ++i) tk->ticket_num[i] += o[n-1].num;
            sjtu::vector<pender> may_buy_ticket;
            pend_list.find(o[n-1].pl, may_buy_ticket);
            pender p;
            for (int i = 0; i < may_buy_ticket.size(); ++i) {
                int mm = 1e7;
                for (int j = may_buy_ticket[i].st; j < may_buy_ticket[i].ed; ++j) 
                    mm = min(mm, tk->ticket_num[j]);
                if (mm >= may_buy_ticket[i].num) {
                    for (int j = may_buy_ticket[i].st; j < may_buy_ticket[i].ed; ++j) 
                    tk->ticket_num[j] -= may_buy_ticket[i].num;
                    p.stt = may_buy_ticket[i].stt;
                    pend_list.del(o[n-1].pl, p);
                    order_inf.modify(may_buy_ticket[i].user_id, may_buy_ticket[i].stt, 0);
                }
            }
        } else {
            order_inf.modify(u, o[n-1].ts, 2);
            pender p; p.stt = o[n-1].ts;
            pend_list.del(o[n-1].pl, p);
        }
    }
    void clear(char c1[], char c2[], char c3[], char c4[], char c5[], char c6[], char c7[]){
        train_inf.clear(c1); ticket_inf.clear(c2);
        nors_id_addr.clear(c3); rs_id_addr.clear(c4); pass_st_tr.clear(c5); 
        order_inf.clear(c6); pend_list.clear(c7);
    }  
    ~trains(){
    }
};
#endif