#include <ctime>
#include<iostream>
#include<cstdio>
#include<fstream>
#include<cstring>
#include<string>
#include"user.hpp"
#include"train.hpp"
#include"time.hpp"
using namespace std;
char time_stamp[105],op[105];
void rd(ins& im, char* ch) {
    scanf("%s",ch);
    while(ch[0] != '[') {
        if (ch[1] == 'c') {
            scanf("%s",im.c);    
        }
        if (ch[1] == 'u') {
            scanf("%s",im.user_id);
        }
        if (ch[1] == 'n') {
            scanf("%s",im.user_name);
        }
        if (ch[1] == 'p') {
            scanf("%s",im.password);
        }
        if (ch[1] == 'm') {
            scanf("%s",im.email);
        }
        if (ch[1] == 'g') {
            scanf("%d",&im.pri);
        }
        scanf("%s",ch);
    }
    return;
}
char tmp_char[4010];
void add_rd(train& tr, char* ch) {
    scanf("%s",ch); 
    while(ch[0] != '[') {
        if (ch[1] == 'i') {
            scanf("%s",tr.train_id);
        }
        if (ch[1] == 'n') {
            scanf("%d",&tr.station_num);
        }
        if (ch[1] == 'm') {
            scanf("%d",&tr.seat_num);
        }
        if (ch[1] == 's') {
            scanf("%s",tmp_char);
            int cnt = 0; int len = strlen(tmp_char);
            int nl = 0;
            for (int i = 0; i < len; ++i) {
                if (tmp_char[i] == '|') {
                    tr.name[cnt][nl] = '\0';    
                    cnt++; nl = 0;
                } else tr.name[cnt][nl++] = tmp_char[i];
            }
            tr.name[cnt][nl] = '\0';
        }
        if (ch[1] == 'p') {
            scanf("%s",tmp_char);
            int cnt = 0; int len = strlen(tmp_char); int pr = 0;
            tr.price[0] = 0;
            for (int i = 0; i < len; ++i) {
                if (tmp_char[i] == '|') {
                    tr.price[cnt+1] = tr.price[cnt] + pr;
                    pr = 0; cnt++;
                } else pr = pr * 10 + tmp_char[i] - '0';
            }
            tr.price[cnt+1] = tr.price[cnt] + pr;    
        }
        if (ch[1] == 'x') {
            scanf("%s",tmp_char);
            tr.start_min = (tmp_char[0]-'0') * 10 * 60 + (tmp_char[1]-'0') * 60
                            + (tmp_char[3]-'0') * 10 + (tmp_char[4]-'0'); 
        }
        if (ch[1] == 't') {
            scanf("%s",tmp_char);
            int cnt = 1; int len = strlen(tmp_char); int tm = 0;
            for (int i = 0; i < len; ++i) {
                if (tmp_char[i] == '|') {
                    tr.arr_time[cnt] = tm;
                    tm = 0; cnt++;
                } else tm = tm * 10 + tmp_char[i] - '0';
            }
            tr.arr_time[cnt] = tm;            
        }
        if (ch[1] == 'o') {
            scanf("%s", tmp_char);
            if (tmp_char[0] != '_') {
                int cnt = 1; int len = strlen(tmp_char); int tm = 0;
                for (int i = 0; i < len; ++i) {
                    if (tmp_char[i] == '|') {
                        tr.lev_time[cnt] = tm;
                        tm = 0; cnt++;
                    } else tm = tm * 10 + tmp_char[i] - '0';
                }
                tr.lev_time[cnt] = tm;                
            } 
        }
        if (ch[1] == 'd') {
            scanf("%s",tmp_char);
            tr.start_day = getday(tmp_char);
            tr.ed_day = getday(tmp_char + 6);  
        }
        if (ch[1] == 'y') {
            scanf("%s",tmp_char);
            tr.type = tmp_char[0];
        }
        scanf("%s",ch);
    }
    tr.lev_time[0] = tr.start_min; tr.lev_time[tr.station_num - 1] = 0;
    for (int i = 1; i < tr.station_num; ++i) 
        tr.arr_time[i] = tr.lev_time[i-1] + tr.arr_time[i], tr.lev_time[i] += tr.arr_time[i];

}
void qt_rd(train& tr,char* ch) {
    scanf("%s",ch);
    while (ch[0] != '[') {
        if (ch[1] == 'i') {
            scanf("%s", tr.train_id);
        } else {
            scanf("%s",tmp_char);
            tr.start_day = getday(tmp_char);            
        }
        scanf("%s",ch);
    }
}
void tr_rd(char* ch, char* st) {
    scanf("%s",st);
    scanf("%s",ch);
    scanf("%s",st);
}
void qr_rd(qr& qr, char* ch) {
    scanf("%s",ch); qr.is_time = 1;
    while (ch[0] != '[') {
        if (ch[1] == 's') {
            scanf("%s",qr.st);
        }
        if (ch[1] == 't') {
            scanf("%s",qr.ed);
        }
        if (ch[1] == 'd') {
            scanf("%d",&qr.st_day);
        }
        if (ch[1] == 'p') {
            scanf("%s",tmp_char);
            if (tmp_char[0] == 'c') qr.is_time = 0;
        }
        scanf("%s",ch);
    }
}
int main() {
    freopen("a.in","r",stdin);
    freopen("a.out","w",stdout);
    scanf("%s",time_stamp);
    char cc[]="id_addr"; char ccc[]="file";
    char _t[]="train_inf"; char _tt[]="ticket_inf"; char _ttt[]="noreal"; char _tttt[]="real";
    char rr[]="pass_tr";
    users U(cc,ccc); trains T(_t,_tt,_ttt,_tttt,rr);
    while(1) {
        int ll = strlen(time_stamp);
        for (int i = 0; i < ll; ++i) printf("%c",time_stamp[i]);
        printf(" ");
        int stt = 0;
        for (int i = 1; i < ll-1; ++i) stt = stt * 10 + time_stamp[i] - '0';   
        scanf("%s",op);
        if (op[0] == 'e') {  //exit

            printf("bye\n");
            break;
        }
        if (op[0] == 'a' && op[4] == 'u') { //add_user
            ins im; 
            rd(im,time_stamp);
            U.add_user(im);
            continue;
        }
        if (op[0] == 'l' && strlen(op) == 5) { // login 
            ins im;
            rd(im,time_stamp);
            U.login(im);
            continue;
        }  
        if (op[0] == 'l' && strlen(op) == 6) { // logout
            ins im;
            rd(im,time_stamp);
            U.logout(im);
            continue;
        }
        if (op[0] == 'q') {
            if (op[6] == 'p') { // query_profile
                ins im;
                rd(im,time_stamp);
                U.query_profile(im);
            }
            if (op[6] == 'o') { // query_order

            }
            if (op[6] == 't') {
                if (op[9] == 'i') { // query_train
                    train tr;
                    add_rd(tr, time_stamp);
                    T.query_train(tr);
                }
                if (op[9] == 'k') { // query_ticket
                    qr qr;
                    qr_rd(qr, time_stamp);
                    T.query_ticket(qr);
                }
                if (op[9] == 'n') { // query_transfer
                    qr qr;
                    qr_rd(qr, time_stamp);
                    T.query_transfer(qr);
                }
            }
            continue;
        }
        if (op[0] == 'm') { //modify_profile
            ins im;
            rd(im,time_stamp);
            U.modify_profile(im);
            continue;
        }
        if (op[0] == 'a') { //add_train
            train tr;
            add_rd(tr, time_stamp);
            T.add_train(tr);
            continue;
        }
        if (op[0] == 'd') { // delete_train
            char tr_id[22];
            tr_rd(tr_id, time_stamp);
            T.delete_train(tr_id);
            continue;
        }
        if (op[0] == 'r' && op[2] == 'l') { //release_train
            char tr_id[22];
            tr_rd(tr_id, time_stamp);
            T.release_train(tr_id);
            continue;
        }
        if (op[0] == 'b') { // buy_ticket
             
            continue;
        }
        if (op[0] == 'r' && op[2] == 'f') { // refund_ticket
            continue;
        }
        if (op[0] == 'c') { // clean

            continue;
        }
    }
    fclose(stdin);
    fclose(stdout);
    return 0;
}