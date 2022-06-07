#ifndef __YADSL_HASH_TABLE__
#define __YADSL_HASH_TABLE__

#include <cinttypes>
#include <iostream>
#include "vector.h"
#include "list.h"
#include "lib.h"

using namespace yadsl;

namespace yadsl
{
	template<typename T>
	class hash_table_t 
	{
		using value_type = T;
		using size_type = uint64_t;
		using index_type = uint64_t;
		using hash_type = uint64_t;

		private:
			vector_t<list_t<value_type>> vector;
			size_type _size = 0;

		private:
			hash_type hash (value_type value) const noexcept
			{
				return value % this->vector.capacity();
			}

		public:
			hash_table_t (size_type capacity)
			{
				std::cout << "prime:" << next_prime(next_prime(capacity * 2)) << std::endl;
				this->vector.assign(next_prime(next_prime(capacity * 2)), list_t<value_type>());
			}

		// element access
		public:
			value_type* get (const value_type& value) noexcept
			{
				list_t<value_type> *list = &(this->vector[this->hash(value)]);
				return &(list->get(value)->value);
			}

		// capacity
		public:
			inline size_type size () const noexcept
			{
				return this->_size;
			}
			inline bool empty () const noexcept
			{
				return this->_size == 0;
			}
		
		// modifiers
		public:
			void operator<< (const value_type& value)
			{
				this->add(value);
			} 
			void add (const value_type& value)
			{
				list_t<value_type> *list = &(this->vector[this->hash(value)]);
				list->push_back(value);
				this->_size++;
			}
			void erase (const value_type& value) noexcept
			{
				list_t<value_type> *list = &(this->vector[this->hash(value)]);
				list->erase(list->get(value));
				this->_size--;
			}
			void clear () noexcept
			{
				for (uint64_t i = 0; i < this->vector.size(); i++) {
					list_t<value_type> *list = &(this->vector[i]);
					list->clear();
				}
			}

		// others
		public:
			void print () noexcept
			{
				for (uint64_t i = 0; i < this->vector.size(); i++) {
					list_t<value_type> *list = &(this->vector[i]);
					std::cout << "[" << i << "] = ";
					list->print();
				}
			}
	};
}

#endif
