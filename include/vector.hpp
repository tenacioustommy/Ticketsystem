#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"
#include"utility.hpp"
namespace sjtu 
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector 
{

public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator 
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the 
	// iterator points to. 
	// STL algorithms and containers may use these type_traits (e.g. the following 
	// typedef) to work properly. In particular, without the following code, 
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 * 
		 */

		T* iter;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator(){}
		iterator(T* pointer):iter(pointer){}

		iterator operator+(const int &n) const 
		{
			//TODO
			iterator tmp;
			tmp.iter=this->iter+n;
			return tmp;
		}
		iterator operator-(const int &n) const 
		{
			//TODO
			iterator tmp;
			tmp.iter=this->iter-n;
			return tmp;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
			//TODO??
			if (sizeof(*(rhs.iter))!=sizeof(*iter))
			{
				throw invalid_iterator();
			}
			return this->iter-rhs.iter;
		}
		iterator& operator+=(const int &n) 
		{
			//TODO
			this->iter+=n;
			return *this;
		}
		iterator& operator-=(const int &n) 
		{
			//TODO
			this->iter-=n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp=*this;
			this->iter+=1;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			this->iter+=1;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp=*this;
			this->iter-=1;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			this->iter-=1;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *(this->iter);
		}
		T* operator->()const{
			return this->iter;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			if (this->iter==rhs.iter)
			return 1;
			else{
				return 0;
			}
		}
		bool operator==(const const_iterator &rhs) const {
			if (this->iter==rhs.iter)
			return 1;
			else{
				return 0;
			}
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			if (this->iter==rhs.iter)
			return 0;
			else{
				return 1;
			}
		}
		bool operator!=(const const_iterator &rhs) const {
			if (this->iter==rhs.iter)
			return 0;
			else{
				return 1;
			}
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator 
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
	private:
		T*iter;
	public:
		const_iterator(){}
		const_iterator(T* pointer):iter(pointer){}

		const_iterator operator+(const int &n) const 
		{
			//TODO
			const_iterator tmp;
			tmp.iter=this->iter+n;
			return tmp;
		}
		const_iterator operator-(const int &n) const 
		{
			//TODO
			const_iterator tmp;
			tmp.iter=this->iter-n;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const 
		{
			//TODO??
			if (type(rhs.iter)!=type(this->iter))
			{
				throw invalid_iterator();
			}
			return this->iter-rhs.iter;
		}
		const_iterator& operator+=(const int &n) 
		{
			//TODO
			this->iter+=n;
			return *this;
		}
		const_iterator& operator-=(const int &n) 
		{
			//TODO
			this->iter-=n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
			const_iterator tmp=*this;
			this->iter+=1;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {
			this->iter+=1;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			const_iterator tmp=*this;
			this->iter-=1;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--(){
			this->iter-=1;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *(this->iter);
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			if (this->iter==rhs.iter)
			return 1;
			else{
				return 0;
			}
		}
		bool operator==(const const_iterator &rhs) const {
			if (this->iter==rhs.iter)
			return 1;
			else{
				return 0;
			}
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			if (this->iter==rhs.iter)
			return 0;
			else{
				return 1;
			}
		}
		bool operator!=(const const_iterator &rhs) const {
			if (this->iter==rhs.iter)
			return 0;
			else{
				return 1;
			}
		}
	};

	
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
private:
	
	T* first;
	T* last; 
	size_t m_size;
	size_t capacity;
	void expandcap()
	{
		if(capacity<512){
			capacity*=2;
		}else{
			capacity*=1.5;
		}
		T*p=first;
		first = static_cast<T*>(operator new[](sizeof(T) * capacity)); 
		last=first+m_size;
		for(int i=0;i<m_size;i++)
		{
			new(first+i)T(p[i]);
			p[i].~T(); 
			// first[i]=p[i];
		}
		operator delete[](p);
		
		p=nullptr;
	}
	

public:
	vector() {
		m_size=0;
		capacity=16;
		
		first = static_cast<T*>(operator new[](sizeof(T) * capacity)); 

		// buf = operator new[](capacity * sizeof(T));
		// first=static_cast<T*>(buf);
		last=first;
	}
	vector(const vector &other) {
		m_size=other.m_size;
		capacity=other.capacity;
		
		first = static_cast<T*>(operator new[](sizeof(T) * capacity)); 

		// buf = operator new[](capacity * sizeof(T));
		// first=static_cast<T*>(buf);
		for (int i=0;i<m_size;i++)
		{
			new(first+i)T(other.first[i]);
		}
		last=first+m_size;
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		for(int i=0;i<m_size;i++)
		{
			first[i].~T();
		}
		operator delete[](first);

	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if(&other!=this)
		{
		
		m_size=other.m_size;
		capacity=other.capacity;
		for(int i=0;i<m_size;i++){
			first[i].~T();
		}
		operator delete[](first);
		first = static_cast<T*>(operator new[](sizeof(T) * capacity)); 
		
		for (int i=0;i<m_size;i++)
		{
			new(first+i)T(other.first[i]);
		}
		last=first+m_size;
		}
		return *this;
		
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, m_size)
	 */
	T & at(const size_t &pos) {
		if(pos<0||pos>=m_size)
		{
			throw index_out_of_bound();
		}
		else{
			return *(first+pos);		
		}
	}
	const T & at(const size_t &pos) const {
		if(pos<0||pos>=m_size)
		{
			throw index_out_of_bound();
		}
		else{
			return *(first+pos);
		}
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, m_size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if(pos<0||pos>=m_size)
		{
			throw index_out_of_bound();
		}
		else{
			return *(first+pos);
		}
	}
	const T & operator[](const size_t &pos) const {
		if(pos<0||pos>=m_size)
		{
			throw index_out_of_bound();
		}
		else{
			return *(first+pos);
		}
	}
	/**
	 * access the first element.
	 * throw container_is_empty if m_size == 0
	 */
	const T & front() const {
		if(m_size==0)
		throw container_is_empty();
		else{
			return *first;
		}
	}
	/**
	 * access the last element.
	 * throw container_is_empty if m_size == 0
	 */
	const T & back() const {
		if (m_size==0)
		throw container_is_empty();
		else{
			return *(last-1);
		}
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		iterator it(first);
		return it;
	}
	const_iterator cbegin() const {
		const_iterator it(first);
		return it;
	}
	/**
	 * returns an iterator to the last.
	 */
	iterator end() {
		iterator it(last);
		return it;
	}
	const_iterator cend() const {
		const_iterator it(last);
		return it;
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return(m_size==0);
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return m_size;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for(int i=0;i<m_size;i++)
		{
			first[i].~T();
		}
		m_size=0;
		last=first;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		size_t loc=pos-iterator(first);
		
		if(m_size==capacity)
		{
			expandcap();
		}
		if(loc==m_size)
		{
			push_back(value);
			return iterator(last-1);
		}
		else{
		m_size++;
		last++;
		new(first+m_size-1)T(first[m_size-2]);
		for(size_t i=m_size-2;i>loc;i--)
		{
			first[i]=first[i-1];	

		}
		first[loc]=value;
		
		return iterator(first+loc);
		}
		
		
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > m_size (in this situation ind can be m_size because after inserting the m_size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if(ind>m_size)
		throw index_out_of_bound();
		else {
			iterator it(first+ind);
			return insert(it,value);
		}
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the last() iterator is returned.
	 */
	iterator erase(iterator pos) {
		for (auto it=pos;it!=end()-1;it++)
		{
			*it=*(it+1);

		}
		m_size--;
		last--;
		first[m_size].~T();
		return pos;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= m_size
	 */
	iterator erase(const size_t &ind) {
		if (ind>=m_size)
		throw index_out_of_bound();
		iterator it(first+ind);
		return erase(it);

	}
	/**
	 * adds an element to the last.
	 */
	void push_back(const T &value) {
		if(m_size==capacity)
		{
			expandcap();
		}
		m_size++;
		last++;
		new(first+m_size-1)T(value);
	}
	/**
	 * remove the last element from the last.
	 * throw container_is_empty if m_size() == 0
	 */
	void pop_back() {
		if (m_size==0)
		throw container_is_empty();
		else{
			
			m_size--;
			last--;
			first[m_size].~T();
		}
	}
};
template<typename T,typename Comp>
void quickSort(vector<T>& arr, int low, int high,Comp comp) {
    if (low < high) {
        T pivot = arr[high];
        int i = low - 1;
        for (int j = low; j <= high - 1; j++) {
            if (comp(arr[j],pivot)) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        int pi = i + 1;
        quickSort(arr, low, pi - 1,comp);
        quickSort(arr, pi + 1, high,comp);
    }
}
template<typename T,typename Comp>
void sort(vector<T>& vec,Comp comp){
	quickSort(vec,0,vec.size()-1,comp);
}

}

#endif