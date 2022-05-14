#ifndef __YADSL_VECTOR__
#define __YADSL_VECTOR__

#include <initializer_list>
#include <functional>
#include <inttypes.h>
#include <iostream>
#include "lib.h"

namespace yadsl 
{
	template <typename T>
	class vector_t 
	{
		using class_type = vector_t<T>;
		using value_type = T;
		using index_type = uint64_t;
		using size_type = uint64_t;
	
		private:
			value_type *arr = nullptr;
			size_type _capacity = 0;
			size_type _size = 0;
			
		private:
			void fill (const value_type& val)
			{
				for (size_type n = 0; n < this->_capacity; n++)
					this->push(val);
			}
			void fill (std::initializer_list<value_type>& il)
			{
				for (auto it = il.begin(); it != il.end(); it++)
					this->push(*it);
			}
			void realloc (size_type new_capacity, bool copy_current_val = false)
			{
				if (!copy_current_val) {
					delete[] this->arr;
					this->arr = new value_type[new_capacity];
					this->_size = 0;
				}
				else {
					if (new_capacity < this->_size)
						this->_size = new_capacity;
						
					value_type *temp = this->arr;
					this->arr = new value_type[new_capacity];
					for (size_type n = 0; n < this->_size; n++) 
						this->arr[n] = temp[n];
					delete[] temp;
				}
				
				this->_capacity = new_capacity;
			}
		
		// constructors
		public:
			~vector_t ()
			{
				if (this->arr != nullptr)
					delete[] this->arr;
			}
			vector_t (size_type capacity)
			{
				this->arr = new value_type[capacity];
				this->_capacity = capacity;
			}
			vector_t (size_type capacity, const value_type& val) : vector_t(capacity)
			{
				this->fill(val);
			}
			vector_t (std::initializer_list<value_type> il) : vector_t(il.size())
			{
				this->fill(il);
			}
			vector_t (size_type capacity, value_type *arr) : vector_t(capacity)
			{
				for (size_type n = 0; n < this->_capacity; n++)
					this->push(arr[n]);
			}
			vector_t (class_type& vector) : vector_t(vector.capacity(), vector.data()) {}
		
		// capacity
		public:
			inline size_type capacity ()
			{
				return this->_capacity;
			}
			inline size_type size ()
			{
				return this->_size;
			}
			void resize (size_type new_size)
			{
				if (new_size == this->_size)
					return;
				else if (new_size > this->_capacity) 
					this->realloc(new_size, true);
				
				this->_size = new_size;
			}
			void resize (size_type new_size, const value_type& val)
			{
				if (new_size == this->_size)
					return;
				else if (new_size > this->_size && new_size <= this->_capacity) 
					for (size_type n = this->_size; n < new_size; n++)
						this->arr[n] = val;
				else if (new_size > this->_capacity) {
					this->realloc(new_size, true);
					for (size_type n = this->_size+1; n < new_size; n++)
						this->arr[n] = val;
				}
				
				this->_size = new_size;
			}
			inline bool empty ()
			{
				return this->_size == 0;
			}
			void reserve (size_type new_capacity)
			{
				if (new_capacity <= this->_capacity)
					return;
				this->realloc(new_capacity, true);
			}
			void shrink_to_fit ()
			{
				if (this->_size == this->_capacity)
					return;
				this->realloc(this->_size, true);
			}
		
		// value access
		public:
			value_type& operator[](index_type index)
			{
				return this->at(index);
			}
			value_type& at (index_type index)
			{
				return this->arr[index];
			}
			value_type front ()
			{
				return this->at(0);
			}
			value_type back ()
			{
				return this->at(this->_size-1);
			}
			inline value_type* data ()
			{
				return this->arr;
			}
			
