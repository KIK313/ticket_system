#ifndef bpt_hpp
#define bpt_hpp
#include <cstddef>
#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<fstream>
const int mod=499;
const int M = 159;
template<class Key,class value>
class BPT {
    #pragma pack(push,1)
    struct Node {   
        int siz;
        bool is_leaf;
        size_t r[M+1];
        Key ind[M+1];
        value val[M+1];
        Node(){ siz = 0;}
        Node(const Node& other) {
            siz = other.siz; is_leaf = other.is_leaf;
            for (int i = 0; i <= M; ++i) 
                r[i] = other.r[i], ind[i] = other.ind[i], val[i] = other.val[i];
        } 
    };
    #pragma pack(pop)
private:
    class LRU {
        private:    
            std::fstream rw;
            int siz;
            size_t root_pos; 
            struct nd_link {
                Node* np; // ptr in internal memory
                size_t pos; // position in external memory 
                nd_link* hp; // hash pre;
                nd_link* hn; // hash next
                nd_link* tp; // time pre
                nd_link* tn; // time next
                nd_link(size_t poss=0): pos(poss){ np = nullptr; hp = hn = tp = tn = nullptr;}
                ~nd_link() {if (np != nullptr) delete np;}
            };
            nd_link Thead,Ttail;
            nd_link Hhead[mod],Htail[mod];
        public:
            LRU(char ch[]) {
                siz = 0;
                rw.open(ch,std::ios::in|std::ios::out|std::ios::binary);
                if (!rw) {
                    rw.open(ch,std::ios::out);
                    rw.close(); rw.open(ch,std::ios::in|std::ios::out|std::ios::binary);
                    rw.seekg(0);
                    root_pos = sizeof(size_t);
                    size_t g = sizeof(size_t);
                    rw.write(reinterpret_cast<const char*>(&g), sizeof(size_t)); 
					Node w; w.is_leaf = 1;
                    rw.write(reinterpret_cast<const char*>(&w), sizeof(Node));
                } else {                    
                    rw.seekg(0);
                    rw.read(reinterpret_cast<char *>(&root_pos),sizeof(size_t));
                }
                Thead.tn = &Ttail; Thead.tp = nullptr; Ttail.tp = &Thead; Ttail.tn = nullptr;
                for (int i = 0; i < mod; ++i) {
                    Hhead[i].hp = Htail[i].hn = nullptr;
                    Hhead[i].hn = &Htail[i]; Htail[i].hp = &Hhead[i];
                }
			}
            size_t getroot() {return root_pos;}
            void change_root(size_t pl) {root_pos = pl;}
            void rt(nd_link* u) {
                rw.seekp(u->pos);
                rw.write(reinterpret_cast<const char*>(u->np), sizeof(Node));
            }
            void del(nd_link* u) {
                (u->tp)->tn = &Ttail;
                Ttail.tp = u->tp;
                (u->hp)->hn = u->hn;
                (u->hn)->hp = u->hp;
                rt(u);
                delete u;
            }
            void move_to_front(nd_link* u) {
                (u->tn)->tp = u->tp; (u->tp)->tn = u->tn;
                u->tn = Thead.tn; u->tp = &Thead;
                Thead.tn = u; (u->tn)->tp = u;
            }
            Node rd(size_t pl) {
                rw.seekg(pl);
				Node nd;
				rw.read(reinterpret_cast<char *>(&nd),sizeof(Node));
                return nd;
            }
            size_t getpl(const Node& u) {
                rw.seekp(0,std::ios::end);
                size_t pl = rw.tellp();
                rw.write(reinterpret_cast<const char*>(&u), sizeof(Node));
				return pl;
            }
            Node* find(size_t pl) {
            	int s = pl % mod;
                nd_link* w = &Hhead[s]; nd_link* lp = &Htail[s];
                while (w->hn != lp) {
                    w = w->hn;
                    if (w->pos == pl) {
                        move_to_front(w); 
						return w->np;
                    }
                }
                if (siz == 2000) {
                    nd_link* u = Ttail.tp;
                    del(u);
                } else siz++;
				nd_link* nd = new nd_link(pl);
                nd->np = new Node(rd(pl));
                nd->tn = Thead.tn; nd->tp = &Thead;
                (nd->tn)->tp = nd; Thead.tn = nd;
                nd_link* r = &Hhead[s];
                nd->hn = r->hn; nd->hp = r;
                r->hn->hp = nd; r->hn = nd;
                return nd->np;
            } 
            ~LRU() {
                siz = 0; 
                rw.seekp(0); rw.write(reinterpret_cast<const char*>(&root_pos),sizeof(size_t)); 
				nd_link* p = Thead.tn; nd_link* pl = &Ttail;
                while (p != pl) {
                    nd_link* q = p->tn;
                    rt(p);
                    delete p;
                    p = q;
                }                
				rw.close();
		    }

    }cash;
    struct ins_retype
    {
        bool is_split;
        size_t rs; Key kk; value val; 
    };
    ins_retype insert(const size_t& pl, const Key& key, const value& val) {
		Node* u = cash.find(pl);
		if (u->is_leaf) {
            int t = u->siz - 1;
            while (t >= 0) {
                if (u->ind[t] > key || (u->ind[t] == key&&u->val[t] > val ) ) {
                    u->val[t+1] = u->val[t];
                    u->ind[t+1] = u->ind[t];
                    t--;
                } else break;
            }
            t++; u->val[t] = val; u->ind[t] = key;
            u->siz++;
            ins_retype it;
            if (u->siz <= M) {
                it.is_split = 0;
                return it;
            } else {
                it.is_split = 1;
                Node nd; nd.siz = u->siz / 2; nd.is_leaf = 1;
                for (int i = 0; i < nd.siz; ++i) 
                    nd.ind[i] = u->ind[i+nd.siz], nd.val[i] = u->val[i+nd.siz];
                it.kk = u->ind[nd.siz]; it.val = u->val[nd.siz];
                u->siz = nd.siz; 
                it.rs = cash.getpl(nd); 
				return it;
            }
        } else {
		    int t = u->siz; 
            ins_retype e; int ls = 0;
            if (key > u->ind[t-1] || (key == u->ind[t-1] && val >= u->val[t-1]))  
                e = insert(u->r[t], key, val), ls = t; else {
                int l = 0; int r = t - 1;
                while (l < r) {
                    int mid = (l + r) >> 1;
                    if (key < u->ind[mid] || (key == u->ind[mid] && val < u->val[mid])) r = mid;
                    else l = mid + 1;
                }
                e = insert(u->r[l], key, val); ls = l;
            }   
            if (e.is_split == 0)  return e;
            for (int i = u->siz - 1; i >= ls; i--) 
                u->ind[i+1] = u->ind[i], u->val[i+1] = u->val[i];
            for (int i = u->siz; i > ls; i--) u->r[i+1] = u->r[i];
            u->siz++;
            u->ind[ls] = e.kk; u->val[ls] = e.val; u->r[ls+1] = e.rs;   					
			if (u->siz <= M - 1) {
                ins_retype it; it.is_split = 0;
                return it;
            } else {
                ins_retype it; it.is_split = 1;
                Node nd; nd.siz = u->siz / 2; nd.is_leaf = 0;
                for (int i = 0; i < nd.siz; ++i) 
                    nd.ind[i] = u->ind[i+nd.siz+1], nd.val[i] = u->val[i+nd.siz+1];
                for (int i = 0; i <= nd.siz; ++i)
                    nd.r[i] = u->r[i+nd.siz+1];
                it.kk = u->ind[nd.siz]; it.val = u->val[nd.siz];
                u->siz = nd.siz; 
                it.rs = cash.getpl(nd);                
                return it;
            }
        }
    }
    int f_cnt;
    value raddr;
    void find(size_t pl, const Key& key) {
        Node* u = cash.find(pl);
        if (u->is_leaf) {
            if (u->siz == 0) return;
            if (u->ind[u->siz - 1] < key) return;
            int l = 0; int r = u->siz - 1;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (u->ind[mid] < key) l = mid + 1;
                else r = mid;
            }
            while (l < u->siz && u->ind[l] == key) {
                f_cnt++; raddr = u->val[l]; l++;
            }
        } else {
            int l = 0; int r = u->siz;
			while (l < r) {
                int mid = (l + r) >> 1;
                if (u->ind[mid] < key || u->ind[mid] == key) l = mid + 1;
                else r = mid;
            }
            while (1) {
                if (l - 1 < 0 || u->ind[l-1] < key) break; 
                l--;
            }
            for (int i = l; i <= r; ++i) find(u->r[i], key),u=cash.find(pl);
        }
    }
    void workL(Node* u, Node* son, Node* ls, int l) {
        int O = (M + 1) / 2;
        if (son->is_leaf) {
            if (ls->siz > O) {
                for (int i = son->siz - 1; i >= 0; --i) {
                    son->ind[i+1] = son->ind[i]; 
                    son->val[i+1] = son->val[i];
                }
                son->siz++;
                son->ind[0] = ls->ind[ls->siz-1]; son->val[0] = ls->val[ls->siz-1];
                ls->siz--;
                u->ind[l-1] = son->ind[0]; u->val[l-1] = son->val[0]; 
            } else {
                for(int i = l; i < u->siz; ++i) u->ind[i-1] = u->ind[i],u->val[i-1] = u->val[i];
                for(int i = l; i < u->siz; ++i) u->r[i] = u->r[i+1];
                u->siz--;
                for (int i = 0; i < son->siz; ++i) 
                    ls->ind[i+ls->siz] = son->ind[i], ls->val[i+ls->siz] = son->val[i];
                ls->siz += son->siz;
            }
        } else {
            if (ls->siz + 1 > O) {
                for(int i = son->siz - 1; i >= 0; --i) 
                    son->ind[i+1] = son->ind[i], son->val[i+1] = son->val[i];
                son->ind[0] = u->ind[l-1]; son->val[0] = u->val[l-1];
                for(int i = son->siz; i >= 0; --i) son->r[i+1] = son->r[i];
                son->r[0] = ls->r[ls->siz];
                son->siz++; 
                ls->siz--;
                u->ind[l-1] = ls->ind[ls->siz]; u->val[l-1] = ls->val[ls->siz];
            } else {
                ls->ind[ls->siz] = u->ind[l-1]; ls->val[ls->siz] = u->val[l-1];
                for (int i = 0; i < son->siz; ++i) {
                    ls->ind[ls->siz+i+1] = son->ind[i];
                    ls->val[ls->siz+i+1] = son->val[i];
                }
                for (int i = 0; i <= son->siz; ++i) ls->r[ls->siz+i+1] = son->r[i];
                ls->siz += son->siz + 1;
                u->siz--;
                for (int i = l - 1; i < u->siz; ++i) {
                    u->ind[i] = u->ind[i+1];
                    u->val[i] = u->val[i+1];
                    u->r[i+1] = u->r[i+2];
                }
            }
        }
    }
    void workR(Node* u, Node* son, Node* rs, int l) {
        int O = (M + 1) / 2;
        if (son->is_leaf) {
            if (rs->siz > O) {
                son->ind[son->siz] = rs->ind[0];
                son->val[son->siz] = rs->val[0];
                son->siz++;
                rs->siz--;
                for(int i = 0; i < rs->siz; ++i) {
                    rs->ind[i] = rs->ind[i+1];
                    rs->val[i] = rs->val[i+1];
                }
                u->ind[l] = rs->ind[0]; u->val[l] = rs->val[0];
            } else {
                for(int i = l + 1; i < u->siz; ++i) u->ind[i-1] = u->ind[i],u->val[i-1] = u->val[i];
                for(int i = l + 1; i < u->siz; ++i) u->r[i] = u->r[i+1];
                u->siz--;
                for (int i = 0; i < rs->siz; ++i) 
                    son->ind[i+son->siz] = rs->ind[i], son->val[i+son->siz] = rs->val[i];
                son->siz += rs->siz;
            }
        } else {
            if (rs->siz + 1 > O) {
                son->ind[son->siz] = u->ind[l]; son->val[son->siz] = u->val[l];
                son->siz++;
                son->r[son->siz] = rs->r[0];
                u->ind[l] = rs->ind[0]; u->val[l] = rs->val[0];
                rs->siz--;
                for (int i = 0; i < rs->siz; ++i) 
                    rs->ind[i] = rs->ind[i+1],rs->val[i] = rs->val[i+1];
                for (int i = 0; i <= rs->siz; ++i)
                    rs->r[i] = rs->r[i+1]; 
            } else {
                son->ind[son->siz] = u->ind[l]; son->val[son->siz] = u->val[l];
                for(int i = 0; i < rs->siz; ++i) {
                    son->ind[son->siz+1+i] = rs->ind[i];
                    son->val[son->siz+1+i] = rs->val[i];
                }
                for(int i = 0; i <= rs->siz; ++i) son->r[son->siz+1+i] = rs->r[i]; 
                son->siz += rs->siz + 1;
                u->siz--;
                for (int i = l; i < u->siz; ++i) {
                    u->ind[i] = u->ind[i+1];
                    u->val[i] = u->val[i+1];
                    u->r[i+1] = u->r[i+2];
                }
            }
        }
    }
    void del(size_t pl, const Key& key, const value& val) {
        Node* u = cash.find(pl);
        if (u->is_leaf) {
            if (u->siz == 0) return;
            if (u->ind[0] > key || (u->ind[0] == key && u->val[0] > val)) return;
            int l = 0; int r = u->siz - 1;
            while (l < r) {
                int mid = (l + r + 1) >> 1;
                if (u->ind[mid] > key || (u->ind[mid] == key && u->val[mid] > val)) r = mid - 1;
                else l = mid;
            }
            if (u->ind[l] == key && u->val[l] == val) {
                u->siz--;
                for (int i = l; i < u->siz; ++i) {
                    u->ind[i] = u->ind[i+1];
                    u->val[i] = u->val[i+1];
                }
            } 
            return;    
        }
        int l = 0; int r = u->siz;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (key < u->ind[mid] || (key == u->ind[mid] && val < u->val[mid])) r = mid;
            else l = mid + 1;
        }
        del(u->r[l], key, val);
        Node* son = cash.find(u->r[l]);
        if (son->is_leaf) {
            int O = (M + 1) / 2;
            if (son->siz >= O) return;
            if (l > 0 && l + 1 < u->siz) {
                Node* ls = cash.find(u->r[l-1]);
                if (ls->siz > O) workL(u, son, ls, l);
                else workR(u, son, cash.find(u->r[l+1]), l);
            } else {
                if (l > 0) workL(u, son, cash.find(u->r[l-1]), l);
                else workR(u, son, cash.find(u->r[l+1]), l);
            }
        } else {
            int O = (M + 1) / 2;
            if (son->siz + 1 >= O) return;
            if (l > 0 && l + 1 <= u->siz) {
                Node* ls = cash.find(u->r[l-1]);
                if (ls->siz + 1 > O) workL(u, son, ls, l);
                else workR(u, son, cash.find(u->r[l+1]), l);
            } else {
                if (l > 0) workL(u, son, cash.find(u->r[l-1]), l);
                else workR(u, son, cash.find(u->r[l+1]), l);
            }
        }
    }
