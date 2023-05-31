#ifndef ACCOUNT_H
#define ACCOUNT_H
#include"BPT.hpp"
#include"Header.h"
#include"File.h"
class Account
{
    friend class Ticketsys;
private:
    struct User
    {
        //unique 
        Username_t username;
        Pass_t pwd;
        Name_t name; 
        Mail_t mail;
        //0~10
        short pvil;
        User()=default;
        User(const Username_t& tmpusername,const Pass_t& tmppwd,
            const Name_t& tmpname,const Mail_t& tmpmail,const int& tmppvil):
            username(tmpusername),pwd(tmppwd),name(tmpname),mail(tmpmail),pvil(tmppvil){}
    };
    friend std::ostream& operator<<(std::ostream& out,const Account::User& user);
    int sizeofuser=sizeof(User);

private:
    User tmpuser;
    BPT<Username_t,Pos_t> accindex;
    File accfile;
    //put username and privilege
    map<Username_t,int> logged;
    int num;
private:
    //write userinfo into accfile and accindex
    void writenewuser(User& user){
        accfile.seekp(0,ios::end);
        accindex.insert(user.username,accfile.tellp());
        accfile.write(CAST(&user),sizeofuser,0,ios::end);
        num++;
    }
    int Modify(const Username_t& curusername,const Username_t& tmpusername,const Pass_t& tmppwd,
            const Name_t& tmpname,const Mail_t& tmpmail,const int tmppvil=-1){
        auto it=logged.find(curusername);
        Pos_t pos;
        if(it!=logged.end()&&accindex.find(tmpusername,pos)){
            User user;
            accfile.read(CAST(&user),sizeofuser,pos);
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
                        return -1;
                    }
                }
                accfile.write(CAST(&user),sizeofuser,pos); 
                cout<<user<<"\n";
                return 0;
            }else{
                return -1;
            }
        }else{
            return -1;
        }
    }
    int Query(const Username_t& curusername,const Username_t& tmpusername){
        auto it=logged.find(curusername);
        Pos_t pos;
        if(it!=logged.end()&&accindex.find(tmpusername,pos)){
            User user;
            accfile.read(CAST(&user),sizeofuser,pos);
            if(it->second>user.pvil||curusername==tmpusername){
                cout<<user<<"\n";
                return 0;
            }else{
                return -1;
            }
        }else{
            return -1;
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
            accfile.read(CAST(&user),sizeofuser,pos);
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
public:
    bool islogged(const Username_t& username){
        if(logged.find(username)!=logged.end()){
            return 1;
        }else return 0;
    }
    int Modify(const std::string& curusername,const std::string& tmpusername,const Pass_t& tmppwd,
            const std::string& tmpname,const std::string& tmpmail,const std::string& tmppvil){
        if(tmppvil.empty()){
            return Modify(Username_t(curusername),Username_t(tmpusername),Pass_t(tmppwd),
            Name_t(tmpname),Mail_t(tmpmail));
        }else{
            return Modify(Username_t(curusername),Username_t(tmpusername),Pass_t(tmppwd),
            Name_t(tmpname),Mail_t(tmpmail),std::stoi(tmppvil));
        }
    }
    int Query(const std::string& curusername,const std::string& tmpusername){
        return Query(Username_t(curusername),Username_t(tmpusername));
    }
    
    int Logout(const std::string& tmpusername){
        return Logout(Username_t(tmpusername));
    }
    
    int Login(const std::string& tmpusername,const Pass_t& tmppwd){
        return Login(Username_t(tmpusername),Pass_t(tmppwd));
    }
    
    int Adduser(const std::string& curusername,const std::string& tmpusername,const Pass_t& tmppwd,
            const std::string& tmpname,const std::string& tmpmail,const std::string& tmppvil){
        return Adduser(Username_t(curusername),Username_t(tmpusername),Pass_t(tmppwd),
            Name_t(tmpname),Mail_t(tmpmail),std::stoi(tmppvil));
    }
    Account():accindex("output/accindex"){
        if(accfile.init(path+"accfile")){
            num=0;
            accfile.write(CAST(&num),sizeof(int),0);
        }else{
            accfile.read(CAST(&num),sizeof(int),0);
        }
    }
    ~Account(){
        accfile.write(CAST(&num),sizeof(int),0);
    }
};
std::ostream& operator<<(std::ostream& out,const Account::User& user){
    out<<user.username<<" "<<user.name<<" "<<user.mail<<" "<<user.pvil;
    return out;
}
Account account;
#endif