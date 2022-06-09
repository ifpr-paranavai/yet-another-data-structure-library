#ifndef __YADSL_LIST__
#define __YADSL_LIST__

#include <functional>
#include <exception>
#include <cinttypes>
#include <iostream>
#include <string>
#include "lib.h"

#define LINDEX (this->_size-1) // last index

namespace yadsl
{
	template <typename T>
	class list_t 
	{
		using value_type = T;
		using size_t = uint64_t;
		using index_type = uint64_t;

		public:
			struct node_t
			{
				value_type value;
				node_t *prev = nullptr;
				node_t *next = nullptr;

				node_t (const value_type& val)
				{
					this->value = val;
				}

				node_t* operator++ (int)
				{
					return this->next;
				}
				node_t* operator-- (int)
				{
					return this->prev;
				}
				bool operator== (node_t* node)
				{
					return this == node;
				}
				bool operator!= (node_t* node)
				{
					return this != node;
				}
			};

		using iterator = node_t;

		private:
			node_t *head = nullptr;
			node_t *tail = nullptr;
			size_t _size = 0;

		private:
			node_t* find_node (index_type index)
			{
				if (index > LINDEX)
					throw std::out_of_range("Index out of range");
					
				node_t *node;
				if (index < (LINDEX / 2)) {
					node = this->head;
					for (index_type i = 0; i < index; i++)
						node = node->next;
				}
				else {
					node = this->tail;
					for (index_type i = LINDEX; i > index; i--)
						node = node->prev;
				}
				return node;
			}

		public:
			~list_t ()
			{
				this->clear();
			}

		public:
			inline iterator* begin () noexcept
			{
				return this->head;
			}
			inline iterator* end () noexcept
			{
				return this->tail;
			}
		// element access
		public:
			value_type& operator[] (index_type index)
			{
				return this->at(index);
			}
			inline value_type& front ()
			{
				return this->head->value;
			}
			inline value_type& back ()
			{
				return this->tail->value;
			}
			value_type& at (index_type index)
			{
				return this->find_node(index)->value;
			}
			node_t* get (const value_type& val)
			{
				node_t *node = this->head;
				while (node != nullptr && node->value != val) {
					node = node->next;
				}
				return node;
			}

		// capacity
		public:
			inline size_t size () const noexcept
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
				this->push_back(val);
			}
			void push_front (const value_type& val)
			{
				node_t *new_node = new node_t(val);
				this->_size++;
				if (this->head == nullptr) {
					this->head = new_node;
					this->tail = new_node;
					return;
				}

				this->head->prev = new_node;
				new_node->next = this->head;
				this->head = new_node;
			}

			void push_back (const value_type& val)
			{
				node_t *new_node = new node_t(val);
				this->_size++;
				if (this->head == nullptr) {
					this->head = new_node;
					this->tail = new_node;
					return;
				}

				this->tail->next = new_node;
				new_node->prev = this->tail;
				this->tail = new_node;
			}
			void pop_front () noexcept
			{
				if (this->head == nullptr)
					return;
				
				node_t *head = this->head;
				if (head->next != nullptr) {
					this->head = head->next;
					this->head->prev = nullptr;
				}
				else {
					this->head = nullptr;
					this->tail = nullptr;
				}
				
				this->_size--;
				delete head;
			}
			void pop_back () noexcept
			{
				if (this->tail == nullptr)
					return;
				
				node_t *tail = this->tail;
				if (tail->prev != nullptr) {
					this->tail = tail->prev;
					this->tail->next = nullptr;
				} 
				else {
					this->head = nullptr;
					this->tail = nullptr;
				}

				this->_size--;
				delete tail;
			}
			node_t* erase (node_t *node)
			{
				node_t *r = nullptr;
				if (node == this->head) {
					if (node == this->tail) {
						this->head = nullptr;
						this->tail = nullptr;
						this->_size = 0;
					}
					else {
						this->head = node->next;
						this->head->prev = nullptr;
						r = this->head;
					}
				}
				else if (node == this->tail) {
					this->tail = node->prev;
					this->tail->next = nullptr;
					r = this->tail;
				}
				else {
					node->next->prev = node->prev;
					node->prev->next = node->next;
					r = node->next;
				}

				this->_size--;
				delete node;
				return r;
			}
			index_type erase (index_type index)
			{
				node_t *node = this->find_node(index);
				this->erase(node);
				return index;
			}
			void remove (const value_type& val) 
			{
				this->for_each([&] (node_t *node, index_type index) {
					if (node->value == val)
						this->erase(node);
				});
			}
			void clear () noexcept
			{
				this->for_each([] (node_t *node) {
					delete node;
				});
				this->head = nullptr;
				this->tail = nullptr;
				this->_size = 0;
			}

		// others
		public:
			void for_each (std::function<void(node_t*)> func) const noexcept
			{
				node_t *node = this->head;
				while (node != nullptr) {
					node_t *next = node->next;
					func(node);
					node = next;
				}
			}
			void for_each (std::function<void(node_t*, index_type)> func) const noexcept
			{
				node_t *node = this->head;
				index_type index = 0;
				while (node != nullptr) {
					node_t *next = node->next;
					func(node, index);
					node = next;
					index++;
				}
			}

			void linked_print () const noexcept
			{
				node_t *node = this->head;
				std::cout << "[<-(head)";
				while (node != nullptr) {
					std::cout << node->value;
					node = node->next;
					if (node != nullptr)
						std::cout << " <-> ";
				}
				std::cout << "(tail)->]" << std::endl;
			}

			void print () const noexcept
			{
				node_t *node = this->head;
				std::cout << '[';
				while (node != nullptr) {
					std::cout << node->value;
					node = node->next;
					if (node != nullptr)
						std::cout << ", ";
				}
				std::cout << ']';
				std::cout << std::endl;
			}

			void print (std::function<std::string(value_type)> func) const noexcept
			{
				node_t *node = this->head;
				std::cout << '[';
				while (node != nullptr) {
					std::cout << func(node->value);
					node = node->next;
					if (node != nullptr)
						std::cout << ", ";
				}
				std::cout << ']';
				std::cout << std::endl;
			}

			void reverse_linked_print () const noexcept
			{
				node_t *node = this->tail;
				std::cout << "[<-(tail)";
				while (node != nullptr) {
					std::cout << node->value;
					node = node->prev;
					if (node != nullptr)
						std::cout << " <-> ";
				}
				std::cout << "(head)->]" << std::endl;
			}

			void reverse_print () const noexcept
			{
				node_t *node = this->tail;
				std::cout << '[';
				while (node != nullptr) {
					std::cout << node->value;
					node = node->prev;
					if (node != nullptr)
						std::cout << ", ";
				}
				std::cout << ']';
				std::cout << std::endl;
			}
	};
}

#undef LINDEX

#endif