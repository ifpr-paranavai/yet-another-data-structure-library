#ifndef __YADSL_HASH_TABLE__
#define __YADSL_HASH_TABLE__

#include <cinttypes>
#include <iostream>
#include <string>
#include <cmath>
#include "list.h"
#include "pair.h"
#include "lib.h"

#define GOLDEN_RATIO (0x61C88647)

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
		using size_type = uint32_t;
		using index_type = uint32_t;
		using hash_type = uint32_t;

		private:
			allocator_type *vector;
			size_type _capacity = 0;
			size_type _size = 0;
			unsigned int nbits = 0;

		private:
			inline unsigned int get_higher_pow2 (unsigned int n)
			{
				unsigned int nbits = 0;
				unsigned int pow2 = 1;
				
				while (pow2 < n)
				{
					nbits++;
					pow2 = 1 << nbits;
				}

				this->nbits = nbits;
				return pow2;
			}
			inline hash_type division_hash (key_type key)
			{
				return static_cast<hash_type>(key % this->_capacity);
			}
			inline hash_type hash_32 (hash_type value, unsigned int bits)
			{
				hash_type hash = value * GOLDEN_RATIO;
				return hash >> (32 - bits);
			}

		public:
			~hash_table_t ()
			{
				delete[] this->vector;
			}
			hash_table_t (size_type capacity)
			{
				// this->_capacity = get_higher_pow2(capacity);
				this->_capacity = capacity;
				this->vector = new allocator_type[this->_capacity];
			}

		// element access
		public:
			hashed_type& at (const key_type& key)
			{
				hash_type hash = this->division_hash(key);
				allocator_type& allocator = this->vector[hash];
				auto list_node = allocator.begin();
				while (list_node != nullptr) {
					if (list_node->value.get_key() == key)
						return list_node->value.get_value();

					list_node = list_node->next;
				}
				throw std::out_of_range("element with given key not found");
			}
			value_type& get (const key_type& key)
			{
				hash_type hash = this->division_hash(key);
				allocator_type& allocator = this->vector[hash];
				auto list_node = allocator.begin();
				while (list_node != nullptr) {
					if (list_node->value.get_key() == key)
						return list_node->value;

					list_node = list_node->next;
				}
				throw std::out_of_range("element with given key not found");
			}
			#if DEBUG_MODE
				value_type& get (const key_type& key, stats_t<uint64_t>& stats)
				{
					uint64_t count = 0;
					hash_type hash = this->hash_32(key, this->nbits);
					allocator_type& allocator = this->vector[hash];
					for (auto it = allocator.begin(); it != nullptr; it++) {
						count++;
						value_type& pair = it->value;
						if (pair.get_key() == key) {
							stats.add(count);
							return pair;
						}
					}
					stats.add(count);
					throw std::out_of_range("element with given key not found");
				}
			#endif

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
				hash_type hash = this->division_hash(val.get_key());
				allocator_type& allocator = this->vector[hash];
				allocator.push_back(val);
				this->_size++;
			}
			void add (const key_type& key, const hashed_type& val)
			{
				this->add(value_type(key, val));
			}
			void erase (const key_type& key) noexcept
			{
				hash_type hash = this->division_hash(key);
				allocator_type& allocator = this->vector[hash];
				auto list_node = allocator.begin();
				while (list_node != nullptr) {
					if (list_node->value.get_key() == key) {
						allocator.erase(list_node);
						return;
					}

					list_node = list_node->next;
				}
			}
			void clear () noexcept
			{
				for (uint32_t i = 0; i < this->vector.size(); i++) {
					allocator_type& allocator = this->vector[i];
					allocator.clear();
				}
			}

		// others
		public:
			void print () noexcept
			{
				for (uint32_t i = 0; i < this->vector.size(); i++) {
					allocator_type& allocator = this->vector[i];
					std::cout << "[" << i << "] = ";
					allocator.print();
				}
			}

			void print (std::function<std::string(hashed_type)> func) noexcept
			{
				for (uint32_t i = 0; i < this->vector.size(); i++) {
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
