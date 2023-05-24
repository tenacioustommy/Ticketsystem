#include<sstream>
#include<filesystem>
#include"BPT.hpp"
#include"Account.hpp"

namespace fs=std::filesystem;

int main(){
    std::freopen("data/normal/basic_2/1.in", "r", stdin);
    std::freopen("1.out","w",stdout);
    
    ios::sync_with_stdio(false);
    std::stringstream ss;
    std::string cmd[30];
    std::string buf,line;
    std::string input[10];
    
    while(std::getline(cin,line)){
        int cmd_cnt=0;
        ss.clear();
        ss<<line;
        while(ss>>buf){
            //cmd[0]=timestamp
            cmd[cmd_cnt++]=buf;
        }
        //used for debug
        if(cmd[0]=="[1703]"){
            int a=1;
        }
        //end debug
        if(cmd[1]=="add_user"){
            int tmp;
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-c"){
                    input[1]=cmd[i+1];
                }else if(cmd[i]=="-u"){
                    input[2]=cmd[i+1];
                }else if(cmd[i]=="-p"){
                    input[3]=cmd[i+1];
                }else if(cmd[i]=="-n"){
                    input[4]=cmd[i+1];
                }else if(cmd[i]=="-m"){
                    input[5]=cmd[i+1];
                }else if(cmd[i]=="-g"){
                    ss.clear();
                    ss<<cmd[i+1];
                    ss>>tmp;
                }
            }
            cout<<account.Adduser(Username_t(input[1]),Username_t(input[2]),Pass_t(input[3]),
                        Name_t(input[4]),Mail_t(input[5]),tmp)<<"\n";
            
        }else if(cmd[1]=="login"){
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-u"){
                    input[1]=cmd[i+1];
                }else if(cmd[i]=="-p"){
                    input[2]=cmd[i+1];
                }
            }
            cout<<account.Login(Username_t(input[1]),Pass_t(input[2]))<<"\n";
        }else if(cmd[1]=="logout"){
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-u"){
                    input[1]=cmd[i+1];
                }
            }
            cout<<account.Logout(Username_t(input[1]))<<"\n";
        }else if(cmd[1]=="query_profile"){
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-c"){
                    input[1]=cmd[i+1];
                }else if(cmd[i]=="-u"){
                    input[2]=cmd[i+1];
                }
            }
            auto p=account.Query(Username_t(input[1]),Username_t(input[2]));
            if(p!=nullptr){
                cout<<*p<<"\n";
            }else{
                cout<<-1<<"\n";
            }
        }else if(cmd[1]=="modify_profile"){
            int tmp=-1;
            for(int i=1;i<6;i++){
                input[i].clear();
            }
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-c"){
                    input[1]=cmd[i+1];
                }else if(cmd[i]=="-u"){
                    input[2]=cmd[i+1];
                }else if(cmd[i]=="-p"){
                    input[3]=cmd[i+1];
                }else if(cmd[i]=="-n"){
                    input[4]=cmd[i+1];
                }else if(cmd[i]=="-m"){
                    input[5]=cmd[i+1];
                }else if(cmd[i]=="-g"){
                    ss.clear();
                    ss<<cmd[i+1];
                    ss>>tmp;
                }
            }
            auto p=account.Modify(Username_t(input[1]),Username_t(input[2]),Pass_t(input[3]),
                        Name_t(input[4]),Mail_t(input[5]),tmp);
            if(p!=nullptr){
                cout<<*p<<"\n";
            }else{
                cout<<-1<<"\n";
            }
        }else if(cmd[1]=="exit"){
            cout<<"bye\n";
            break;
        }
    }
    
    return 0;
}
// int main(){
//     // std::freopen("input.dat","r",stdin);
//     // std::freopen("output.dat","w",stdout);
//     int num;
//     std::string instruction;
//     ios::sync_with_stdio(false);
//     BPT<string<64>,int> test("test");
//     cin>>num;
//     string<64> index;
    
//     for(int i=0;i<num;i++){
//         cin>>instruction;
//         if(instruction=="insert"){
//             int value;
//             std::cin>>index.buf>>value;
//             test.insert(index,value);
//         }else if(instruction=="delete"){
//             int value;
//             std::cin>>index.buf>>value;
//             test.remove(index,value);
//         }else if(instruction=="find"){
//             std::cin>>index.buf;
//             std::vector<int> vec;
//             test.findall(index,vec);
//             if(vec.empty()){
//                 cout<<"null\n";
//             }else{
//                 for(auto ele:vec ){
//                 cout<<ele<<" ";
//                 }
//                 cout<<"\n";
//             }
            
//         }
//     }
    
//     // system("pause");
//     return 0;
// }
// int main(){
//     // std::freopen("input.dat","r",stdin);
//     // std::freopen("output.dat","w",stdout);
//     int num;
//     std::string instruction;
//     ios::sync_with_stdio(false);
//     BPT<string<64>,int> test("test");
//     cin>>num;
//     string<64> index;
    
//     for(int i=0;i<num;i++){
//         cin>>instruction;
//         if(instruction=="insert"){
//             int value;
//             std::cin>>index.buf>>value;
//             test.insert(index,value);
//         }else if(instruction=="delete"){
//             int value;
//             std::cin>>index.buf>>value;
//             test.remove(index,value);
//         }else if(instruction=="find"){
//             std::cin>>index.buf;
//             std::vector<int> vec;
//             test.findall(index,vec);
//             if(vec.empty()){
//                 cout<<"null\n";
//             }else{
//                 for(auto ele:vec ){
//                 cout<<ele<<" ";
//                 }
//                 cout<<"\n";
//             }
            
//         }
//     }
    
//     // system("pause");
//     return 0;
// }

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