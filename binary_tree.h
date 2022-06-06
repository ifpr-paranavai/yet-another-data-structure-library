#ifndef __YADSL_BTREE__
#define __YADSL_BTREE__

#include <cinttypes>
#include <iostream>

namespace yadsl
{
	template <typename T>
	class binary_tree_t
	{
		using value_type = T;
		using size_type = uint64_t;

		public:
			struct node_t
			{
				value_type value;
				node_t *left = nullptr;
				node_t *right = nullptr;

				node_t (const value_type& val)
				{
					this->value = val;
				}
			};

		private:
			node_t *root = nullptr;
			size_type _size = 0;

		private:
			void add (node_t *node, const value_type& val)
			{
				if (val < node->value) {
					if (node->left == nullptr)
						node->left = new node_t(val);
					else
						return this->add(node->left, val);
				}
				else {
					if (node->right == nullptr)
						node->right = new node_t(val);
					else
						return this->add(node->right, val);
				}
				this->_size++;
			}
			node_t* erase (node_t **parent, node_t *node, const value_type& val) noexcept
			{
				if (node == nullptr)
					return nullptr;
				else if (val > node->value)
					return this->erase(&(node->right), node->right, val);
				else if (val < node->value)
					return this->erase(&(node->left), node->left, val);
				
				if (node->right == nullptr && node->left == nullptr)
					*parent = nullptr;
				else if (node->right == nullptr) 
					*parent = node->left;
				else if (node->left == nullptr)
					*parent = node->right;
				else {
					*parent = node->right;
					node_t *temp = node->right;
					while (temp->left)
						temp = temp->left;
					temp->left = node->left;
				}

				delete node;
				this->_size--;
				return *parent;
			}
			void erase_subtree(node_t **parent, node_t *node) noexcept
			{
				if (node == nullptr)
					return;

				this->erase_subtree(&(node->right), node->right);
				this->erase_subtree(&(node->left), node->left);

				this->_size--;
				delete node;
				*parent = nullptr;
			}
			void erase_leaves (node_t **parent, node_t *node) noexcept
			{
				if (node == nullptr)
					return;

				if (node->right == nullptr && node->left == nullptr) {
					delete node;
					*parent = nullptr;
					this->_size--;
				}
				else {
					this->erase_leaves(&(node->right), node->right);
					this->erase_leaves(&(node->left), node->left);
				}
			}
			void unsafe_erase_subtree (node_t *node) noexcept
			{
				if (node == nullptr)
					return;

				this->unsafe_erase_subtree(node->right);
				this->unsafe_erase_subtree(node->left);

				delete node;
				this->_size--;
			}

		public:
			~binary_tree_t () noexcept
			{
				this->clear();
			}

		// element access
		public:
			inline node_t* data () const noexcept
			{
				return this->root;
			}
			node_t* get (const value_type& val) const noexcept
			{
				if (this->root == nullptr)
					return nullptr;

				node_t *node = this->root;
				while (node->value != val) {
					if (val < node->value)
						node = node->left;
					else
						node = node->right;
					if (node == nullptr)
						break;
				}
				return node;
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
				if (this->root == nullptr) {
					this->root = new node_t(val);
					this->_size++;
				}
				else
					this->add(this->root, val);
			}
			node_t* erase (const value_type& val) noexcept
			{
				return this->erase(&(this->root), this->root, val);
			}
			void erase_subtree (node_t *node) noexcept
			{
				this->erase_subtree(&(node->right), node->right);
				this->erase_subtree(&(node->left), node->left);
			}
			void erase_leaves () noexcept
			{
				this->erase_leaves(&(this->root), this->root);
			}
			void clear () noexcept
			{
				this->unsafe_erase_subtree(this->root);
				this->root = nullptr;
			}

		// others
		public:
			void print_subtree(node_t *node, uint8_t space = 0) const noexcept
			{
				if (node == nullptr)
					return;

				space += 5;
				this->print_subtree(node->right, space);

				for (int i = 5; i < space; i++) {
					if ((space - i) > 5)
						std::cout << " ";
					else {
						if ((space - i) == 5)
							std::cout << "+";
						else
							std::cout << "-";
					}
				}
				std::cout << node->value << "\n";

				this->print_subtree(node->left, space);
			}
			void print () const noexcept
			{
				this->print_subtree(this->root);
			}
	};
};

#endif