		// modifiers
		public: 
			void operator<<(const value_type& val)
			{
				this->push(val);
			}
			void operator=(class_type& vector)
			{
				this->swap(vector);
			}
			void push (const value_type& val)
			{
				if (this->_size == this->_capacity) 
					this->realloc(this->_capacity+1, true);
				
				this->arr[this->_size] = val; 
				this->_size++;
			}
			void set (index_type index, const value_type& val)
			{
				this->arr[index] = val;
			}
			void pop ()
			{
				this->_size--;
			}
			void insert (const value_type& val, size_type index, size_type n)
			{
				for (size_type i = index; i < n; i++)
					this->set(i, val);
				this->_size += (n - index) - 1;
			}
			void assign (size_type capacity, const value_type& val)
			{
				this->realloc(capacity);
				this->fill(val);
			}
			void assign (std::initializer_list<value_type> il)
			{
				this->realloc(il.size());
				this->fill(il);
			}
			int64_t erase (index_type index)
			{
				#define L_INDEX this->_size - 1 
				if (index > L_INDEX)
					return -1;
				else if (index == L_INDEX) 
					index--;
				else 
					for (size_type n = index; n < L_INDEX; n++)
						this->arr[n] = this->arr[n+1];
				#undef L_INDEX
				
				this->_size--;
				return index;
			}
			int64_t erase (index_type first, index_type last)
			{
				#define L_INDEX this->_size - 1 
				if ((first > L_INDEX || last > L_INDEX) || first > last)
					return -1;
				else if (first == last && last == L_INDEX) {
					this->_size--;
					first--;
				}
				else {
					for (index_type i = first; i < last; i++)
						for (size_type n = first; n < L_INDEX; n++) {
							value_type temp = arr[n];
							this->arr[n] = this->arr[n+1];
							this->arr[n+1] = temp;
						}

					this->_size -= last - first;
				}
				#undef L_INDEX
				
				return first;
			}
			void copy (class_type& vector)
			{
				if (this->arr != nullptr)
					delete[] this->arr;
					
				this->_size = 0;
				this->_capacity = vector.capacity();
				this->arr = new value_type[this->_capacity];
				for (size_type n = 0; n < this->_capacity; n++)
					this->push(vector[n]);
			}
			void swap (class_type& vector)
			{
				class_type temp(*this);
				this->copy(vector);
				vector.copy(temp);
			}
			void clear ()
			{
				if (this->arr == nullptr)
					return;
					
				delete[] this->arr;
				this->arr = new value_type[this->_capacity];
				this->_size = 0;
			}
			int64_t emplace (index_type index, const value_type& val)
			{
				if (index > this->_size)
					return -1;
					
				if (this->_size == this->_capacity)
					this->realloc(this->_capacity+1, true);
					
				if (index < this->_size)
					for (size_type n = this->_size; n > index; n--) {
						value_type temp = arr[n];
						this->arr[n] = this->arr[n-1];
						this->arr[n-1] = temp;
					}
					
				this->_size++;
				this->arr[index] = val;
				return index;
			}
			
		// comparisons
		public:
			bool operator==(class_type& vector)
			{
				return this->is_equal(vector);
			}
			bool is_equal (class_type& vector)
			{
				if (this->_size != vector.size())
					return false;
					
				for (size_type n = 0; n < this->_size; n++)
					if (this->at(n) != vector[n])
						return false;
				
				return true;
			}
			bool operator!=(class_type& vector)
			{
				return !this->is_equal(vector);
			}
			inline bool operator>(class_type& vector)
			{
				return this->_size > vector.size();
			}
			inline bool operator<(class_type& vector)
			{
				return this->_size < vector.size();
			}
			inline bool operator>=(class_type& vector)
			{
				return this->_size >= vector.size();
			}
			inline bool operator<=(class_type& vector)
			{
				return this->_size <= vector.size();
			}
		
		// others
		public:
			int64_t index_of (value_type val)
			{
				for (size_type n = 0; n < this->_size; n++)
					if (this->at(n) == val)
						return n;
				
				return -1;
			}
			void print ()
			{	
				std::cout << "[";
				for (size_type n = 0; n < this->_size; n++)
					std::cout << this->at(n) << ", ";
				std::cout << "]" << std::endl;
				std::cout << "s(" << this->_size << ") ";
				std::cout << "c(" << this->_capacity << ")" << std::endl;
			}
			void println ()
			{
				for (size_type n = 0; n < this->_size; n++)
					std::cout << "[" << n << "]: " << this->at(n) << std::endl; 
				std::cout << "s(" << this->_size << ") ";
				std::cout << "c(" << this->_capacity << ")" << std::endl;
			}
			void for_each (std::function<void(value_type, size_type)> func)
			{
				for (size_type n = 0; n < this->_size; n++)
					func(this->at(n), n);
			}
			void for_each (std::function<void(value_type)> func)
			{
				for (size_type n = 0; n < this->_size; n++)
					func(this->at(n));
			}
	};
	
	template <typename T>
	void swap (vector_t<T>& x, vector_t<T>& y)
	{		
		vector_t<T> temp(x);
		x.copy(y);
		y.copy(temp);
	}
}

#endif

