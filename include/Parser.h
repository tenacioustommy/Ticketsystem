#ifndef PARSER_H
#define PARSER_H
#include<sstream>
#include"Account.hpp"
void Parser(std::string line){
    std::stringstream ss;
    std::string cmd[20];
    std::string buf;
    std::string input[10];
    int cmd_cnt=0;
    ss<<line;
    while(ss>>buf){
        //cmd[0]=timestamp
        cmd[cmd_cnt++]=buf;
    }
    if(cmd[1]=="add_user"){
        int tmp;
        for(int i=2;i<cmd_cnt;i+=2){
            if(cmd[i]=="-c"){
                input[1]=cmd[i+1];
            }else if(cmd[i]=="-u"){
                input[2]==cmd[i+1];
            }else if(cmd[i]=="-p"){
                input[3]==cmd[i+1];
            }else if(cmd[i]=="-n"){
                input[4]==cmd[i+1];
            }else if(cmd[i]=="-m"){
                input[5]==cmd[i+1];
            }else if(cmd[i]=="-g"){
                ss<<cmd[i+1];
                ss>>tmp;
            }
        }
        cout<<account.Adduser(Username_t(input[1]),Username_t(input[2]),Pass_t(input[3]),
                    Name_t(input[4]),Mail_t(input[5]),tmp);
        
    }else if(cmd[1]=="login"){
        for(int i=2;i<cmd_cnt;i+=2){
            if(cmd[i]=="-u"){
                input[1]==cmd[i+1];
            }else if(cmd[i]=="-p"){
                input[2]==cmd[i+1];
            }
        }
        cout<<account.Login(Username_t(input[1]),Pass_t(input[2]));
    }else if(cmd[1]=="logout"){
        for(int i=2;i<cmd_cnt;i+=2){
            if(cmd[i]=="-p"){
                input[1]==cmd[i+1];
            }
        }
        cout<<account.Logout(Username_t(input[1]));
    }else if(cmd[1]=="query_profile"){
        for(int i=2;i<cmd_cnt;i+=2){
            if(cmd[i]=="-c"){
                input[1]=cmd[i+1];
            }else if(cmd[i]=="-u"){
                input[2]==cmd[i+1];
            }
        }
        auto p=account.Query(Username_t(input[1]),Username_t(input[2]));
        if(p!=nullptr){
            cout<<*p;
        }else{
            cout<<-1;
        }
    }else if(cmd[1]=="modify_profile"){
        int tmp=-1;
        for(int i=2;i<cmd_cnt;i+=2){
            if(cmd[i]=="-c"){
                input[1]=cmd[i+1];
            }else if(cmd[i]=="-u"){
                input[2]==cmd[i+1];
            }else if(cmd[i]=="-p"){
                input[3]==cmd[i+1];
            }else if(cmd[i]=="-n"){
                input[4]==cmd[i+1];
            }else if(cmd[i]=="-m"){
                input[5]==cmd[i+1];
            }else if(cmd[i]=="-g"){
                ss<<cmd[i+1];
                ss>>tmp;
            }
        }
        auto p=account.Modify(Username_t(input[1]),Username_t(input[2]),Pass_t(input[3]),
                    Name_t(input[4]),Mail_t(input[5]),tmp);
        if(p!=nullptr){
            cout<<*p;
        }else{
            cout<<-1;
        }
    }
    

}
#endif