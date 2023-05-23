#include"BPT.hpp"
int main(){
    int num;
    std::string instruction;
    ios::sync_with_stdio(false);
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
    
    // system("pause");
    return 0;
}

// using namespace std;
// int K = 1e4;

// int main()
// {
// ios::sync_with_stdio(0);
//  freopen("input.dat", "w", stdout);
// cout << 5*K << "\n";
//  for (int i = 0; i < 2*K; i++)
// cout << "insert " << "Kiss" << i << ' ' << i << '\n';
//  for (int i = 2345; i < K+2345; i++)
// cout << "delete " << "Kiss" << i << ' ' << i << '\n';
// for (int i = 0; i < 2*K; i++)
// cout << "find " << "Kiss" << i << '\n';
// }