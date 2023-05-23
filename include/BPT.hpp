#ifndef BPT_H
#define BPT_H
#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
#define M 4
#define CAST(N) reinterpret_cast<char*>(N)
using std::cin,std::cout,std::endl,std::ios;
const int MAXSIZE=M,MINSIZE=M/2;
enum NODE{ROOTLEAF,LEAF,ROOT,BRANCH};

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
template<class Key,class Value>
class BPT
{
    typedef int pos_t;
private:
    struct Element
    {
        Key str;
        Value val; 
        Element() = default;
        Element(const Key &s, const Value &v) {
            str = s;
            val = v;
        }
        Element &operator=(const Element &x) {
            if (this == &x) return *this;
            str = x.str;
            val = x.val;
            return *this;
        }  /* data */
        bool operator==(const Element& rhs) const {
            return ((str==rhs.str)&&(val == rhs.val));      
        }
        bool operator!=(const Element& rhs) const {
            return !(*this == rhs);
        }
        bool operator<(const Element& rhs) const{
            if(str!=rhs.str){
                return (str<rhs.str);
            }
            return val<rhs.val;
        }
        bool operator>(const Element& rhs) const{
            if(str!=rhs.str){
                return (str>rhs.str);
            }
            return val>rhs.val;
        }
        bool operator<=(const Element& rhs) const{
            return !(*this>rhs);
        }
        bool operator>=(const Element& rhs) const{
            return !(*this<rhs);
        }
    };
    struct Block
    {
        //root can never be deleted from file
        NODE nodetype=LEAF;
        int size=0;
        //nextblockpos, only when isleaf==1, it has meaning
        pos_t nextblock=-1;
        //element num<=maxsize,i give one more for buffer
        Element data[MAXSIZE+1];
        //childpos
        pos_t child[MAXSIZE+2];
    };
    int sizeofblock= sizeof(Block);
    // int sizeofele=sizeof(Element);
private:
    pos_t smallpos;
    pos_t rootpos;
    //from root layer
    pos_t layer[5];
    int curlayer;
    /*available pos indicate where is available */
    pos_t avai=-1;
    std::fstream file;
    std::string filename;
    //rootpos by default at 0 pos
    void readblk(pos_t pos,Block& blk){
        file.seekg(pos);
        file.read(CAST(&blk),sizeofblock);
    }
    //return writepos
    pos_t writeblk(pos_t pos,Block& blk){
        file.seekp(pos);
        file.write(CAST(&blk),sizeofblock);
        if(pos==avai){
            avai=file.tellp();
        }
        return pos;
    }
    //insert ele in certain index in leafblk
    void insertchild(const pos_t& chd,Block& blk,int i){
        for(int j=blk.size;j>i;j--){
            blk.child[j]=blk.child[j-1];
        }
        blk.child[i]=chd;
    }
    void removechild(Block& blk,int i){
        for(int j=i;j<blk.size+1;j++){
            blk.child[j]=blk.child[j+1];
        }
    }
    void insertdata(const Element& ele,Block& blk,int i){
        for(int j=blk.size;j>i;j--){
            blk.data[j]=blk.data[j-1];
        }
        blk.data[i]=ele;
        blk.size++;
    }
    void removedata(Block& blk,int i){
        for(int j=i;j<blk.size-1;j++){
            blk.data[j]=blk.data[j+1];
        }
        blk.size--;
    }
    int findindex(const Element& ele,Block& blk){
        for(int i=0;i<blk.size+1;++i){
            if(i==blk.size||ele<blk.data[i]){
                return i;
            }
        }
        throw std::runtime_error("impossible");
    }
    //provide blk and blk's pos!!
    void insertadjust(Block& blk,int pos){
        Block parentblk;
        Block left,right;
        if(blk.nodetype==ROOTLEAF){
            blk.nodetype=ROOT;
            left.nodetype=LEAF;
            right.nodetype=LEAF;
            for(int i=0;i<MINSIZE;i++){
                left.data[i]=blk.data[i];
            }
            for(int i=MINSIZE,j=0;i<MAXSIZE+1;i++,j++){
                right.data[j]=blk.data[i];
            }
            left.size=MINSIZE;
            right.size=MINSIZE+1;
            left.nextblock=avai;
            //first write right then left
            blk.data[0]=blk.data[MINSIZE];
            blk.size=1;
            blk.child[1]=writeblk(avai,right);
            blk.child[0]=writeblk(avai,left);
            smallpos=blk.child[0];
            writeblk(rootpos,blk);
            return;
        }else if(blk.nodetype==ROOT){
            left.nodetype=BRANCH;
            right.nodetype=BRANCH;
            for(int i=0;i<MINSIZE;i++){
                left.data[i]=blk.data[i];
                left.child[i]=blk.child[i];
            }
            left.child[MINSIZE]=blk.child[MINSIZE];
            for(int i=MINSIZE+1,j=0;i<MAXSIZE+1;i++,j++){
                right.data[j]=blk.data[i];
                right.child[j]=blk.child[i];
            }
            right.child[MINSIZE]=blk.child[MAXSIZE+1];
            left.size=right.size=MINSIZE;
            
            blk.data[0]=blk.data[MINSIZE];
            blk.size=1;
            blk.child[0]=writeblk(avai,left);
            blk.child[1]=writeblk(avai,right);
            //dad not assigned
            writeblk(rootpos,blk);
            return;
        }else if(blk.nodetype==LEAF){
            readblk(layer[curlayer-1],parentblk);
            right.nodetype=LEAF;
            for(int i=MINSIZE,j=0;i<MAXSIZE+1;i++,j++){
                right.data[j]=blk.data[i];
            }
            blk.size=MINSIZE;
            right.size=MINSIZE+1;
            //change nextblock
            right.nextblock=blk.nextblock;
            blk.nextblock=avai;
            
            int i=findindex(right.data[0],parentblk);
            insertdata(right.data[0],parentblk,i);
            insertchild(avai,parentblk,i+1);
            writeblk(pos,blk);
            writeblk(avai,right);
            if(parentblk.size>MAXSIZE){
                insertadjust(parentblk,layer[--curlayer]);
            }else{
                writeblk(layer[curlayer-1],parentblk);
            }
            return;
        }else{
            readblk(layer[curlayer-1],parentblk);
            right.nodetype=BRANCH;
            for(int i=MINSIZE+1,j=0;i<MAXSIZE+1;i++,j++){
                right.data[j]=blk.data[i];
                right.child[j]=blk.child[i];
            }
            right.child[MINSIZE]=blk.child[MAXSIZE+1];
            blk.size=MINSIZE;
            right.size=MINSIZE;

            int i=findindex(blk.data[MINSIZE],parentblk);
            insertdata(blk.data[MINSIZE],parentblk,i);
            insertchild(avai,parentblk,i+1); 
            writeblk(pos,blk);
            writeblk(avai,right);
            if (parentblk.size>MAXSIZE)
            {   
                insertadjust(parentblk,layer[--curlayer]);/* code */
            }else{
                writeblk(layer[curlayer-1],parentblk);
            }
            return;
        }
    }
    bool rootadjust(Block& blk,Block& parentblk,bool leftorright){
        if(parentblk.size==0&&parentblk.nodetype==ROOT){
            //rootleaf
            if(curlayer==1){
                blk.nodetype=ROOTLEAF;
            }else{
                blk.nodetype=ROOT;
            }
            rootpos=parentblk.child[leftorright];
            writeblk(rootpos,blk);
            return 1;
        }else{
            return 0;
        }
    }
    void removeadjust(Block& blk,pos_t pos){
        Block parentblk,neighborblk;
        int index;
        if(blk.nodetype==LEAF){
            readblk(layer[curlayer-1],parentblk);
            index=findindex(blk.data[0],parentblk);
            if(index==0){
                readblk(parentblk.child[index+1],neighborblk);
                //borrow child directly
                if(neighborblk.size>MINSIZE){
                    insertdata(parentblk.data[0],blk,blk.size);
                    //first remove then fuzhi
                    removedata(neighborblk,0);
                    parentblk.data[0]=neighborblk.data[0];
                    // adjusthead();
                    writeblk(parentblk.child[0],blk);
                    writeblk(parentblk.child[1],neighborblk);
                    writeblk(layer[curlayer-1],parentblk);
                }else{
                    for(int i=blk.size,j=0;i<blk.size+MINSIZE;i++,j++){
                        blk.data[i]=neighborblk.data[j];
                    }
                    blk.size+=MINSIZE;
                    blk.nextblock=neighborblk.nextblock;
                    removedata(parentblk,0);
                    removechild(parentblk,1);
                    if(rootadjust(blk,parentblk,1)){
                        return;
                    }
                    writeblk(parentblk.child[0],blk);
                    if(parentblk.size<MINSIZE){
                        removeadjust(parentblk,layer[--curlayer]);
                        return;
                    }else{
                        writeblk(layer[curlayer-1],parentblk);
                    }
                }  
            }else{
                readblk(parentblk.child[index-1],neighborblk);
                if(neighborblk.size>MINSIZE){
                    insertdata(neighborblk.data[neighborblk.size-1],blk,0);
                    removedata(neighborblk,neighborblk.size-1);
                    parentblk.data[index-1]=blk.data[0];
                    writeblk(parentblk.child[index],blk);
                    writeblk(parentblk.child[index-1],neighborblk);
                    writeblk(layer[curlayer-1],parentblk);
                }else{
                    for(int i=neighborblk.size,j=0;i<neighborblk.size+MINSIZE-1;i++,j++){
                        neighborblk.data[i]=blk.data[j];
                    }
                    neighborblk.size+=MINSIZE-1;
                    neighborblk.nextblock=blk.nextblock;
                    removedata(parentblk,index-1);
                    removechild(parentblk,index);
                    
                    //parent is root and has no child at all,height-1
                    if(rootadjust(neighborblk,parentblk,0)){
                        return;
                    }
                    writeblk(parentblk.child[index-1],neighborblk);
                    if(parentblk.size<MINSIZE){
                        removeadjust(parentblk,layer[--curlayer]);
                        return;
                    }else{
                        writeblk(layer[curlayer-1],parentblk);
                    }
                }
            }
        }else if(blk.nodetype==BRANCH){
            readblk(layer[curlayer-1],parentblk);
            index=findindex(blk.data[0],parentblk);
            if(index==0){
                readblk(parentblk.child[index+1],neighborblk);
                //borrow child directly
                if(neighborblk.size>MINSIZE){
                    insertdata(parentblk.data[0],blk,blk.size);
                    parentblk.data[0]=neighborblk.data[0];
                    insertchild(neighborblk.child[0],blk,blk.size);
                    removedata(neighborblk,0);
                    removechild(neighborblk,0);
                    // adjusthead();
                    writeblk(parentblk.child[0],blk);
                    writeblk(parentblk.child[1],neighborblk);
                    writeblk(layer[curlayer-1],parentblk);
                }else{
                    insertdata(parentblk.data[0],blk,blk.size);
                    for(int i=blk.size,j=0;i<blk.size+MINSIZE;i++,j++){
                        blk.data[i]=neighborblk.data[j];
                        blk.child[i]=neighborblk.child[j];
                    }
                    blk.size+=MINSIZE;
                    blk.nextblock=neighborblk.nextblock;
                    blk.child[blk.size]=neighborblk.child[MINSIZE];
                    removedata(parentblk,0);
                    removechild(parentblk,1);
                    
                    //only condition that shortens the height
                    if(rootadjust(blk,parentblk,1)){
                        return;
                    }
                    writeblk(parentblk.child[0],blk);
                    // writeblk(blk.parentpos,parentblk);
                    if(parentblk.size<MINSIZE){
                        removeadjust(parentblk,layer[--curlayer]);
                        return;
                    }else{
                        writeblk(layer[curlayer-1],parentblk);
                    }
                }  
            }else{
                readblk(parentblk.child[index-1],neighborblk);
                if(neighborblk.size>MINSIZE){
                    insertdata(parentblk.data[index-1],blk,0);
                    parentblk.data[index-1]=neighborblk.data[neighborblk.size-1];
                    insertchild(neighborblk.child[neighborblk.size],blk,0);
                    removedata(neighborblk,neighborblk.size-1);
                    //child is at last no need to remove
                    writeblk(parentblk.child[index],blk);
                    writeblk(parentblk.child[index-1],neighborblk);
                    writeblk(layer[curlayer-1],parentblk);
                }else{
                    insertdata(parentblk.data[index-1],neighborblk,neighborblk.size);
                    for(int i=neighborblk.size,j=0;i<neighborblk.size+MINSIZE-1;i++,j++){
                        neighborblk.data[i]=blk.data[j];
                        neighborblk.child[i]=blk.child[j];
                    }
                    neighborblk.size+=MINSIZE-1;
                    neighborblk.nextblock=blk.nextblock;
                    neighborblk.child[neighborblk.size]=blk.child[blk.size];

                    removedata(parentblk,index-1);
                    removechild(parentblk,index);
                    if(rootadjust(neighborblk,parentblk,0)){
                        return;
                    }
                    writeblk(parentblk.child[index-1],neighborblk);
                    if(parentblk.size<MINSIZE){
                        removeadjust(parentblk,layer[--curlayer]);
                        return;
                    }else{
                        writeblk(layer[curlayer-1],parentblk);
                    }
                }
            }
        }else{
            writeblk(pos,blk);
        }
    }
    //modify previous index if first data is removed
    void adjusthead(){

    }
public:
    bool remove(const Key& key,const Value& val){
        Element ele(key,val);
        Block blk,tmpblk;
        pos_t elepos=-1;
        int index;
        curlayer=0;
        layer[curlayer++]=rootpos;
        readblk(layer[curlayer-1],blk);
        while (1)
        { 
            int i=findindex(ele,blk);
            
            //judge if it has child node
            if(blk.nodetype<=LEAF){
                //element doesn't exist
                if(i==0||blk.data[i-1]!=ele){
                    return 0;
                }
                i--;
                removedata(blk,i);
                if(elepos!=-1){
                    tmpblk.data[index]=blk.data[0];
                    writeblk(elepos,tmpblk);
                }
                if(blk.size<MINSIZE){
                    removeadjust(blk,layer[--curlayer]);
                }else{
                    writeblk(layer[curlayer-1],blk);
                }
                return 1;
            }else{
                if(i!=0&&blk.data[i-1]==ele){
                    tmpblk=blk;
                    index=i-1;
                    elepos=layer[curlayer-1];
                }
                layer[curlayer++]=blk.child[i];
                readblk(layer[curlayer-1],blk);
            }
        }
        
        
    }
    //success return pos fail return -1 todo
    int insert(const Key& key,const Value val){
        Element ele(key,val);
        Block blk;
        curlayer=0;
        layer[curlayer++]=rootpos;
        readblk(rootpos,blk);
        if(blk.size==0){
            insertdata(ele,blk,0);
            writeblk(rootpos,blk);
            return 0;
        }
        while(1){
            int i=findindex(ele,blk);
            //judge if it has child node
            if(blk.nodetype<=LEAF){
                //repeated element
                if(i!=0&&blk.data[i-1]==ele){
                    return -1;
                }
                insertdata(ele,blk,i);
                if(blk.size>MAXSIZE){
                    insertadjust(blk,layer[--curlayer]);
                }else{
                    writeblk(layer[curlayer-1],blk);
                }
                return 0;
            }else{
                layer[curlayer++]=blk.child[i];
                readblk(layer[curlayer-1],blk);
                
            }
        }
        return -1;
    }
    bool find(const Key& key,std::vector<Value>& vec){
        Block blk;
        int pos=rootpos;
        int index;
        while(1){
            readblk(pos,blk);
            for(index=0;index<blk.size;index++){
                //here is <= because there maybe the same key at left side
                if(key<=blk.data[index].str){
                    break;
                }
            }
            if(blk.nodetype<=LEAF){
                while(1){
                    while(index<blk.size){
                        if(blk.data[index].str==key){
                            vec.push_back(blk.data[index].val);
                            index++;
                        }else{
                            return !vec.empty();
                        }
                    }
                    //maybe key is also in next block
                    if(blk.nextblock==-1){
                        return !vec.empty();
                    }else{
                        readblk(blk.nextblock,blk);
                        index=0;
                    }
                }
            }else{
                pos=blk.child[index];
            }
            
        }
    }
    BPT(const char* filena){
        filename=filena;
        file.open(filename,ios::binary|ios::in|ios::out);
        if(!file){
            file.open(filename,ios::out);
            file.close();
            file.open(filename,ios::binary|ios::in|ios::out);
            rootpos=smallpos=8;
            file.write(CAST(&rootpos),sizeof(int));
            file.write(CAST(&smallpos),sizeof(int));
            Block blk;
            //root pos never changes!! and never delete
            blk.nodetype=ROOTLEAF;
            file.write(CAST(&blk),sizeofblock);
            avai=file.tellp();
        }else{
            file.read(CAST(&rootpos),sizeof(int));
            file.read(CAST(&smallpos),sizeof(int));
            file.seekg(0,ios::end);
            avai=file.tellg();
        }
        if(!file){
            throw std::runtime_error("file error");
        }
    }
    ~BPT(){
        file.seekp(0);
        file.write(CAST(&rootpos),sizeof(int));
        file.write(CAST(&smallpos),sizeof(int));
        file.close();
    }
};


#endif