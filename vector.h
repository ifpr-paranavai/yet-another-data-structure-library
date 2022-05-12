#ifndef __YADSL_VECTOR__
#define __YADSL_VECTOR__

#include <functional>
#include <inttypes.h>
#include <iostream>
#include "lib.h"

using index_t = uint64_t;
using capacity_t = uint64_t;

namespace yadsl {
	template <typename T>
	class vector_t 
	{
		private:
			T *arr;
			capacity_t _capacity;
			capacity_t _size = 0;
		
		public:
			vector_t (capacity_t capacity)
			{
				this->arr = new T[capacity];
				this->_capacity = capacity;
			}
			
		public:
			inline T* data ()
			{
				return this->arr;
			}
			inline capacity_t capacity ()
			{
				return this->_capacity;
			}
			inline capacity_t size ()
			{
				return this->_size;
			}
		
		public: 
			void push (T element)
			{
				this->arr[this->_size] = element; 
				this->_size++;
			}
			void push (T element, index_t index)
			{
				this->arr[index] = element;
			}
			void insert (T element, capacity_t index, capacity_t n)
			{
				for (capacity_t i = index; i < n; i++)
					this->push(element, i);
				this->_size += (n - index) - 1;
			}
			void pop ()
			{
				*(&this->arr[this->_size - 1]) = *(new T);
				this->_size--;
			}
			
		public:
			T& at (index_t index)
			{
				return this->arr[index];
			}
			T front ()
			{
				return this->at(0);
			}
			T back ()
			{
				return this->at(this->_size-1);
			}
			int index_of (T element)
			{
				for (capacity_t n = 0; n < this->_size; n++)
					if (this->at(n) == element)
						return n;
				
				return -1;
			}
			
		public:
			bool empty ()
			{
				return this->_size == 0;
			}
			bool is_equal (vector_t vector)
			{
				if (this->_size != vector.size())
					return false;
					
				for (capacity_t n = 0; n < this->_size; n++)
					if (this->at(n) != vector[n])
						return false;
				
				return true;
			}
			
		public:
			void print ()
			{
				for (capacity_t n = 0; n < this->_size; n++)
					std::cout << "[" << n << "]: " << this->at(n) << std::endl; 
			}
			void for_each (std::function<void(T, capacity_t)> func)
			{
				for (capacity_t n = 0; n < this->_size; n++)
					func(this->at(n), n);
			}
			void for_each (std::function<void(T)> func)
			{
				this->for_each([&](T element, capacity_t index) {
					func(element);
				});
			}
			
		public:
			T& operator[](index_t index)
			{
				return this->at(index);
			}
			void operator<<(T element)
			{
				this->push(element);
			}
			bool operator==(vector_t vector)
			{
				return this->is_equal(vector);
			}
			bool operator!=(vector_t vector)
			{
				return !this->is_equal(vector);
			}
			bool operator>(vector_t vector)
			{
				return this._size > vector.size();
			}
			bool operator<(vector_t vector)
			{
				return this._size < vector.size();
			}
			bool operator>=(vector_t vector)
			{
				return this._size >= vector.size();
			}
			bool operator<=(vector_t vector)
			{
				return this._size <= vector.size();
			}
		
	};
}

#endif

