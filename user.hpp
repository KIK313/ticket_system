#ifndef user_control
#define user_control

#include <fstream>
#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include "bpt.hpp"
#include "simple_file_LRU.hpp"
;
#pragma pack(push,1)
struct ins
{   
    char c[22];
    char user_id[22];
    char password[32];
    char user_name[17];
    char email[32];
    int pri;
    ins() {c[0] = user_id[0] = password[0] = user_name[0] = email[0] = '\0'; pri = -1;}    
};
struct user{
    char user_id[22];
    char password[32];
    char user_name[17];
    char email[32];
    int pri;
    user(const user& u) {
        strcpy(user_id, u.user_id);
        strcpy(password, u.password);
        strcpy(user_name, u.user_name);
        strcpy(email, u.email);
        pri = u.pri;
    }
    user(const ins& u) {
        strcpy(user_id, u.user_id);
        strcpy(password, u.password);
        strcpy(user_name, u.user_name);
        strcpy(email, u.email);
        pri = u.pri;        
    }
    user(){}
};
#pragma pack(pop)
struct user_id {
    char id[22];
    public:
    user_id() {}
    user_id(const char* s) {
        strcpy(id,s);
    }
    bool operator < (const user_id& other) const {
        return (strcmp(id,other.id)<0);
    }
    bool operator < (const char* s) const {
        return (strcmp(id,s)<0);
    }
    bool operator > (const char* s) const {
        return (strcmp(id,s)>0);
    }
    bool operator > (const user_id& other) const {
        return (strcmp(id,other.id)>0);
    }
    bool operator == (const user_id& other) const {
        return (strcmp(id, other.id) == 0);
    }
};

