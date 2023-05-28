#ifndef TICKET_H
#define TICKET_H
#include"Account.h"
#include"File.h"
#include"util.h"
#include<algorithm>
#include<set>

class Ticketsys
{   
private:
    /*directly mapping b+tree is fast,
    but if value repeated too much,it will waste space
    */
    BPT<ID,Pos_t> traindex;
    //pass by station to trainfilepos
    BPT<Station_t,Pos_t> stationindex;
    File trainfile;
    //id to seatindex, which can also indicate release or not
    BPT<ID,Pos_t> seatindex;
    File seatfile;
    //id to orderindex
    BPT<Username_t,Pos_t> orderindex;
    File orderfile;
    //store the trainid and pos in orderfile
    pair<ID,Pos_t> queue[100000];
    int queuesize;
    File queuefile;
    struct Train
    {
        ID trainid;
        int stationnum;
        Station_t stations[100];
        int seatnum;
        int price[99];
        Time_t starttime;
        int traveltime[99];
        int stoptime[98];
        //0 start, 1 end,no longer than 72h
        Date_t saledate[2];
        char type;
    };
    int sizeoftrain=sizeof(Train);
    void separate(Train& train,const std::string& str,int num,std::string where){
        int start = 0, end = 0,i=0;
        char separator='|';
        if(where=="station"){
            for(i=0;i<num-1;i++){
                end = str.find(separator, start);
                train.stations[i] = Station_t(str.substr(start, end - start));
                start = end + 1;
            }
            train.stations[i]=Station_t(str.substr(start));
        }else if(where=="price"){
            for(i=0;i<num-1;i++){
                end = str.find(separator, start);
                train.price[i] = stoi(str.substr(start, end - start));
                start = end + 1;
            }
            train.price[i]=stoi(str.substr(start));
        }else if(where=="traveltime"){
            for(i=0;i<num-1;i++){
                end = str.find(separator, start);
                train.traveltime[i] = stoi(str.substr(start, end - start));
                start = end + 1;
            }
            train.traveltime[i]=stoi(str.substr(start));
        }else if(where=="stoptime"){
            for(i=0;i<num-1;i++){
                end = str.find(separator, start);
                train.stoptime[i] = stoi(str.substr(start, end - start));
                start = end + 1;
            }
            train.stoptime[i]=stoi(str.substr(start));
        }else if(where=="date"){
            train.saledate[0]=Date_t(str.substr(0,5));
            train.saledate[1]=Date_t(str.substr(6));
        }else{
            throw std::invalid_argument("invalid");
        }
    }
    void writenewtrain(Train& train){
        trainfile.seekp(0,ios::end);
        Pos_t pos=trainfile.tellp();
        traindex.insert(train.trainid,pos);
        for(int i=0;i<train.stationnum;i++){
            stationindex.insert(train.stations[i],pos);
        }
        trainfile.write(CAST(&train),sizeoftrain,pos);
    }
    int removetrain(const ID& id){
        Pos_t pos;
        if(traindex.removeunique(id,pos)){
            Train train;
            trainfile.read(CAST(&train),sizeoftrain,pos);
            for(int i=0;i<train.stationnum;i++){
                stationindex.remove(train.stations[i],pos);
            }
            return 0;
        }else{
            return -1;
        }
    }
    int readtrain(Train& train,const ID& trainid){
        Pos_t pos;
        if(!traindex.find(trainid,pos)){
            return -1;
        }
        trainfile.read(CAST(&train),sizeoftrain,pos);
        return 0;
    }
    int find_stationindex(const Train& train,const Station_t& station,int& idx){
        for(int i=0;i<train.stationnum;i++){
            if(train.stations[i]==station){
                idx=i;
                return 0;
            }
        }
        //don't exist
        return -1;
    }
    bool isonsale(const Train& train,int day){
        if(day<to_relative_day(train.saledate[0])||day>to_relative_day(train.saledate[1])){
            return 0;
        }else{
            return 1;
        }
    } 
    struct Seats
    {
        //seatnum of ith day startime train and j station to j+1 station
        int count[93][99];
    };
    int releaseseat(const Train& train,Seats& seat){
        seatfile.seekp(0,ios::end);
        if(seatindex.insert(train.trainid,seatfile.tellp())==0){
            seatfile.write(CAST(&seat),sizeof(seat),0,ios::end);
            return 0;
        }else{
            return -1;
        }
    }
     //success return pos,else return -1
    Pos_t readseat(const ID& trainid,Seats& seat){
        //not released
        Pos_t pos;
        if(!seatindex.find(trainid,pos)){
            return -1;
        }else{
            seatfile.read(CAST(&seat),sizeof(seat),pos);
            return pos;
        }
    }
    int find_min_seat(const Seats& seat,int start,int end,int day){
        int min=INT32_MAX;
        for(int i=start;i<end;i++){
            if(seat.count[day][i]<min){
                min=seat.count[day][i];
            }
        }
        return min;
    } 
    int buy_seat(Seats& seat,const int day,const int& num,const int start,const int end){
        for(int i=start;i<end;i++){
            if(seat.count[day][i]<num){
                return -1;
            }
        }
        for(int i=start;i<end;i++){
            seat.count[day][i]-=num;
        }
        return 0;
    }
    void refund_seat(Seats& seat,const int day,const int& num,const int start,const int end){
        for(int i=start;i<end;i++){
            seat.count[day][i]+=num;
        }
    }
    struct Order
    {
        int time;
        Status_t status;
        ID trainid;
        Station_t departure;
        int departindex;
        Date_t departdate;
        Time_t departtime;
        Station_t arrival;
        int arrivalindex;
        Date_t arrivaldate;
        Time_t arrivaltime;
        Date_t startdate;
        int price;
        int num;
    };
    void writeorder(const Username_t& username,Order& order){
        orderfile.seekp(0,ios::end);
        Pos_t pos=orderfile.tellp();
        if(order.status=="[pending]"){
            queue[queuesize++]=pair<ID,Pos_t>(order.trainid,pos);
            // queue.push_back(pair<ID,Pos_t>(order.trainid,pos));
        }
        orderindex.insert(username,pos);
        orderfile.write(CAST(&order),sizeof(order),0,ios::end);
    }
    //read order by username and sort from new to old
    int readorder(const Username_t& username,std::vector<Order>& ordervec,std::vector<Pos_t>& posvec){
        Order order;
        //posvec increase
        if(orderindex.findall(username,posvec)){
            for(auto it=posvec.begin();it!=posvec.end();it++){
                orderfile.read(CAST(&order),sizeof(order),*it);
                ordervec.push_back(order);
            }
            std::reverse(ordervec.begin(),ordervec.end());
            std::reverse(posvec.begin(),posvec.end());
            return 0;
        }else{
            return -1;
        }
    }
    
