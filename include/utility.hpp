#ifndef SJTU_UTILITY_HPP
#define SJTU_UTILITY_HPP
namespace sjtu {

template<class T1, class T2>
class pair {
public:
	T1 first;
	T2 second;
	constexpr pair() : first(), second() {}
	pair(const pair &other) = default;
	pair(pair &&other) = default;
	pair(const T1 &x, const T2 &y) : first(x), second(y) {}
	template<class U1, class U2>
	pair(U1 &&x, U2 &&y) : first(x), second(y) {}
	template<class U1, class U2>
	pair(const pair<U1, U2> &other) : first(other.first), second(other.second) {}
	template<class U1, class U2>
	pair(pair<U1, U2> &&other) : first(other.first), second(other.second) {}
};
template<class T>
void swap(T& a,T& b){
	T tmp=a;
	a=b;
	b=tmp;
}
template<class ITER>
void reverse(ITER begin,ITER end){
	end--;
    while (begin!=end) {
        swap(*begin,*end);
        begin++;
		if(begin==end){
			break;
		}
        end--;
    }
}
  
}
#endif
