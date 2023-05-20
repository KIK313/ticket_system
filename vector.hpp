#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP


#include <climits>
#include <cstddef>
#include <cstdint>

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
	int siz,hp_siz;
	T** array;
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

	private:

		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:		
		int pos;
		vector<T> *father;
		iterator (int pp = 0,vector<T> *ff = nullptr) {
			pos = pp;
			father = ff;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const 
		{
			iterator it; it.pos = pos + n; it.father = father;
			return it;
			//TODO
		}
		iterator operator-(const int &n) const 
		{
			iterator it; it.pos = pos-n; it.father = father;
			return it;
			//TODO
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
			return pos - rhs.pos; 
			//TODO
		}
		iterator& operator+=(const int &n) 
		{
			pos += n;
			return (*this);
			//TODO
		}
		iterator& operator-=(const int &n) 
		{
			pos -= n;
			return (*this);
			//TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp = (*this);
			pos++;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			pos++;
			return (*this);
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp = (*this);
			pos--;
			return tmp;			
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			pos--;
			return (*this);
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *((*father).array[pos]);
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			if (father != rhs.father) return 0;
			return pos == rhs.pos;
		}
		bool operator==(const const_iterator &rhs) const {
			if (father != rhs.father) return 0;
			return pos == rhs.pos;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			if (father != rhs.father) return 1;
			return pos != rhs.pos;
		}
		bool operator!=(const const_iterator &rhs) const {
			if (father != rhs.father) return 1;
			return pos != rhs.pos;
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
	
	private:
		/*TODO*/

	public:		
		int pos;
		const vector<T> *father;
		const_iterator (int pp=0, vector<T> *ff=nullptr) { 
			//is the order of pp and ff always the same
			pos = pp;
			father = ff; 
		}
		const_iterator operator + (const int &n) const {
			const_iterator it;
			it.pos = pos + n;
			it.father = father;
			return it;
		}
		const_iterator operator - (const int &n) const {
			const_iterator it;
			it.pos = pos - n;
			it.father = father;
			return it;
		}
		int operator - (const const_iterator &rhs) const {
			return pos - rhs.pos;
		}
		const_iterator& operator += (const int &n) {
			pos += n;
			return (*this);
		}
		const_iterator& operator -= (const int &n) {
			pos -= n;
			return (*this);
		}
		const_iterator operator ++ (int) {
			const_iterator it = (*this);
			pos++;
			return it;
		}
		const_iterator& operator ++() {
			pos++;
			return (*this);
		}
		const_iterator operator -- (int) {
			const_iterator it = (*this);
			pos--;
			return it;
		}
		const_iterator& operator -- () {
			pos--;
			return (*this);
		}
		T operator * () const{
			return *((*father).array[pos]);
		}
		bool operator==(const iterator &rhs) const {
			if (father != rhs.father) return 0;
			return pos == rhs.pos;
		}
		bool operator==(const const_iterator &rhs) const {
			if (father != rhs.father) return 0;
			return pos == rhs.pos;
		}
		bool operator!=(const iterator &rhs) const {
			if (father != rhs.father) return 1;
			return pos != rhs.pos;
		}
		bool operator!=(const const_iterator &rhs) const {
			if (father != rhs.father) return 1;
			return pos != rhs.pos;
		}
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() {
		hp_siz = 2; siz = 0;
		array = new T*[2];
	}
	vector(const vector &other) {
		hp_siz = other.hp_siz; siz= other.siz;
		array = new T*[hp_siz];
		for (int i = 0; i < siz; ++i) array[i]=new T(*(other.array[i]));
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		for (int i = 0; i < siz; ++i) delete array[i];
		delete[] array;
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (this == (&other)) return (*this);
		for (int i = 0; i < siz; ++i) delete array[i];
		delete[] array;
		hp_siz = other.hp_siz; siz= other.siz;
		array = new T*[hp_siz];
		for (int i = 0; i < siz; ++i) array[i]=new T(*(other.array[i]));
		return (*this);				 
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		return (*array[pos]);
	}
	const T & at(const size_t &pos) const {
		return (*array[pos]);
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		return (*array[pos]);
	}
	const T & operator[](const size_t &pos) const {
		return (*array[pos]);
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		return (*array[0]);
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		return (*array[siz-1]); 
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		iterator it; 
		it.pos = 0;
		it.father = this;
		return it;
	}
	const_iterator cbegin() const {
		const_iterator it;
		it.pos = 0;
		it.father = this;
		return it;
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		iterator it;
		it.pos = siz;
		it.father = this;
		return it;
	}
	const_iterator cend() const {
		const_iterator it;
		it.pos = siz;
		it.father = this;
		return it;
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {return siz==0;}
	/**
	 * returns the number of elements
	 */
	size_t size() const {return siz;}
	/**
	 * clears the contents
	 */
	void clear() {
		for (int i = 0; i < siz; ++i) delete array[i];
		delete[] array;
		hp_siz = 2; siz = 0;
		array = new T*[2];
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		int ind = pos.pos;
		int l2; l2 = hp_siz;
		if(siz == l2) l2 <<= 1;
		T** o = new T*[l2];
		for (int i = 0; i < ind; ++i) o[i] = array[i];
		o[ind] = new T(value);
		for (int i = ind; i < siz; ++i) o[i+1] = array[i];
		delete[] array;
		array = o; siz++; hp_siz = l2;
		iterator it; it.pos = ind; it.father = this;
		return it;
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		int l2; l2 = hp_siz;
		if(siz == l2) l2 <<= 1;
		T** o = new T*[l2];
		for (int i = 0; i < ind; ++i) o[i] = array[i];
		o[ind] = new T(value);
		for (int i = ind; i < siz; ++i) o[i+1] = array[i];
		delete[] array;
		array = o; siz++; hp_siz = l2;
		iterator it; it.pos = ind; it.father = this;
		return it;
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		int ind = pos.pos;
		int l2 = hp_siz;
		if (siz - 1 < (hp_siz>>2)) l2 >>= 1;
		T** o = new T*[l2];
		for (int i = 0; i < ind; ++i) o[i] = array[i];
		for (int i = ind + 1; i < siz; ++i) o[i-1] = array[i];
		delete array[ind];
		delete[] array;
		array = o; siz--; hp_siz = l2;
		iterator it; it.pos = ind; it.father = this;
		return it;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		int l2 = hp_siz;
		if (siz - 1 < (hp_siz>>2)) l2 >>= 1;
		T** o = new T*[l2];
		for (int i = 0; i < ind; ++i) o[i] = array[i];
		for (int i = ind + 1; i < siz; ++i) o[i-1] = array[i];
		delete array[ind];
		delete[] array;
		array = o; siz--; hp_siz = l2;
		iterator it; it.pos = ind; it.father = this;
		return it;
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if (siz != hp_siz) {
			array[siz] = new T(value);
			siz++;
		} else {
			T** o = new T*[hp_siz*2];
			for (int i = 0; i < siz; ++i) o[i] = array[i];
			o[siz] = new T(value);
			delete[] array;
			siz++; hp_siz *= 2;
			array = o;
		}
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if (siz - 1 < (hp_siz >> 2) ) {
			siz--; hp_siz >>= 1;
			delete array[siz];
			T** o = new T*[hp_siz];
			for (int i = 0; i < siz; ++i) o[i] = array[i];
			delete[] array;
			array = o; 
		} else {
			siz--;
			delete array[siz];
			array[siz] = nullptr;
		}
	}
};


}

#endif