    struct Ticket
    {
        ID trainid;
        Date_Time departdt;
        int price;
        int seatnum;
        Date_Time arrivaldt;
        Station_t depart;
        Station_t arrival;
    };
    class Compbytime
    {
    public:
        bool operator()(const Ticket& t1,const Ticket& t2)const{
            if((t1.arrivaldt-t1.departdt)<(t2.arrivaldt-t2.departdt)){
                return 1;
            }else if((t1.arrivaldt-t1.departdt)==(t2.arrivaldt-t2.departdt)){
                return t1.trainid<t2.trainid;
            }
            return 0;
        }
    };
    class Compbycost
    {
    public:
        bool operator()(const Ticket& t1,const Ticket& t2)const{
            if(t1.price<t2.price){
                return 1;
            }else if(t1.price==t2.price){
                return t1.trainid<t2.trainid;
            }
            return 0;
        }
    };
private:
    int Delete_train(const ID& tmptrainid){
        Pos_t pos;
        if(seatindex.find(tmptrainid,pos)){
            return -1;
        }else{
            return removetrain(tmptrainid);
        }
    }
   
    int Release_train(const ID& tmptrainid){
        Train train;
        if(readtrain(train,tmptrainid)!=-1){
            //release twice is not allowed
            Pos_t tmp;
            if(seatindex.find(tmptrainid,tmp)){
               return -1;
            }
            Seats seat;
            for(int i=to_relative_day(train.saledate[0]);i<=to_relative_day(train.saledate[1]);i++){
                for(int j=0;j<train.stationnum;j++){
                    seat.count[i][j]=train.seatnum;
                }
            }
            return releaseseat(train,seat);
        }else {
            return -1;
        }
        
    }
    //date trainid
    int Add_train(const ID& tmptrainid,const int& tmpstanum,const int& tmpseatnum,
                const std::string& tmpstation,const std::string& tmpprice,const Time_t& tmpstarttime,
                const std::string& tmptraveltime,const std::string& tmpstoptime,
                const std::string& tmpsaledate,const char& tmptype){
        Pos_t pos;
        //already exist
        if(traindex.find(tmptrainid,pos)){
            return -1;
        }
        Train train;
        train.trainid=tmptrainid;
        train.stationnum=tmpstanum;
        train.seatnum=tmpseatnum;
        
        train.type=tmptype;
        train.starttime=tmpstarttime;
        separate(train,tmpstation,tmpstanum,"station");
        separate(train,tmpprice,tmpstanum-1,"price");
        separate(train,tmptraveltime,tmpstanum-1,"traveltime");
        if(tmpstoptime!="_"){
            separate(train,tmpstoptime,tmpstanum-2,"stoptime");
        }
        separate(train,tmpsaledate,2,"date");
        writenewtrain(train);
        return 0;
    }
    