class users{
private:
    class online_user{
    private:
        struct val{
            char id[22];
            int pri;    
            val(const val& u) {
                pri = u.pri; strcpy(id, u.id);
            }
            val(const char* ch, int pos) {
                strcpy(id, ch); pri = pos;
            }
        };
        struct node{
            node* nxt;
            node* pre;
            val* nd;
            node() {nxt = pre = nullptr; nd = nullptr;}
            ~node() {if (nd != nullptr) delete nd;}
        };
        const int M;
        int hash(const char* ch) {
            int u = 0; int len=strlen(ch);
            for (int i = 0; i < len; i++) {
                u = (u * 499 + ch[i]) % 9973;
            }
            return u;
        }
        node head[9973], tail[9973]; 
    public:
        online_user() :M(9973) {
            for (int i = 0; i < M; ++i) {
                head[i].nxt = &tail[i];    
                tail[i].pre = &head[i];
            }
        }
        int find(const char* ch) {
            int o = hash(ch);
            node* p = head[o].nxt;
            while (p != &tail[o]) {
                if (strcmp(p->nd->id, ch) == 0) {
                    return p->nd->pri;
                }
                p = p->nxt;
            }
            return -1;
        }   
        void insert(const char* ch, int v) {
            int o = hash(ch);
            node* p = new node();
            p->nd = new val(ch, v);
            p->nxt = head[o].nxt; head[o].nxt->pre = p;
            head[o].nxt = p; p->pre = &head[o];
        }
        int del(const char* ch) {
            int o = hash(ch);
            node* p = head[o].nxt;
            while (p != &tail[o]) {
                if (strcmp(p->nd->id, ch) == 0) {
                    p->nxt->pre = p->pre;
                    p->pre->nxt = p->nxt;
                    delete p;
                    return 0;
                }
                p = p->nxt;
            }
            return -1;
        }
        void modify(const char* ch, int pri) {
            int o = hash(ch);
            node* p = head[o].nxt;
            while (p != &tail[o]) {
                if (strcmp(ch, p->nd->id) == 0) {
                    p->nd->pri = pri;
                    return;
                }
                p = p->nxt;
            }
            return;
        }
        ~online_user() {
            for (int i = 0; i < M; ++i) {
                node* p = head[i].nxt;
                while (p != &tail[i]) {
                    node* q = p->nxt;
                    delete p;
                    p = q;
                }
            }
        }
        /*login add_user(const val& u) {
            int o = hash(u.id);
            node* nd = new node(); nd->nd = new val(u);     
        }*/
    }log_user;
    BPT<user_id,size_t> id_addr;
    file<user> addr_inf;
public:
    users(char w[],char r[]) :id_addr(w),addr_inf(r,300) {

    }
    void add_user(const ins& u) {
        if (addr_inf.is_empty()) {
            user o(u); o.pri = 10;
            size_t pl = addr_inf.write(o);
            user_id r(u.user_id);
            id_addr.insert(r, pl);
            printf("0\n");
        } else {
            int r = log_user.find(u.c);
            user_id oo(u.user_id); 
            if (r < 0 || u.pri >= r || id_addr.find(oo).first > 0) {
                printf("-1\n");
                return;
            }
            printf("0\n");
            user o(u);
            size_t pl = addr_inf.write(o);
            user_id rr(u.user_id);
            id_addr.insert(rr, pl);            
        }
    }
    void login(const ins& u) {
        std::pair<int, size_t> o = id_addr.find(u.user_id);
        if (o.first == 0) {printf("-1\n"); return;}
        if (log_user.find(u.user_id) >= 0) {printf("-1\n"); return;}
        user* z = addr_inf.read(o.second);
        if (strcmp(z->password, u.password) == 0) {
            printf("0\n");
            log_user.insert(z->user_id, z->pri);
        } else printf("-1\n");
    }
    void logout(const ins& u) {
        printf("%d\n",log_user.del(u.user_id));
    }
    void print(const char* ch) {
        int len = strlen(ch);
        for (int i = 0; i < len; ++i) printf("%c",ch[i]);
        printf(" ");
    }
    void query_profile(const ins& u) {
        int pr = log_user.find(u.c);
        if (pr < 0) {
            printf("-1\n"); return;
        }
        if (strcmp(u.c, u.user_id) == 0) {
            std::pair<int, size_t> o = id_addr.find(u.user_id);
            if (o.first == 0) {
                printf("-1\n"); return;
            }
            user* uu = addr_inf.read(o.second);
            print(uu->user_id); print(uu->user_name); print(uu->email); printf("%d\n",uu->pri);
        } else {
            std::pair<int, size_t> o = id_addr.find(u.user_id);
            if (o.first == 0) {
                printf("-1\n"); return;
            }
            user* uu = addr_inf.read(o.second);
            if (pr > uu->pri) {
                print(uu->user_id); print(uu->user_name);
                print(uu->email); printf("%d\n",uu->pri);
            } else printf("-1\n");
        }
    }
    void modify_profile(const ins& u) {
        int pr = log_user.find(u.c);
        if (pr < 0) {printf("-1\n"); return;}
        user* uu;
        if (strcmp(u.c, u.user_id) == 0) {
            std::pair<int, size_t> o = id_addr.find(u.user_id);
            if (o.first == 0) {
                printf("-1\n"); return;
            }
            uu = addr_inf.read(o.second);
        } else {
            std::pair<int, size_t> o = id_addr.find(u.user_id);
            if (o.first == 0) {
                printf("-1\n"); return;  
            }
            uu = addr_inf.read(o.second);
            if (uu->pri >= pr) {
                printf("-1\n"); return;
            }
        }
        if (u.password[0] != '\0') {
            strcpy(uu->password, u.password);
        }
        if (u.email[0] != '\0') {
            strcpy(uu->email, u.email);
        }
        if (u.user_name[0] != '\0') {
            strcpy(uu->user_name, u.user_name);
        }
        if (u.pri != -1) {
            uu->pri = u.pri;
            log_user.modify(u.user_id, u.pri);
        }
        print(uu->user_id); print(uu->user_name);
        print(uu->email); printf("%d\n",uu->pri);
    }
    bool is_online(const char* ch) {
        return log_user.find(ch) >= 0;
    }
    ~users() {
    }
};
#endif