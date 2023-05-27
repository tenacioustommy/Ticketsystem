#ifndef FILE_H
#define FILE_H
#include<fstream>
using std::ios;
class File
{
    using Pos_t=int;
private:
    std::fstream file;
public:
    Pos_t tellg(){
        return file.tellg();
    }
    Pos_t tellp(){
        return file.tellp();
    }
    void seekg(Pos_t pos,ios::seekdir where=ios::beg){
        file.seekg(pos,where);
    }
    void seekp(Pos_t pos,ios::seekdir where=ios::beg){
        file.seekp(pos,where);
    }
    void write(const char* buf,int size,Pos_t pos,ios::seekdir where=ios::beg){
        file.seekp(pos,where);
        file.write(buf,size);
    }
    void read(char* buf,int size,Pos_t pos,ios::seekdir where=ios::beg){
        file.seekg(pos,where);
        file.read(buf,size);
    }
    //new file return 1,else return 0
    bool init(std::string filename){
        bool flag=false;
        file.open(filename,ios::binary|ios::in|ios::out);
        if(!file){
            file.open(filename,ios::out);
            file.close();
            file.open(filename,ios::binary|ios::in|ios::out);
            flag=true;
        }
        if(!file){
            throw std::runtime_error("file error");
        }
        return flag;
    }
    File()=default;
    ~File(){
        file.close();
    }
};

#endif