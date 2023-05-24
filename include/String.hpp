#ifndef STRING_H
#define STRING_H
#include<ostream>
#include<cstring>
template<int SIZE>
struct string {
    char buf[SIZE+1];
    string(){buf[0]='\0';}
    string(const char* str){
        strncpy(buf,str,SIZE);
        buf[SIZE]='\0';
    }
    string(const std::string str){
        strcpy(buf,str.c_str());
    }
    bool isempty()const{
        if(buf[0]=='\0'){
            return 1;
        }else return 0;
    }
    bool operator< (const string &rhs) const {
        return strcmp(buf, rhs.buf) < 0;
    }
    bool operator> (const string &rhs) const {
        return strcmp(buf, rhs.buf) > 0;
    }
    bool operator>= (const string &rhs) const {
        return strcmp(buf, rhs.buf) >= 0;
    }
    bool operator<= (const string &rhs) const {
        return strcmp(buf, rhs.buf) <= 0;
    }
    bool operator== (const string &rhs) const {
        return strcmp(buf, rhs.buf) == 0;
    }
    bool operator!= (const string &rhs) const {
        return strcmp(buf, rhs.buf) != 0;
    }
    string& operator= (const string &rhs){
        if(this==&rhs) return *this;
        strcpy(buf,rhs.buf);
        return *this;
    }
};
template<int SIZE>
std::ostream& operator<<(std::ostream& out,const string<SIZE>& str){
    out<<str.buf;
    return out;
}
#endif