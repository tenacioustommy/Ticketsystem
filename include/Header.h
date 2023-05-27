#ifndef HEADER_H
#define HEADER_H
#include"String.hpp"
using Username_t=string<20>;
using Pass_t=string<30>; 
using Name_t=string<15>;
using Mail_t=string<30>;
using Pos_t= int;
using Logpair=std::pair<Username_t,int>;
using ID=string<20>;
using Station_t=string<30>;
using Time_t=string<5>;
using Date_t=string<5>;
using Status_t=string<10>;
using std::stoi,std::cin,std::cout,std::endl,std::vector,std::pair;
std::string path="output/";
#endif