    int Buy_ticket(int transtime,const Username_t& username,const ID& trainid,const Date_t& date,
                const int& num,const Station_t& departure,const Station_t& arrival,std::string isqueue="false"){
        if(!account.islogged(username)){
            return -1;
        }
        Order order;
        Seats seat;
        Pos_t pos;
        if((pos=readseat(trainid,seat))==-1){
            return -1;
        }
        Train train;
        readtrain(train,trainid);
        int day=to_relative_day(date);
        int starttime=time_to_min(train.starttime);
        int start,end;
        if(find_stationindex(train,departure,start)==-1){
            return -1;
        }
        if(find_stationindex(train,arrival,end)==-1){
            return -1;
        }
        if(start>=end){
            return -1;
        }
        order.departindex=start;
        order.arrivalindex=end;
        for(int i=0;i<start;i++){
            starttime+=train.traveltime[i]+train.stoptime[i];
            if(starttime>=1440){
                starttime-=1440;
                day--;
            }
        }
        //startday not in sale
        if(!isonsale(train,day)){
            return -1;
        }
        order.startdate=relative_to_date(day);
        //price
        int price=0;
        for(int i=start;i<end;i++){
            price+=train.price[i];
        }
        if(buy_seat(seat,day,num,start,end)==-1){
            if(isqueue=="true"){
                order.status=Status_t("[pending]");
                cout<<"queue\n";
            }else{
                return -1;
            }
        }else{
            //write seat
            seatfile.write(CAST(&seat),sizeof(seat),pos);
            cout<<price*num<<"\n";
            order.status=Status_t("[success]");
        }
        order.price=price;
        order.trainid=trainid;
        order.num=num;
        order.time=transtime;
        //depart
        order.departtime=min_to_time(starttime);
        order.departdate=date;
        order.departure=departure;
        //arrival
        day=to_relative_day(date);
        for(int i=start;i<end;i++){
            if(i==end-1){
                starttime+=train.traveltime[i];
            }else{
                starttime+=train.traveltime[i]+train.stoptime[i];
            }
            if(starttime>=1440){
                starttime-=1440;
                day++;
            }
        }
        order.arrivaldate=relative_to_date(day);
        order.arrivaltime=min_to_time(starttime);
        order.arrival=arrival;
        writeorder(username,order);
        return 0;
    }
    int Query_train(const ID& trainid,const Date_t& date){
        Train train;
        if(readtrain(train,trainid)==-1){
            return -1;
        }
        //not in release date
        if(!isonsale(train,to_relative_day(date))){
            return -1;
        }
        Seats seat;
        bool isrelease=true;
        //not released
        if(readseat(trainid,seat)==-1){
            //todo
            isrelease=false;
        }
        int day=to_relative_day(date);
        int *count=seat.count[day];
        int min=time_to_min(train.starttime);
        cout<<trainid<<" "<<train.type<<"\n";
        int price=0;
        for(int i=0;i<train.stationnum;i++){
            if(!isrelease){
                count[i]=train.seatnum;
            }
            if(i==0){
                cout<<train.stations[i]<<" xx-xx xx:xx -> "<<date<<" "<<train.starttime<<" "<<price<<" "<<count[i]<<"\n";
                price+=train.price[i];
                min+=train.traveltime[i];
            }else if(i==train.stationnum-1){
                cout<<train.stations[i]<<" "<<relative_to_date(day)<<" "<<min_to_time(min)<<" -> xx-xx xx:xx "<<price<<" x\n";
            }else{
                cout<<train.stations[i]<<" "<<relative_to_date(day)<<" "<<min_to_time(min)<<" -> ";
                min+=train.stoptime[i-1];
                if(min>=1440){
                    min-=1440;
                    day++;
                }
                cout<<relative_to_date(day)<<" "<<min_to_time(min)<<" "<<price<<" "<<count[i]<<"\n";
                min+=train.traveltime[i];
                price+=train.price[i];
            }
            if(min>=1440){
                min-=1440;
                day++;
            }
        }
        return 0;
    }

