#include<iostream>
#include<cstdio>
#include<fstream>
#include<cstring>
#include<string>
#include"user.hpp"
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
int main() {
    freopen("a.in","r",stdin);
    freopen("a.out","w",stdout);
    scanf("%s",time_stamp);
    char cc[]="id_addr"; char ccc[]="file";
    users U(cc,ccc);
    while(1) {
        int ll = strlen(time_stamp);
        for (int i = 0; i < ll; ++i) printf("%c",time_stamp[i]);
        printf(" ");   
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

                }
                if (op[9] == 'k') { // query_ticket

                }
                if (op[9] == 'n') { // query_transfer

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

            continue;
        }
        if (op[0] == 'd') { // delete_train

            continue;
        }
        if (op[0] == 'r' && op[2] == 'l') { //release_train

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