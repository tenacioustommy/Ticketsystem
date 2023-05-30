#include"BPT.hpp"
#include"Ticketsys.h"
int main(){
    // std::freopen("data/basic_2/1.in", "r", stdin);
    // std::freopen("1.out","w",stdout);
    ios::sync_with_stdio(false);
    std::string cmd[30];
    std::string buf,line;
    std::string input[15];
    
    while(std::getline(cin,line)){
        int cmd_cnt=0;
        buf.clear();
        int start=0,end=0;
        while(end<line.size()){
            while(line[end]!=' '&&line[end]!='\n'){
                end++;
            }
            cmd[cmd_cnt++]=line.substr(start,end-start);
            end++;
            start=end;
        }
        //used for debug
        cout<<cmd[0]<<" ";
        if(cmd[1]=="add_user"){
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
                    input[6]=cmd[i+1];
                }
            }
            cout<<account.Adduser(input[1],input[2],input[3],input[4],input[5],input[6])<<"\n";
            
        }else if(cmd[1]=="login"){
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-u"){
                    input[1]=cmd[i+1];
                }else if(cmd[i]=="-p"){
                    input[2]=cmd[i+1];
                }
            }
            cout<<account.Login(input[1],input[2])<<"\n";
        }else if(cmd[1]=="logout"){
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-u"){
                    input[1]=cmd[i+1];
                }
            }
            cout<<account.Logout(input[1])<<"\n";
        }else if(cmd[1]=="query_profile"){
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-c"){
                    input[1]=cmd[i+1];
                }else if(cmd[i]=="-u"){
                    input[2]=cmd[i+1];
                }
            }
            if(account.Query(input[1],input[2])==-1){
                cout<<-1<<"\n";
            }
        }else if(cmd[1]=="modify_profile"){
            for(int i=1;i<7;i++){
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
                    input[6]=cmd[i+1];
                }
            }
            if(account.Modify(input[1],input[2],input[3],input[4],input[5],input[6])==-1){
                 cout<<-1<<"\n";
            }
        }else if(cmd[1]=="add_train"){
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-i"){
                    input[1]=cmd[i+1];
                }else if(cmd[i]=="-n"){
                    input[2]=cmd[i+1];
                }else if(cmd[i]=="-m"){
                    input[3]=cmd[i+1];
                }else if(cmd[i]=="-s"){
                    input[4]=cmd[i+1];
                }else if(cmd[i]=="-p"){
                    input[5]=cmd[i+1];
                }else if(cmd[i]=="-x"){
                    input[6]=cmd[i+1];
                }else if(cmd[i]=="-t"){
                    input[7]=cmd[i+1];
                }else if(cmd[i]=="-o"){
                    input[8]=cmd[i+1];
                }else if(cmd[i]=="-d"){
                    input[9]=cmd[i+1];
                }else if(cmd[i]=="-y"){
                    input[10]=cmd[i+1];
                }
            }
            cout<<ticketsys.Add_train(input[1],stoi(input[2]),stoi(input[3]),input[4],
                input[5],input[6],input[7],input[8],input[9],input[10].c_str()[0])<<"\n";
        }else if(cmd[1]=="delete_train"){
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-i"){
                    input[1]=cmd[i+1];
                }
            }
            cout<<ticketsys.Delete_train(input[1])<<"\n";
        }else if(cmd[1]=="release_train"){
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-i"){
                    input[1]=cmd[i+1];
                }
            }
            cout<<ticketsys.Release_train(input[1])<<"\n";
        }else if(cmd[1]=="query_train"){
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-i"){
                    input[1]=cmd[i+1];
                }else if(cmd[i]=="-d"){
                    input[2]=cmd[i+1];
                }
            }
            if(ticketsys.Query_train(input[1],input[2])==-1){
                cout<<-1<<"\n";
            }
        }else if(cmd[1]=="query_ticket"){
            input[4]="time";
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-s"){
                    input[1]=cmd[i+1];
                }else if(cmd[i]=="-t"){
                    input[2]=cmd[i+1];
                }else if(cmd[i]=="-d"){
                    input[3]=cmd[i+1];
                }else if(cmd[i]=="-p"){
                    input[4]=cmd[i+1];
                }
            }
            ticketsys.Query_ticket(input[1],input[2],input[3],input[4]);
        }else if(cmd[1]=="query_transfer"){
            input[4]="time";
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-s"){
                    input[1]=cmd[i+1];
                }else if(cmd[i]=="-t"){
                    input[2]=cmd[i+1];
                }else if(cmd[i]=="-d"){
                    input[3]=cmd[i+1];
                }else if(cmd[i]=="-p"){
                    input[4]=cmd[i+1];
                }
            }
            ticketsys.Query_transfer(input[1],input[2],input[3],input[4]);
        }else if(cmd[1]=="buy_ticket"){
            input[7]="false";
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-u"){
                    input[1]=cmd[i+1];
                }else if(cmd[i]=="-i"){
                    input[2]=cmd[i+1];
                }else if(cmd[i]=="-d"){
                    input[3]=cmd[i+1];
                }else if(cmd[i]=="-n"){
                    input[4]=cmd[i+1];
                }else if(cmd[i]=="-f"){
                    input[5]=cmd[i+1];
                }else if(cmd[i]=="-t"){
                    input[6]=cmd[i+1];
                }else if(cmd[i]=="-q"){
                    input[7]=cmd[i+1];
                }
            } 
            if(ticketsys.Buy_ticket(cmd[0],input[1],input[2],input[3],stoi(input[4]),input[5],input[6],input[7])==-1){
                cout<<-1<<"\n";
            }
        }else if(cmd[1]=="query_order"){
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-u"){
                    input[1]=cmd[i+1];
                }
            }
            if(ticketsys.Query_order(input[1])==-1){
                cout<<-1<<"\n";
            }
        }else if(cmd[1]=="refund_ticket"){
            input[2]="1";
            for(int i=2;i<cmd_cnt;i+=2){
                if(cmd[i]=="-u"){
                    input[1]=cmd[i+1];
                }else if(cmd[i]=="-n"){
                    input[2]=cmd[i+1];
                }
            }
            cout<<ticketsys.Refund_ticket(input[1],stoi(input[2]))<<"\n";
        }else if(cmd[1]=="clean"){
            std::remove((path+"accfile").c_str());
            std::remove((path+"accindex").c_str());
            std::remove((path+"orderfile").c_str());
            std::remove((path+"orderindex").c_str());
            std::remove((path+"released").c_str());
            std::remove((path+"seatfile").c_str());
            std::remove((path+"stationindex").c_str());
            std::remove((path+"trainfile").c_str());
            std::remove((path+"trainindex").c_str());
            std::remove((path+"queueindex").c_str());
            cout<<0<<"\n";
        }else if(cmd[1]=="exit"){
            cout<<"bye\n";
            break;
        }
    }
    return 0;
}

// int main(){
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
    
//     system("pause");
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