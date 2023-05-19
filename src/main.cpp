#include"BPT.hpp"
int main(){
    int num;
    std::string instruction;
    cin.sync_with_stdio(false);
    BPT<string<64>,int> test("test");
    cin>>num;
    string<64> index;
    for(int i=0;i<num;i++){
        cin>>instruction;
        if(instruction=="insert"){
            int value;
            std::cin>>index.buf>>value;
            test.insert(index,value);
        }else if(instruction=="delete"){
            int value;
            std::cin>>index.buf>>value;
            test.remove(index,value);
        }else if(instruction=="find"){
            std::cin>>index.buf;
            std::vector<int> vec;
            test.find(index,vec);
            if(vec.empty()){
                cout<<"null\n";
            }else{
                for(auto ele:vec ){
                cout<<ele<<" ";
                }
                cout<<"\n";
            }
            
        }
    }
    system("pause");
}