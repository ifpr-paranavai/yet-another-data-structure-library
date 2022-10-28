#ifndef __YADSL_VECTOR__
#define __YADSL_VECTOR__

#include <inttypes.h>
#include <initializer_list>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <math.h>
#include "lib.h"

#define LINDEX (this->_size - 1) // last index

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
			void rshift_arr_content (index_type from, size_type places=1)
			{
				for (index_type i = LINDEX; i > (from+places)-1; i--) 
					this->arr[i] = this->arr[i-places];
			}
			void lshift_arr_content (index_type from, size_type places=1)
			{
				for (size_type i = from; i < (LINDEX-places)+1; i++)
					this->arr[i] = this->arr[i+places];
			}
			void alloc (size_t capacity)
			{
				this->arr = new value_type[capacity];
				this->_capacity = capacity;
			}
			void realloc (size_type new_capacity, bool copy_current_val = false)
			{
				if (!copy_current_val) {
					delete[] this->arr;
					this->alloc(new_capacity);
					this->_size = 0;
					return;
				}

				if (new_capacity < this->_size)
					this->_size = new_capacity;
					
				value_type *temp = this->arr;
				this->alloc(new_capacity);
				for (size_type n = 0; n < this->_size; n++) 
					this->arr[n] = temp[n];
				delete[] temp;
			}
		
		// constructors
		public:
			~vector_t ()
			{
				if (this->arr != nullptr)
					delete[] this->arr;
			}
			vector_t ()
			{
				this->arr = new value_type[0];
			}
			vector_t (size_type capacity)
			{
				this->alloc(capacity);
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
		
		// size
		public:
			inline size_type capacity () const noexcept
			{
				return this->_capacity;
			}
			inline size_type size () const noexcept
			{
				return this->_size;
			}
			uint64_t max_size () const noexcept
			{
				return (1ULL << (ENVBITSIZE-1))/sizeof(value_type);
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
			inline bool empty () const noexcept
			{
				return this->_size == 0;
			}
			inline bool full () const noexcept
			{
				return this->_size == this->_capacity;
			}
			void reserve (size_type new_capacity)
			{
				if (new_capacity <= this->_capacity)
					return;
				this->realloc(new_capacity, true);
			}
			void shrink_to_fit ()
			{
				if (this->full())
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
				if (index > LINDEX)
					throw std::out_of_range("Index out of range");

				return this->arr[index];
			}
			value_type& front ()
			{
				return this->at(0);
			}
			value_type& back ()
			{
				return this->at(LINDEX);
			}
			inline value_type* data () const noexcept
			{
				return this->arr;
			}
			value_type& get (const value_type& val)
			{
				for (uint64_t i = 0; i < this->_size; i++) {
					value_type& element = this->arr[i];
					if (element == val)
						return element;
				}

				throw "Not found";
			}
			int64_t find_index (const value_type& val)
			{
				for (uint64_t i = 0; i < this->_size; i++) {
					value_type element = this->arr[i];
					if (element == val)
						return i;
				}
				return -1;
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
				if (this->full()) 
					this->realloc(this->_capacity+1, true);
				
				this->arr[this->_size] = val; 
				this->_size++;
			}
			void set (index_type index, const value_type& val) const noexcept
			{
				if (index <= LINDEX)
					this->arr[index] = val;
			}
			void pop () noexcept
			{
				if (this->_size > 0)
					this->_size--;
			}
			int64_t insert (index_type index, const value_type& val)
			{
				if (index > LINDEX)
					return -1;
				else if (this->full())
					this->realloc(1, true);

				this->_size++;
				this->rshift_arr_content(index);
				this->arr[index] = val;

				return index;
			}
			int64_t insert (index_type index, value_type&& val)
			{
				if (index > LINDEX)
					return -1;
				else if (this->full())
					this->realloc(1, true);

				this->_size++;
				this->rshift_arr_content(index);
				this->arr[index] = val;

				return index;
			}
			int64_t insert (index_type index, size_type n, const value_type& val)
			{
				if (index > LINDEX)
					return -1;
				else if (this->full())
					this->realloc(this->_capacity+n, true);
				else if ((this->_capacity - this->_size) < n) 
					this->realloc(this->_capacity + (n-(this->_capacity-this->_size)), true);

				this->_size += n;
				this->rshift_arr_content(index, n);
				for (index_type i = index; i < index+n; i++)
					this->arr[i] = val;

				return index;
			}
			int64_t insert (index_type index, std::initializer_list<value_type> il)
			{
				uint64_t n = il.size();

				if (index > LINDEX)
					return -1;
				else if (this->full())
					this->realloc(this->_capacity+n, true);
				else if ((this->_capacity - this->_size) < n) 
					this->realloc(this->_capacity + (n-(this->_capacity-this->_size)), true);
				
				this->_size += n;
				this->rshift_arr_content(index, n);
				for (index_type i = index; i < index + n; i++)
					this->arr[i] = *(il.begin()+(i-index));

				return index;
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
			int64_t erase (const value_type& val) noexcept
			{
				int64_t index = this->find_index(val);

				if (index == -1)
					return -1;

				this->lshift_arr_content(index);
				this->_size--;

				return (index-1) == LINDEX ? index-1 : index;
			}
			int64_t erase (index_type first, index_type last) noexcept
			{
				if ((first > LINDEX || last > LINDEX+1) || first > last)
					return -1;
				else if (first == last && last == LINDEX) {
					this->_size--;
					first--;
				}
				else {
					for (index_type i = first; i < last; i++)
						this->lshift_arr_content(first);

					this->_size -= last - first;
				}
				
				return first;
			}
			void copy (class_type& vector)
			{
				if (this->arr != nullptr)
					delete[] this->arr;
					
				this->_size = 0;
				this->alloc(vector.capacity());
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
				this->alloc(this->_capacity);
				this->_size = 0;
			}
			template <class... Args>
			int64_t emplace (index_type index, Args&&... args)
			{
				if (this->full())
					this->realloc(this->_capacity+1, true);
				
				if (index > LINDEX)
					return -1;	
					
				this->_size++;
				this->rshift_arr_content(index);
				this->arr[index] = value_type(args...);
				return index;
			}
			template <class... Args>
			int64_t emplace_back (Args&&... args) 
			{
				if (this->full())
					this->realloc(this->_capacity + 1, true);

				this->arr[this->_size] = value_type(args...);
				this->_size++;
				return LINDEX;
			}
			
		// comparisons
		public:
			bool operator==(class_type& vector) const noexcept
			{
				return this->is_equal(vector);
			}
			bool is_equal (class_type& vector) const noexcept
			{
				if (this->_size != vector.size())
					return false;
					
				for (size_type n = 0; n < this->_size; n++)
					if (this->at(n) != vector[n])
						return false;
				
				return true;
			}
			bool operator!=(class_type& vector) const noexcept
			{
				return !this->is_equal(vector);
			}
			inline bool operator>(class_type& vector) const noexcept
			{
				return this->_size > vector.size();
			}
			inline bool operator<(class_type& vector) const noexcept
			{
				return this->_size < vector.size();
			}
			inline bool operator>=(class_type& vector) const noexcept
			{
				return this->_size >= vector.size();
			}
			inline bool operator<=(class_type& vector) const noexcept
			{
				return this->_size <= vector.size();
			}
		
		// others
		public:
			int64_t index_of (value_type val) const noexcept
			{
				for (size_type n = 0; n < this->_size; n++)
					if (this->arr[n] == val)
						return n;
				
				return -1;
			}
			void print ()
			{	
				std::cout << "[";
				for (size_type n = 0; n < this->_size; n++)
					std::cout << (n > 0 ? ", " : "") << this->at(n);
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

#undef LINDEX

#endif

