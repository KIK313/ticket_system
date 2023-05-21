#ifndef simple_file_plus_LRU
#define simple_file_plus_LRU

#include <cstddef>
#include<iostream>
#include<cstdio>
#include<fstream>
#include<cstring>
#include<string>
template <class val> 
class file {
private:
    class LRU {
    private:    
        std::fstream rw;
        int siz;
        int M;
        size_t root_pos; 
        struct nd_link {
            val* np; // ptr in internal memory
            size_t pos; // position in external memory 
            nd_link* hp; // hash pre;
            nd_link* hn; // hash next
            nd_link* tp; // time pre
            nd_link* tn; // time next
            nd_link(size_t poss=0): pos(poss){ np = nullptr; hp = hn = tp = tn = nullptr;}
            ~nd_link() {if (np != nullptr) delete np;}
        };
        nd_link Thead,Ttail;
        nd_link Hhead[499],Htail[499];
    public:
        LRU(char ch[], int MM) :M(MM) {
            siz = 0;
            rw.open(ch,std::ios::in|std::ios::out|std::ios::binary);
            if (!rw) {
                rw.open(ch,std::ios::out);
                rw.close(); rw.open(ch,std::ios::in|std::ios::out|std::ios::binary);
            }
            Thead.tn = &Ttail; Thead.tp = nullptr; Ttail.tp = &Thead; Ttail.tn = nullptr;
            for (int i = 0; i < 499; ++i) {
                Hhead[i].hp = Htail[i].hn = nullptr;
                Hhead[i].hn = &Htail[i]; Htail[i].hp = &Hhead[i];
            }
        }
        bool is_empty() {
            return (rw.tellg() == 0);
        }
        void rt(nd_link* u) {
            rw.seekp(u->pos);
            rw.write(reinterpret_cast<const char*>(u->np), sizeof(val));
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
        val rd(size_t pl) {
            rw.seekg(pl);
            val nd;
            rw.read(reinterpret_cast<char *>(&nd),sizeof(val));
            return nd;
        }
        size_t getpl(const val& u) {
            rw.seekp(0,std::ios::end);
            size_t pl = rw.tellp();
            rw.write(reinterpret_cast<const char*>(&u), sizeof(val));
            return pl;
        }
        val* find(size_t pl) {
            int s = pl % 499;
            nd_link* w = &Hhead[s]; nd_link* lp = &Htail[s];
            while (w->hn != lp) {
                w = w->hn;
                if (w->pos == pl) {
                    move_to_front(w); 
                    return w->np;
                }
            }
            if (siz == M) {
                nd_link* u = Ttail.tp;
                del(u);
            } else siz++;
            nd_link* nd = new nd_link(pl);
            nd->np = new val(rd(pl));
            nd->tn = Thead.tn; nd->tp = &Thead;
            (nd->tn)->tp = nd; Thead.tn = nd;
            nd_link* r = &Hhead[s];
            nd->hn = r->hn; nd->hp = r;
            r->hn->hp = nd; r->hn = nd;
            return nd->np;
        } 
        ~LRU() {
            siz = 0; 
            nd_link* p = Thead.tn; nd_link* pl = &Ttail;
            while (p != pl) {
                nd_link* q = p->tn;
                rt(p);
                delete p;
                p = q;
            }                
            rw.close();
        }
    }cache;
public:
    file(char ch[], int size_LRU) :cache(ch, size_LRU) {}
    val* read(size_t pl) {
        return cache.find(pl);
    }
    size_t write(const val& nd) {
        return cache.getpl(nd);
    }
    bool is_empty() {
        return cache.is_empty();
    }
    ~file() {
    }
};
#endif