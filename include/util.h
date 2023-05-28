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
int to_relative_day(const Date_t& date){
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

struct Time
{
    Time_t buf;
    Time()=default;
    Time(const Time_t& tmpbuf){buf=tmpbuf;}
};
int operator+(const Time& t1,const int& min){
    return time_to_min(t1.buf)+min;
}
int operator-(const Time& t1,const Time& time){
    return time_to_min(t1.buf)-time_to_min(time.buf);
}
bool operator<(const Time& t1,const Time& rhs){
    return time_to_min(t1.buf)<time_to_min(rhs.buf);
}
std::ostream& operator<<(std::ostream& out,const Time& time){
    out<<time.buf;
    return out;
}

struct Date
{
    Date_t buf;  
    Date& operator++(int){
        buf=relative_to_date(to_relative_day(buf)+1);
        return *this;
    }
    Date()=default;
    Date(const Date_t& tmpbuf){buf=tmpbuf;}
};

Date operator+(const Date& t1,const int& day){
    return Date(relative_to_date(to_relative_day(t1.buf)+day));
}
int operator-(const Date& d1,const Date& date){
        return to_relative_day(d1.buf)-to_relative_day(date.buf);
    }
bool operator<(const Date& d1,const Date& rhs){
        return to_relative_day(d1.buf)<to_relative_day(rhs.buf);
    }
std::ostream& operator<<(std::ostream& out,const Date& date){
    out<<date.buf;
    return out;
}
struct Date_Time
{
    Date date;
    Time time;
    Date_Time()=default;
    Date_Time(const Date& tmpdate,const Time& tmptime){
        date=tmpdate;
        time=tmptime;
    }
};
bool operator<(const Date_Time& dt1,const Date_Time& dt2){
    if(dt1.date<dt2.date){
        return 1;
    }else if(dt1.date.buf==dt2.date.buf){
        return dt1.time<dt2.time;
    }
    return 0;
}
int operator-(const Date_Time& dt1,const Date_Time& dt2){
    return (dt1.date-dt2.date)*24*60+(dt1.time-dt2.time);
}
Date_Time& operator+=(Date_Time& dt,const int& min){
    if(dt.time+min>=1440){
        dt.time=Time(min_to_time(dt.time+min-1440));
        dt.date++;
    }else{
        dt.time=Time(min_to_time(dt.time+min));
    }
    return dt;
}
// bool operator==(const Date_Time& dt1,const Date_Time& dt2){
//     return (dt1.date.buf==dt2.date.buf)&&(dt1.time.buf)
// }

std::ostream& operator<<(std::ostream& out,const Date_Time& dt){
    out<<dt.date<<" "<<dt.time;
    return out;
}
#endif