public:
    BPT(char ch[]) :cash(ch) {
                
    }
    std::pair<int, value> find(const Key& p) {  
        f_cnt = 0;
        find(cash.getroot(), p);
        std::pair<int, value> g;
        g.first = f_cnt; g.second = raddr;
        f_cnt = 0;
        return g;
    }
    void insert(const Key& p,const value& v) {
    	f_cnt=0;
        ins_retype o = insert(cash.getroot(), p, v);
        if (o.is_split) {                
            Node nd; 
            nd.is_leaf = 0; nd.siz = 1;
            nd.ind[0] = o.kk; nd.val[0] = o.val;
            nd.r[1] = o.rs; nd.r[0] = cash.getroot();
			size_t tt = cash.getpl(nd);
            cash.change_root(tt);
            //cout<<cash.getroot()<<" root"<<endl;
            //cout<<cash.getroot()<<" OO"<<endl;
        	Node* u = cash.find(cash.getroot());
		}
        return;
    }
    void del(const Key& p,const value& v) {
        del(cash.getroot(), p, v);        
        Node* np = cash.find(cash.getroot());
        if (np->siz == 0 && (!np->is_leaf)) {
            cash.change_root(np->r[0]);
        }
    }
    void print() {
    	dfs(cash.getroot());
	}
    ~BPT() {
        
    }
};  
#endif