    bool achieveticket(Ticket& ticket,const Train& train,int start,int end,const Date& curdate){
        Seats seat;
        Date_Time dt;
        if(start>=end||readseat(train.trainid,seat)==-1){
            return 0;
        }
        int day=to_relative_day(curdate.buf),min=time_to_min(train.starttime);
        for(int i=0;i<start;i++){
            min+=train.traveltime[i]+train.stoptime[i];
            if(min>=1440){
                min-=1440;
                day--;
            }
        }
        if(!isonsale(train,day)){
            return 0;
        }
        ticket.departdt.time=Time(min_to_time(min));
        dt.date=curdate;
        dt.time=Time(min_to_time(min));
        ticket.seatnum=find_min_seat(seat,start,end,day);
        int price=0;
        for(int i=start;i<end;i++){
            price+=train.price[i];
            if(i==end-1){
                dt+=train.traveltime[i]; 
            }else{
                dt+=train.traveltime[i]+train.stoptime[i];
            }
        }
        ticket.depart=train.stations[start];
        ticket.arrival=train.stations[end];
        ticket.departdt.date=curdate;
        ticket.arrivaldt.date=dt.date;
        ticket.price=price;
        ticket.arrivaldt.time=dt.time;
        ticket.trainid=train.trainid;
        return 1;
    }
    void Query_ticket(const Station_t& departstation,const Station_t& arrivalstation,const Date_t& date,const std::string& opt="time"){

        std::vector<Pos_t> vec1,vec2,vec;
        std::set<Ticket,Compbytime> tickettimeset;
        std::set<Ticket,Compbycost> ticketcostset;
        stationindex.findall(departstation,vec1);
        stationindex.findall(arrivalstation,vec2);
        find_sameele(vec1,vec2,vec);
        if(vec.empty()){
            cout<<0<<"\n";
            return;
        }
        int start,end;
        Train train;
        Ticket ticket;
        for(auto it=vec.begin();it!=vec.end();it++){
            trainfile.read(CAST(&train),sizeoftrain,*it);
            find_stationindex(train,departstation,start);
            find_stationindex(train,arrivalstation,end);
            if(!achieveticket(ticket,train,start,end,date)){
                continue;
            }
            if(opt=="time"){
                tickettimeset.insert(ticket);
            }else if(opt=="cost"){
                ticketcostset.insert(ticket);
            }
        }
        if(opt=="time"){
            cout<<tickettimeset.size()<<"\n";
            for(auto it=tickettimeset.begin();it!=tickettimeset.end();it++){
                cout<<it->trainid<<" "<<departstation<<" "<<it->departdt<<" -> "<<arrivalstation<<" "<<it->arrivaldt<<" "<<it->price<<" "<<it->seatnum<<"\n";
            }
        }else if(opt=="cost"){
            cout<<ticketcostset.size()<<"\n";
            for(auto it=ticketcostset.begin();it!=ticketcostset.end();it++){
                cout<<it->trainid<<" "<<departstation<<" "<<it->departdt<<" -> "<<arrivalstation<<" "<<it->arrivaldt<<" "<<it->price<<" "<<it->seatnum<<"\n";
            }
        }
    }
    //t1,t2 original,t3,t4 new 
    bool Comptransfer(const Ticket& t1,const Ticket& t2,const Ticket& t3,const Ticket& t4,const std::string& opt){
        if(opt=="time"){
            if((t2.arrivaldt-t1.departdt)<(t4.arrivaldt-t3.departdt)){
                return 1;
            }else if((t2.arrivaldt-t1.departdt)==(t4.arrivaldt-t3.departdt)){
                if(t1.price+t2.price<t3.price+t4.price){
                    return 1;
                }else if(t1.price+t2.price==t3.price+t4.price){
                    if(t1.trainid<t3.trainid){
                        return 1;
                    }else if(t1.trainid==t3.trainid){
                        return t2.trainid<t4.trainid;
                    }
                }
            }
            return 0;
        }else if(opt=="cost"){
            if(t1.price+t2.price<t3.price+t4.price){
                return 1;
            }else if(t1.price+t2.price==t3.price+t4.price){
                if((t2.arrivaldt-t1.departdt)<(t4.arrivaldt-t3.departdt)){
                    return 1;
                }else if((t2.arrivaldt-t1.departdt)==(t4.arrivaldt-t3.departdt)){
                    if(t1.trainid<t3.trainid){
                        return 1;
                    }else if(t1.trainid==t3.trainid){
                        return t2.trainid<t4.trainid;
                    }
                }
            }
            return 0;
        }
        throw std::invalid_argument("invalid opt");
    }
    void Query_transfer(const Station_t& departstation,const Station_t& arrivalstation,const Date_t& date,const std::string& opt="time"){
        vector<Train> trainvec1,trainvec2;
        vector<Pos_t> vec1,vec2;
        Ticket ticket1,ticket2,tmpticket1,tmpticket2;
        bool ishaveticket=false;
        stationindex.findall(departstation,vec1);
        stationindex.findall(arrivalstation,vec2);
        Train train;
        for(auto it=vec1.begin();it!=vec1.end();it++){
            trainfile.read(CAST(&train),sizeoftrain,*it);
            trainvec1.push_back(train);
        }
        for(auto it=vec2.begin();it!=vec2.end();it++){
            trainfile.read(CAST(&train),sizeoftrain,*it);
            trainvec2.push_back(train);
        }
        int start,end,mid;
        for(auto it1=trainvec1.begin();it1!=trainvec1.end();it1++){
            for(auto it2=trainvec2.begin();it2!=trainvec2.end();it2++){
                if(it1->trainid==it2->trainid){
                    continue;
                }
                find_stationindex(*it1,departstation,start);
                find_stationindex(*it2,arrivalstation,end);
                for(int i=start+1;i<it1->stationnum;i++){
                    for(int j=0;j<end;j++){
                        if(it1->stations[i]==it2->stations[j]){
                            //find possible res
                            if(!achieveticket(tmpticket1,*it1,start,i,date)){
                                continue;
                            }
                            Date cur=Date((*it2).saledate[0]);
                            if(tmpticket1.arrivaldt.date<cur){
                                //i need consider onsale or not in coming days
                                if(!achieveticket(tmpticket2,*it2,j,end,cur)){
                                    if(!achieveticket(tmpticket2,*it2,j,end,cur+1)){
                                        if(!achieveticket(tmpticket2,*it2,j,end,cur+2)){
                                            if(!achieveticket(tmpticket2,*it2,j,end,cur+3)){
                                                continue;
                                            }
                                        } 
                                    }
                                }
                            }else{
                                if(!achieveticket(tmpticket2,*it2,j,end,Date(tmpticket1.arrivaldt.date))){
                                    continue;
                                }
                            }
                            //time may not be proper
                            if(tmpticket2.departdt<tmpticket1.arrivaldt){
                                if(!achieveticket(tmpticket2,*it2,j,end,tmpticket2.departdt.date+1)){
                                    continue;
                                }
                            }
                            //no ticket at all
                            if(!ishaveticket){
                                ticket1=tmpticket1;
                                ticket2=tmpticket2;
                                ishaveticket=true;
                            }else{
                                if(!Comptransfer(ticket1,ticket2,tmpticket1,tmpticket2,opt)){
                                    ticket1=tmpticket1;
                                    ticket2=tmpticket2;
                                }
                            }
                        }
                    }
                }
            }
        }
        if(!ishaveticket){
            cout<<0<<"\n";
            return;
        }else{
            cout<<ticket1.trainid<<" "<<ticket1.depart<<" "<<ticket1.departdt<<" -> "<<ticket1.arrival<<" "<<ticket1.arrivaldt<<" "<<ticket1.price<<" "<<ticket1.seatnum<<"\n";
            cout<<ticket2.trainid<<" "<<ticket2.depart<<" "<<ticket2.departdt<<" -> "<<ticket2.arrival<<" "<<ticket2.arrivaldt<<" "<<ticket2.price<<" "<<ticket2.seatnum<<"\n";
            return ;
        }
    }
    int Query_order(const Username_t& username){
        if(!account.islogged(username)){
            return -1;
        }
        std::vector<Order> ordervec;
        std::vector<Pos_t> posvec;
        if(readorder(username,ordervec,posvec)!=-1){
            cout<<ordervec.size()<<"\n";
            for(auto it=ordervec.begin();it!=ordervec.end();it++){
                cout<<it->status<<" "<<it->trainid<<" "<<it->departure<<" "<<it->departdate<<" "<<it->departtime<<" -> "<<
                it->arrival<<" "<<it->arrivaldate<<" "<<it->arrivaltime<<" "<<it->price<<" "<<it->num<<"\n";
            }
        }else{
            cout<<0<<"\n";
        }
        return 0;
    }   
    int Refund_ticket(const Username_t& username,const int n=1){
        if(!account.islogged(username)){
            return -1;
        }
        std::vector<Order> ordervec;
        std::vector<Pos_t> posvec;
        if(readorder(username,ordervec,posvec)!=-1){
            //nth order don't exist
            if(n>ordervec.size()){
                return -1;
            }
            Order order=ordervec[n-1];
            if(order.status=="[success]"){
                //change order status
                order.status="[refunded]";
                orderfile.write(CAST(&order),sizeof(order),posvec[n-1]);
                //refund seat
                Seats seat;
                Pos_t pos;
                if((pos=readseat(order.trainid,seat))==-1){
                    throw std::runtime_error("fatal error");
                }
                refund_seat(seat,to_relative_day(order.startdate),order.num,order.departindex,order.arrivalindex);
                //find if there is more ticket for queue
                int index=0;
                while(index!=queuesize){
                    //trainid not equal
                    if(queue[index].first!=order.trainid){
                        index++;
                    }else{
                        Order pending;
                        orderfile.read(CAST(&pending),sizeof(pending),queue[index].second);
                        if(buy_seat(seat,to_relative_day(pending.startdate),pending.num,pending.departindex,pending.arrivalindex)==-1){
                            //buy seat fail,move to next one
                            index++;
                        }else{
                            //success then change status,write and delete queue
                            pending.status="[success]";
                            orderfile.write(CAST(&pending),sizeof(pending),queue[index].second);
                            //remove ele
                            queuesize--;
                            for(int i=index;i<queuesize;i++){
                                queue[i]=queue[i+1];
                            }
                        }
                    }
                }
                //write seatfile at last
                seatfile.write(CAST(&seat),sizeof(seat),pos);
                return 0;
            }else if(order.status=="[pending]"){
                //change order status
                order.status="[refunded]";
                orderfile.write(CAST(&order),sizeof(order),posvec[n-1]);
                return 0;
            }else{
                return -1;
            }
        }else{
            return -1;
        }
    }
    
public:
    int Refund_ticket(const std::string& username,const std::string& n){
        if(n.empty()){
            return Refund_ticket(Username_t(username));
        }else{
            return Refund_ticket(Username_t(username),stoi(n));
        }
    }
    int Query_order(const std::string& username){
        return Query_order(Username_t(username));
    }
    int Buy_ticket(std::string& transtime,const std::string& username,const std::string& trainid,const std::string& date,
                const std::string& num,const std::string& departure,const std::string& arrival,std::string& isqueue){
        transtime.pop_back();
        transtime=transtime.substr(1);
        if(isqueue.empty()){
            return Buy_ticket(stoi(transtime),Username_t(username),ID(trainid),Date_t(date),stoi(num),
                Station_t(departure),Station_t(arrival));
        }else{
            return Buy_ticket(stoi(transtime),Username_t(username),ID(trainid),Date_t(date),stoi(num),
                Station_t(departure),Station_t(arrival),isqueue);
        }
    }
    void Query_transfer(const std::string& departstation,const std::string& arrivalstation,const std::string& date,const std::string& opt){
        if(opt.empty()){
            Query_transfer(Station_t(departstation),Station_t(arrivalstation),date);
        }else{
            Query_transfer(Station_t(departstation),Station_t(arrivalstation),date,opt);
        }
    }
    void Query_ticket(const std::string& departstation,const std::string& arrivalstation,const std::string& date,const std::string& opt){
        if(opt.empty()){
            Query_ticket(Station_t(departstation),Station_t(arrivalstation),date);
        }else{
            Query_ticket(Station_t(departstation),Station_t(arrivalstation),date,opt);
        }
    }
    int Query_train(const std::string& trainid,const std::string& date){
        return Query_train(ID(trainid),Date_t(date));
    }
    int Release_train(const std::string& tmptrainid){
        return Release_train(ID(tmptrainid));
    }
    int Delete_train(const std::string& tmptrainid){
        return Delete_train(ID(tmptrainid));
    }
    int Add_train(const std::string& tmptrainid,const std::string& tmpstanum,const std::string& tmpseatnum,
                const std::string& tmpstation,const std::string& tmpprice,const std::string& tmpstarttime,
                const std::string& tmptraveltime,const std::string& tmpstoptime,
                const std::string& tmpsaledate,const std::string& tmptype){
        
        return Add_train(ID(tmptrainid),std::stoi(tmpstanum),std::stoi(tmpseatnum),tmpstation,
            tmpprice,Time_t(tmpstarttime),tmptraveltime,tmpstoptime,tmpsaledate,tmptype.c_str()[0]);
        
    }
    Ticketsys():traindex(path+"trainindex"),seatindex(path+"released"),
                stationindex(path+"stationindex"),orderindex(path+"orderindex"){
        trainfile.init(path+"trainfile");
        seatfile.init(path+"seatfile");
        orderfile.init(path+"orderfile");
        if(!queuefile.init(path+"queuefile")){
            //old file
            queuefile.read(CAST(&queuesize),sizeof(queuesize),0);
            queuefile.read(CAST(queue),sizeof(pair<ID,Pos_t>)*queuesize,4);
        }
    }
    ~Ticketsys(){
        queuefile.write(CAST(&queuesize),sizeof(queuesize),0);
        queuefile.write(CAST(queue),sizeof(pair<ID,Pos_t>)*queuesize,4);
    }
};
Ticketsys ticketsys;

#endif