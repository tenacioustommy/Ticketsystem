#ifndef ACCOUNT_H
#define ACCOUNT_H
#include<map>
#include"BPT.hpp"
using Username_t=string<20>;
using Pass_t=string<30>; 
using Name_t=string<15>;
using Mail_t=string<30>;
class Account
{
using Pos_t= int;
using Logpair=std::pair<Username_t,int>;
    
private:
    struct User
    {
        //unique 
        Username_t username;
        Pass_t pwd;
        Name_t name; 
        Mail_t mail;
        //0~10
        int pvil;
        User()=default;
        User(const Username_t& tmpusername,const Pass_t& tmppwd,
            const Name_t& tmpname,const Mail_t& tmpmail,const int& tmppvil):
            username(tmpusername),pwd(tmppwd),name(tmpname),mail(tmpmail),pvil(tmppvil){}
    };
    friend std::ostream& operator<<(std::ostream& out,const Account::User& user);
    int sizeofuser=sizeof(User);
    std::string filename="output/accfile";

private:
    User tmpuser;
    BPT<Username_t,Pos_t> accindex;
    std::fstream accfile;
    //put username and privilege
    std::map<Username_t,int> logged;
    int num;
private:
    //write userinfo into accfile and accindex
    void writenewuser(User& user){
        accfile.seekp(0,ios::end);
        accindex.insert(user.username,accfile.tellp());
        accfile.write(CAST(&user),sizeofuser);
        num++;
    }
    void readuser(User& user,const Pos_t& pos){
        accfile.seekg(pos);
        accfile.read(CAST(&user),sizeofuser);
    }
    void reviseuser(User& user,const Pos_t& pos){
        accfile.seekp(pos);
        accfile.write(CAST(&user),sizeofuser);
    }
public:
    User* Modify(const Username_t& curusername,const Username_t& tmpusername,const Pass_t& tmppwd=Pass_t(),
            const Name_t tmpname=Name_t(),const Mail_t tmpmail=Mail_t(),const int tmppvil=-1){
        auto it=logged.find(curusername);
        Pos_t pos;
        if(it!=logged.end()&&accindex.find(tmpusername,pos)){
            User user;
            readuser(user,pos);
            if(it->second>user.pvil||curusername==tmpusername){
                if(!tmppwd.isempty()){
                    user.pwd=tmppwd;
                }
                if(!tmpname.isempty()){
                    user.name=tmpname;
                }
                if(!tmpmail.isempty()){
                    user.mail=tmpmail;
                }
                if(tmppvil!=-1){
                    if(tmppvil<it->second){
                        user.pvil=tmppvil;
                    }else{
                        return nullptr;
                    }
                }
                tmpuser=user;
                reviseuser(user,pos);
                return &tmpuser;
            }else{
                return nullptr;
            }
        }else{
            return nullptr;
        }
    }
    User* Query(const Username_t& curusername,const Username_t& tmpusername){
        auto it=logged.find(curusername);
        Pos_t pos;
        if(it!=logged.end()&&accindex.find(tmpusername,pos)){
            User user;
            readuser(user,pos);
            if(it->second>user.pvil||curusername==tmpusername){
                tmpuser=user;
                return &tmpuser;
            }else{
                return nullptr;
            }
        }else{
            return nullptr;
        }
    }
    int Logout(const Username_t& tmpusername){
        auto it=logged.find(tmpusername);
        if(it!=logged.end()){
            logged.erase(it);
            return 0;
        }else{
            return -1;
        }
    }
    int Login(const Username_t& tmpusername,const Pass_t& tmppwd){
        Pos_t pos;
        //haven't logged in and username exist
        if(logged.find(tmpusername)==logged.end()&&accindex.find(tmpusername,pos)){
            //check pwd
            User user;
            readuser(user,pos);
            if(user.pwd==tmppwd){
                logged.insert(Logpair(tmpusername,user.pvil));
                return 0;
            }else{
                return -1;
            }
        }else{
            return -1;
        }
    }
    int Adduser(const Username_t& curusername,const Username_t& tmpusername,const Pass_t& tmppwd,
            const Name_t& tmpname,const Mail_t& tmpmail,int tmppvil){
        User user(tmpusername,tmppwd,tmpname,tmpmail,tmppvil);
        if(num==0){
            user.pvil=10;
            writenewuser(user);
            return 0;
        }else{
            auto it=logged.find(curusername);
            //judge curuser exist and pvil valid
            if(it!=logged.end()&&it->second>tmppvil){
                //judge newusername is unique
                Pos_t tmp;
                if(!accindex.find(tmpusername,tmp)){
                    writenewuser(user);
                    return 0;
                }else{
                    return -1;
                }
            }else{
                return -1;
            }
        }
    }
    Account():accindex("output/accindex"){
        accfile.open(filename,ios::binary|ios::in|ios::out);
        if(!accfile){
            accfile.open(filename,ios::out);
            accfile.close();
            accfile.open(filename,ios::binary|ios::in|ios::out);
            num=0;
            accfile.seekp(0);
            accfile.write(CAST(&num),sizeof(int));
        }else{
            accfile.read(CAST(&num),sizeof(int));
        }
        if(!accfile){
            throw std::runtime_error("accfile error");
        }
    }
    ~Account(){
        accfile.seekp(0);
        accfile.write(CAST(&num),sizeof(int));
    }
};
std::ostream& operator<<(std::ostream& out,const Account::User& user){
    out<<user.username<<" "<<user.name<<" "<<user.mail<<" "<<user.pvil;
    return out;
}
Account account;
#endif