#ifndef UTIL_H
#define UTIL_H
#include"Header.h"
int month[12]{0,31,59,90,120,151,181,212,243,273,304,334};
Time_t min_to_time(int min){
    int hour=min/60;
    std::string h,m;
    if(hour<10){
        h="0"+std::to_string(hour);
    }else{
        h=std::to_string(hour);
    }
    min-=hour*60;
    if(min<10){
        m="0"+std::to_string(min);
    }else{
        m=std::to_string(min);
    }
    return Time_t(h+":"+m);
}
int time_to_min(const Time_t&time){
    std::string str(time.buf);
    return std::stoi(str.substr(0,2))*60+std::stoi(str.substr(3,2));
}
Date_t relative_to_date(int day){
    day+=month[5];
    int m=0;
    while(month[m]<day){
        m++;
    }
    std::string monstr,daystr;
    if(m<10){
        monstr="0"+std::to_string(m);
    }else{
        monstr=std::to_string(m);
    }
    day-=month[m-1];
    if(day<10){
        daystr="0"+std::to_string(day);
    }else{
        daystr=std::to_string(day);
    }
    std::string str=monstr+"-"+daystr;
    return Date_t(str);
}
int to_relative_day(Date_t date){
    std::string str(date.buf);
    return month[stoi(str.substr(0,2))-1]+std::stoi(str.substr(3,2))-month[5];
}
template<class T>
void find_sameele(const std::vector<T>& vec1,const std::vector<T>& vec2,std::vector<T>& vec){
    auto it1=vec1.begin(),it2=vec2.begin();
    while(it1!=vec1.end()&&it2!=vec2.end()){
        if(*it1<*it2){
            it1++;
        }else if(*it1>*it2){
            it2++;
        }else{
            vec.push_back(*it1);
            it1++;
        }
    }
}

#endif