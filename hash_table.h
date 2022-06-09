#ifndef __YADSL_HASH_TABLE__
#define __YADSL_HASH_TABLE__

#include <cinttypes>
#include <iostream>
#include <string>
#include "vector.h"
#include "list.h"
#include "pair.h"
#include "lib.h"

using namespace yadsl;

namespace yadsl
{
	template< 
		typename Key, 
		typename T, 
		typename Alloc = list_t<pair_t<Key, T>>
	> class hash_table_t 
	{
		using key_type = Key;
		using hashed_type = T;
		using value_type = pair_t<key_type, hashed_type>;
		using allocator_type = Alloc;
		using size_type = uint64_t;
		using index_type = uint64_t;
		using hash_type = uint64_t;

		private:
			vector_t<allocator_type> vector;
			size_type _size = 0;

		private:
			const hash_type hash (key_type key)
			{
				return static_cast<hash_type>(key % this->vector.capacity());
			}

		public:
			hash_table_t (size_type capacity)
			{
				this->vector.assign(next_prime(capacity * 2), allocator_type());
			}

		// element access
		public:
			hashed_type& at (const key_type& key)
			{
				allocator_type& allocator = this->vector[this->hash(key)];
				for (auto it = allocator.begin(); it != nullptr ; it++) {
					value_type pair = it->value;
					if (pair.get_key() == key)
						return pair.value();
				}
				throw std::out_of_range("element with given key not found");
			}
			value_type get (const key_type& key)
			{
				allocator_type& allocator = this->vector[this->hash(key)];
				for (auto it = allocator.begin(); it != nullptr ; it++) {
					value_type pair = it->value;
					if (pair.get_key() == key)
						return pair;
				}
				throw std::out_of_range("element with given key not found");
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
			void operator<< (const value_type& val)
			{
				this->add(val);
			}
			void add (const value_type& val)
			{
				allocator_type& allocator = this->vector[this->hash(val.get_key())];
				allocator << val;
				this->_size++;
			}
			void add (const key_type& key, const hashed_type& val)
			{
				this->add(value_type(key, val));
			}
			void erase (const key_type& key) noexcept
			{
				allocator_type& allocator = this->vector[this->hash(key)];
				for (auto it = allocator.begin(); it != nullptr ; it++) {
					value_type pair = it->value;
					if (pair.get_key() == key) {
						allocator.erase(it);
						this->_size--;
						return;
					}
				}
			}
			void clear () noexcept
			{
				for (uint64_t i = 0; i < this->vector.size(); i++) {
					allocator_type& allocator = this->vector[i];
					allocator.clear();
				}
			}

		// others
		public:
			void print () noexcept
			{
				for (uint64_t i = 0; i < this->vector.size(); i++) {
					allocator_type& allocator = this->vector[i];
					std::cout << "[" << i << "] = ";
					allocator.print();
				}
			}

			void print (std::function<std::string(hashed_type)> func) noexcept
			{
				for (uint64_t i = 0; i < this->vector.size(); i++) {
					allocator_type& allocator = this->vector[i];
					std::cout << "[" << i << "] = ";
					allocator.print([&] (value_type pair) {
						return func(pair.get_value());
					});
				}
			}
	};
}

#endif
